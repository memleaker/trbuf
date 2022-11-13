#ifndef __TRBUF_RINGBUFFER_H__
#define __TRBUF_RINGBUFFER_H__

#include <cstddef>
#include <cstring>
#include <memory>
#include <string>
#include <list>

namespace trbuf {

#define BUFFERLEN 4096

class Buffer
{
public:
    Buffer() : offset(0), end(BUFFERLEN)
    { std::memset(base, 0, BUFFERLEN); }

    size_t offset;
    size_t end;
    u_char base[4096];
};

class RingBuffer
{
public:
    RingBuffer(size_t bufsize);

    size_t FindChOffset(char ch);
    size_t FindStrOffset(const std::string& str);

    size_t CurrSize(void);
    // just copy. 主要用于拷贝TLV的TL，以便查看是否有足够数据给V
    // 不增加POS，每次拷贝都是从相同的起点
    size_t CopyFromRing(u_char *base, size_t len);
    // change offset, currpos
    size_t MoveFromRing(u_char *base, size_t len);
    size_t CopyToRing(u_char *base, size_t len);
private:
    std::list<std::shared_ptr<Buffer>> m_ring;
    std::list<std::shared_ptr<Buffer>>::iterator m_currpos;
};
}

#endif