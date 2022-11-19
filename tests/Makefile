TARGET:RingBufferTest TrbufTest

CC=g++
CXXFLAGS= -I /usr/include/trbuf -L /usr/lib -g -Wall -std=c++17
LDFLAGS=  -ltrbuf -lpthread

RingBufferTest:RingBufferTest.cpp
	${CC} $^ -o $@ ${CXXFLAGS} ${LDFLAGS}

TrbufTest:TrbufTest.cpp
	${CC} $^ -o $@ ${CXXFLAGS} ${LDFLAGS}

.PHONY:
clean:
	-rm RingBufferTest TrbufTest
