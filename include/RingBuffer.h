#ifndef __TRBUF_RINGBUFFER_H__
#define __TRBUF_RINGBUFFER_H__

#include <cstddef>
#include <cstring>
#include <memory>
#include <string>
#include <list>
#include <mutex>

namespace trbuf {

class RingBuffer
{
public:
    RingBuffer(size_t size);

    // delete
    RingBuffer() = delete;
    RingBuffer(const RingBuffer&) = delete;
    void operator=(const RingBuffer &) = delete;

    // find
    size_t FindChOffset(char ch);
    size_t FindStrOffset(const std::string& str);

    inline bool   IsFull(void)   { return ((m_in - m_out) == m_size); }
    inline bool   IsEmpty(void)  { return (m_in == m_out); }
    inline size_t UsedSize(void) { return (m_in - m_out); }
    inline size_t FreeSize(void) { return ((m_size) - (m_in - m_out)); }
    inline size_t Capacity(void) { return m_size; }

    // just copy, not change pointer.
    size_t CopyFromRing(char *base, size_t len);

    // copy and change in out pointer.
    size_t MoveFromRing(char *base, size_t len);
    size_t CopyToRing(const char *base, size_t len);

private:
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
