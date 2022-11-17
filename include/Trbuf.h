#ifndef __TRBUF_TRBUF_H__
#define __TRBUF_TRBUF_H__

#include <cstddef>
#include <cstdint>
#include <string>
#include <memory>
#include <tuple>
#include <mutex>

#include "RingBuffer.h"

namespace trbuf {

#define O_MOVE 0  // move data from buffer.
#define O_COPY 1  // just copy, buffer nochange.

enum Error {
	EINARG = -1, // Invalid argument
	ECHNFD = -2, // Character not found
	ESTNFD = -3, // String not found
	ENOEOU = -4, // No enough data
};

template<typename T, typename L, typename V>
struct Tlv
{
	T type;
	L length;
	V value;
};

class Trbuf
{
public:
	Trbuf(size_t bufsize);
	~Trbuf();

	// delete
	Trbuf() = delete;
	Trbuf(const Trbuf &) = delete;
	void operator=(const Trbuf &) = delete;

	/**
	 * @return
	 *  return size of data in Trbuf.
	 */
	size_t GetDataSize(void);

	/**
	 * @return
	 *  return size of spare space in Trbuf.
	 */
	size_t GetFreeSize(void);

	/**
	 * @return
	 *  return capacity of Trbuf.
	 */
	size_t GetCapacity(void);

	/**
	 * @param buf
	 *  read bytes from Trbuf to buf.
	 * @param len
	 *  read len bytes from Trbuf to buf.
	 * @param flag
	 *  if flag == O_COPY, Read() just copy bytes to buf, but Trbuf nochange.
	 *  if flag == O_MOVE, Read() copy bytes to buf, and change Trbuf.
	 *  default flag is O_MOVE.
	 * @returns
	 *  On error, error number is returned (err num < 0)
	 *            error EINARG: param len is too long.
	 *  On success, the number of bytes read is returned (>= 0).
	*/
	ssize_t Read(void* buf, size_t len, int flag = O_MOVE);

	/**
	 * @param buf
	 *  read bytes from Trbuf to buf.
	 * @param ch
	 *  read bytes from Trbuf to buf util meet character 'ch'.
	 * @param flag
	 *  if flag == O_COPY, Read() just copy bytes to buf, but Trbuf nochange.
	 *  if flag == O_MOVE, Read() copy bytes to buf, and change Trbuf.
	 *  default flag is O_MOVE.
	 * @returns
	 *  On error, error number is returned (err num < 0)
	 *            error ECHNFD: param ch is not found in Trbuf.
	 *  On success, the number of bytes read is returned (>= 0).
	*/
	ssize_t ReadUtilCh(void *buf, char ch, int flag = O_MOVE);

	/**
	 * @param buf
	 *  read bytes from Trbuf to buf.
	 * @param ch
	 *  read bytes from Trbuf to buf util meet string 'pattern'.
	 * @param flag
	 *  if flag == O_COPY, Read() just copy bytes to buf, but Trbuf nochange.
	 *  if flag == O_MOVE, Read() copy bytes to buf, and change Trbuf.
	 *  default flag is O_MOVE.
	 * @returns
	 *  On error, error number is returned (err num < 0)
	 *            error ESTNFD: param pattern is not found in Trbuf.
	 *  On success, the number of bytes read is returned (>= 0).
	*/
	ssize_t ReadUtilStr(void *buf, const std::string& pattern, int flag = O_MOVE);
	
	/**
	 * @param stru
	 *  read sizeof(stru) bytes from Trbuf to stru.
	 * @param flag
	 *  if flag == O_COPY, Read() just copy bytes to buf, but Trbuf nochange.
	 *  if flag == O_MOVE, Read() copy bytes to buf, and change Trbuf.
	 *  default flag is O_MOVE.
	 * @returns
	 *  On error, error number is returned (err num < 0)
	 *            error ENOEOU: no enough data copy to stru.
	 *  On success, the number of bytes read is returned (>= 0).
	*/
	template<typename S> ssize_t ReadStructure(S *stru, int flag = O_MOVE)
	{
		std::lock_guard<std::mutex> lock(m_read_lock);
    
		if (sizeof(S) > m_buffer->UsedSize()) {
			return trbuf::ENOEOU;
		}

		if (flag == O_COPY) {
			return m_buffer->CopyFromRing((char*)stru, sizeof(S));
		} else {
			return m_buffer->MoveFromRing((char*)stru, sizeof(S));
		}
	}

	/**
	 * @param tlv
	 *  read sizeof(T) + sizeof(L) + tlv.length bytes from Trbuf to stru.
	 * @param flag
	 *  if flag == O_COPY, Read() just copy bytes to buf, but Trbuf nochange.
	 *  if flag == O_MOVE, Read() copy bytes to buf, and change Trbuf.
	 *  default flag is O_MOVE.
	 * @returns
	 *  On error, error number is returned (err num < 0)
	 *            error ENOEOU: no enough data copy to tlv.
	 * 			  error EINARG: L or length is too long.
	 *  On success, the number of bytes read is returned (>= 0).
	*/
	template<typename T, typename L, typename V> ssize_t ReadTLV(Tlv<T, L, V> &tlv, int flag = O_MOVE)
	{
		T type;
		L length;
		size_t n{0}, len{0};

		std::lock_guard<std::mutex> lock(m_read_lock);

		if (sizeof(T) + sizeof(L) > m_buffer->UsedSize()) {
			return trbuf::ENOEOU;
		}

		m_buffer->CopyFromRing((char*)&type, sizeof(T));
		m_buffer->CopyFromRing((char*)&length, sizeof(L));
		
		len = static_cast<size_t>(length);
		if (len > m_buffer->UsedSize()) {
			return trbuf::ENOEOU;
		}

		if (len > SSIZE_MAX) {
			return trbuf::EINARG;
		}

		if (flag == O_COPY) {
			n += m_buffer->CopyFromRing((char*)&tlv.type, sizeof(T));
			n += m_buffer->CopyFromRing((char*)&tlv.length, sizeof(L));
			n += m_buffer->CopyFromRing((char*)&tlv.value, len);
			return n;
		} else {
			n += m_buffer->MoveFromRing((char*)&tlv.type, sizeof(T));
			n += m_buffer->MoveFromRing((char*)&tlv.length, sizeof(L));
			n += m_buffer->MoveFromRing((char*)&tlv.value, len);
			return n;
		}
	}

	/**
	 * @param buf
	 *  write bytes from buf to Trbuf.
	 * @param len
	 *  writes len bytes from buf to Trbuf.
	 * @returns
	 *  On error: error number is returned (error num < 0).
	 *            error EINARG:  param len is too long.
	 *  On success: the number of bytes write is returned (>= 0).
	*/ 
	ssize_t Write(const void *buf, size_t len);
private:
    std::mutex m_read_lock;
    std::mutex m_write_lock;

	std::unique_ptr<RingBuffer> m_buffer;
};
}

#endif