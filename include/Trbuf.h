/*
	MIT License

	Copyright (c) 2022 rklu

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/
#ifndef __TRBUF_TRBUF_H__
#define __TRBUF_TRBUF_H__

#include <cstddef>
#include <cstdint>
#include <string>
#include <memory>
#include <tuple>
#include <mutex>
#include <climits>

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
	 *  read TLV from Trbuf to tlv.
	 *  note: structure memory align.
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
	template<typename T, typename L, typename V> ssize_t ReadTLV(Tlv<T, L, V> *tlv, int flag = O_MOVE)
	{
		size_t len{0};
		size_t off {((char*)(&tlv->value)) - ((char*)tlv)};
		std::unique_ptr<char[]> tl = std::make_unique<char[]>(off);

		std::lock_guard<std::mutex> lock(m_read_lock);

		/* Have enough data to fill type and length */
		if (off > m_buffer->UsedSize()) {
			return trbuf::ENOEOU;
		}

		/* copy data to type and length */
		m_buffer->CopyFromRing(tl.get(), off);

		/* calculate value size */
		len = static_cast<size_t>(((decltype(tlv))tl.get())->length);
		len = std::max(len, sizeof(*tlv) - off);  /* some align bytes after value */

		/* len is too big */
		if (len > SSIZE_MAX) {
			return trbuf::EINARG;
		}

		/* have enough to fill value */
		if (len > m_buffer->UsedSize()) {
			return trbuf::ENOEOU;
		}

		/* copy or move totlen to tlv */
		if (flag == O_COPY) {
			return m_buffer->CopyFromRing((char*)tlv, off + len);
		} else {
			return m_buffer->MoveFromRing((char*)tlv, off + len);
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