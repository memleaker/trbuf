#include <cstring>
#include <cstdlib>
#include <algorithm>

#include <iostream>

#include "RingBuffer.h"

namespace trbuf {
RingBuffer::RingBuffer(size_t size)
{
    if (!is_power_of_two(size)) {
        size = roundup_pow_of_two(size);
    }

    try {
        m_in = 0;
        m_out = 0;
        m_size = size;
	    m_buffer = std::make_unique<char[]>(size);
    } catch(std::bad_alloc &e) {
        m_size = 0;
        throw e;
    }
}

size_t RingBuffer::FindChOffset(char ch)
{
    size_t rlen {0}; // rlen is int ? or size_t
    size_t outpos {0};
    void *ptr {nullptr};

    std::lock_guard<std::mutex> lock(m_read_lock);

    /* get outpos */
    outpos = m_out & (m_size-1);

    /* right length */
    rlen = std::min(UsedSize(), m_size - outpos);
    ptr = std::memchr(m_buffer.get()+outpos, ch, rlen);
    if (ptr) {
        return (static_cast<char*>(ptr) - (m_buffer.get()+outpos) + 1);
    }

    ptr = std::memchr(m_buffer.get(), ch, UsedSize() - rlen);
    if (ptr) {
        return (static_cast<char*>(ptr) - m_buffer.get()) + rlen + 1;
    }

    return 0;
}

size_t RingBuffer::FindStrOffset(const std::string& str)
{
    size_t size {0};
    // KMP
    std::lock_guard<std::mutex> lock(m_read_lock);
}

size_t RingBuffer::CopyFromRing(char *base, size_t len)
{
    size_t rlen {0};
    size_t outpos {0};

    std::lock_guard<std::mutex> lock(m_read_lock);

    /* get outpos */
    outpos = m_out & (m_size-1);

    /* get readable length */
    len = std::min(len, UsedSize());

    /* right length */
    rlen = std::min(len, m_size - outpos);

    memcpy(base, m_buffer.get()+outpos, rlen);
    memcpy(base + rlen, m_buffer.get(), len - rlen);

    return len;
}

size_t RingBuffer::MoveFromRing(char *base, size_t len)
{
    size_t rlen {0};
    size_t outpos {0};

    std::lock_guard<std::mutex> lock(m_read_lock);

    /* get outpos */
    outpos = m_out & (m_size-1);

    /* get readable length */
    len = std::min(len, UsedSize());

    /* right length */
    rlen = std::min(len, m_size - outpos);

    memcpy(base, m_buffer.get()+outpos, rlen);
    memcpy(base + rlen, m_buffer.get(), len - rlen);

    m_out += len;

    return len;
}

size_t RingBuffer::CopyToRing(const char *base, size_t len)
{
    size_t rlen {0};
    size_t inpos {0};

    std::lock_guard<std::mutex> lock(m_write_lock);

    /* get inpos */
    inpos = m_in & (m_size-1);

    /* get writable length */
    len = std::min(len, FreeSize());

    /* right length */
    rlen = std::min(len, m_size - inpos);

    memcpy(m_buffer.get()+inpos, base, rlen);
    memcpy(m_buffer.get(), base + rlen, len - rlen);

    m_in += len;

    return len;
}

size_t RingBuffer::fls(size_t x)
{
	size_t r = 32;

	if (!x)
		return 0;
	if (!(x & 0xffff0000u)) {
		x <<= 16;
		r -= 16;
	}
	if (!(x & 0xff000000u)) {
		x <<= 8;
		r -= 8;
	}
	if (!(x & 0xf0000000u)) {
		x <<= 4;
		r -= 4;
	}
	if (!(x & 0xc0000000u)) {
		x <<= 2;
		r -= 2;
	}
	if (!(x & 0x80000000u)) {
		x <<= 1;
		r -= 1;
	}
	return r;
}

size_t RingBuffer::roundup_pow_of_two(size_t size)
{
    size_t fls_long;

    if (sizeof(size) == 4) {
        fls_long = fls(size);
    } else {
        fls_long = (size>>32) ? (fls(size>>32)+32) : fls(size);
    }

    return 1UL << fls_long;
}

}
