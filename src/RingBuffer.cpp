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

    m_in = 0;
    m_out = 0;
    m_size = size;
    m_buffer = std::make_unique<char[]>(size);
}

size_t RingBuffer::FindChOffset(char ch)
{
    size_t rlen {0};
    size_t outpos {0};
    void *ptr {nullptr};

    std::lock_guard<std::mutex> lock(m_read_lock);

    /* get outpos */
    outpos = m_out & (m_size-1);

    /* right length */
    rlen = std::min(UsedSize(), m_size - outpos);
    ptr = std::memchr(m_buffer.get()+outpos, ch, rlen);
    if (ptr) {
        return (((char*)ptr) - (m_buffer.get()+outpos) + 1);
    }

    ptr = std::memchr(m_buffer.get(), ch, UsedSize() - rlen);
    if (ptr) {
        return (((char*)ptr) - m_buffer.get()) + rlen + 1;
    }

    return 0;
}

void RingBuffer::get_nextarr(const std::string &pattern, std::vector<size_t>& next)
{
    size_t i {1};
    size_t mep {0}; // maximum equal prefix suffix
    size_t size {pattern.size()};

    next.push_back(0);
    next.push_back(0);

    while (i < size)
    {
        if (mep == 0 || pattern[i] == pattern[mep]) {
            i++;
            mep++;

            if (pattern[i] != pattern[mep]) {
                next.push_back(mep);
            } else {
                next.push_back(next[mep]);
            }
        } else {
            mep = next[mep];
        }
    }
}

// KMP
size_t RingBuffer::FindStrOffset(const std::string& pattern)
{
    size_t r{0}, p{0};
    size_t ring_size {0};
    size_t patt_size {0};
    std::vector<size_t> next;

    std::lock_guard<std::mutex> lock(m_read_lock);

    if (pattern.size() == 0 || UsedSize() == 0)
        return 0;

    r = m_out;
    p = 0;
    ring_size = UsedSize() + m_out;
    patt_size = pattern.size();

    get_nextarr(pattern, next);

    while (r < ring_size && p < patt_size)
    {
        /* if matched, index all increment */
        if ((p == 0) || (m_buffer.get()[r & (m_size-1)] == pattern[p])) {
            p++;
            r++;
        } else {
            /* not match, update p */
            p = next[p];
        }
    }

    /* match whole pattern */
    if (p == patt_size) {
        return (r - m_out);
    } else {
        return 0;
    }
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
