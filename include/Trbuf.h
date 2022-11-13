#ifndef __TRBUF_TRBUF_H__
#define __TRBUF_TRBUF_H__

#include <cstddef>
#include <cstdint>
#include <string>
#include <memory>

#include "RingBuffer.h"

namespace trbuf {

#define O_MOVE 0  // 拷贝时会修改Buffer
#define O_COPY 1  // 只是拷贝, Buffer无变化

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

	Trbuf() = delete;
	Trbuf(const Trbuf &) = delete;
	void operator=(const Trbuf &) = delete;

	// get
	size_t GetDataSize(void);

	// read
	int Read(std::string &buf, uint32_t len, int flag = O_MOVE);
	// 先找到字符串，找到之后再调用Read读取指定长度的串，找不到直接返回找不到
	int ReadUtilStr(std::string &buf, const char* seg, int flag = O_MOVE);
	int ReadUtilCh(std::string &buf, char ch, int flag = O_MOVE);
	template<typename T, typename L, typename V> int ReadTLV(Tlv<T, L, V> &tlv, int flag = O_MOVE);
	template<typename S> int ReadStructure(S *stru, int flag = O_MOVE);

	// write
	int Write(const char *str);
	int Write(const std::string &buf);
private:
	std::shared_ptr<RingBuffer> m_buffer;
};
}

#endif