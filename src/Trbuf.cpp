#include <climits>

#include "Trbuf.h"

namespace trbuf {
Trbuf::Trbuf(size_t bufsize)
{
    m_buffer = std::make_unique<RingBuffer>(bufsize);
}

size_t Trbuf::GetDataSize(void)
{
	std::lock_guard<std::mutex> lock(m_read_lock);
	return m_buffer->UsedSize();
}

size_t Trbuf::GetFreeSize(void)
{
    std::lock_guard<std::mutex> lock(m_read_lock);
    return m_buffer->FreeSize();
}

size_t Trbuf::GetCapacity(void)
{
    std::lock_guard<std::mutex> lock(m_read_lock);
    return m_buffer->Capacity();
}

ssize_t Trbuf::Read(void *buf, size_t len, int flag)
{
    if (len > SSIZE_MAX)
        return trbuf::EINARG;
    
    std::lock_guard<std::mutex> lock(m_read_lock);

    if (flag == O_COPY) {
        return m_buffer->CopyFromRing((char*)buf, len);
    } else {
        return m_buffer->MoveFromRing((char*)buf, len);
    }
}

ssize_t Trbuf::ReadUtilCh(void *buf, char ch, int flag)
{
    size_t offset;
    std::lock_guard<std::mutex> lock(m_read_lock);

    if (!(offset = m_buffer->FindChOffset(ch))) {
        return trbuf::ECHNFD;
    }

    if (flag == O_COPY) {
        return m_buffer->CopyFromRing((char*)buf, offset);
    } else {
        return m_buffer->MoveFromRing((char*)buf, offset);
    }
}

ssize_t Trbuf::ReadUtilStr(void *buf, const std::string& pattern, int flag)
{
    size_t offset;
    std::lock_guard<std::mutex> lock(m_read_lock);

    if (!(offset = m_buffer->FindStrOffset(pattern))) {
        return trbuf::ESTNFD;
    }

    if (flag == O_COPY) {
        return m_buffer->CopyFromRing((char*)buf, offset);
    } else {
        return m_buffer->MoveFromRing((char*)buf, offset);
    }
}

ssize_t Trbuf::Write(const void *buf, size_t len)
{
    if (len >= SSIZE_MAX)
        return trbuf::EINARG;
    
    std::lock_guard<std::mutex> lock(m_write_lock);

    return m_buffer->CopyToRing((const char *)buf, len);
}
}