#ifndef __TRBUF_RINGBUFFER_H__
#define __TRBUF_RINGBUFFER_H__

#include <cstddef>
#include <cstring>
#include <memory>
#include <string>
#include <list>
#include <mutex>
#include <vector>

namespace trbuf {

class RingBuffer
{
public:
    RingBuffer(size_t size);

    // delete
    RingBuffer() = delete;
    RingBuffer(const RingBuffer&) = delete;
    void operator=(const RingBuffer &) = delete;

    /**
	 * @param ch
	 *  find character 'ch' from Trbuf.
	 * @returns
	 *  return offset+1 to the first occurrence of the character 'ch'.
     *  if not found, return 0.
	*/
    size_t FindChOffset(char ch);

    /**
	 * @param pattern
	 *  find string 'pattern' from Trbuf.
	 * @returns
	 *  return offset+strlen(pattern) to the first occurrence of the string.
     *  if not found, return 0.
	*/
    size_t FindStrOffset(const std::string& pattern);

    /**
     * @return 
     *  return RingBuffer is or not full.
    */
    inline bool   IsFull(void)   { return ((m_in - m_out) == m_size); }

    /**
     * @return 
     *  return RingBuffer is or not empty.
    */
    inline bool   IsEmpty(void)  { return (m_in == m_out); }

    /**
     * @return 
     *  return RingBuffer data size.
    */
    inline size_t UsedSize(void) { return (m_in - m_out); }

    /**
     * @return 
     *  return RingBuffer unused or free size.
    */
    inline size_t FreeSize(void) { return ((m_size) - (m_in - m_out)); }

    /**
     * @return 
     *  return RingBuffer capacity.
    */
    inline size_t Capacity(void) { return m_size; }

    /**
     * just copy bytes to base, ringbuffer no change.
     * @param base
     *  copy bytes from ringbuffer to base.
     * @param len
     *  copy len bytes from ringbuffer to base.
     * @return
     *  return the number of bytes copied.
    */
    size_t CopyFromRing(char *base, size_t len);

    /**
     * move bytes to base, ringbuffer will be change.
     * @param base
     *  move bytes from ringbuffer to base.
     * @param len
     *  move len bytes from ringbuffer to base.
     * @return
     *  return the number of bytes moved.
    */
    size_t MoveFromRing(char *base, size_t len);

    /**
     * copy bytes from base to ringbuffer.
     * @param base
     *  copy bytes from base to ringbuffer.
     * @param len
     *  copy len bytes from base to ringbuffer.
     * @return
     *  return the number of bytes copied.
    */
    size_t CopyToRing(const char *base, size_t len);

private:
    void get_nextarr(const std::string &pattern, std::vector<size_t>& next);
    inline bool is_power_of_two(size_t size) { return (size != 0) && ((size & (size-1)) == 0); }
    size_t fls(size_t x);
    size_t roundup_pow_of_two(size_t size);
private:
    size_t m_in;   // write pointer
    size_t m_out;  // read  pointer
    size_t m_size;
    
    std::mutex m_read_lock;
    std::mutex m_write_lock;

    std::unique_ptr<char []> m_buffer;
};
}

#endif
