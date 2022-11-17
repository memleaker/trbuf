#include <iostream>

#include <thread>
#include <unistd.h>
#include <Trbuf.h>

void TestThread1(trbuf::Trbuf &buf)
{
	char str[16];

	for (auto i = 0; i < 10000; i++)
	{
		buf.Write("Thread1", 8);
		buf.Read(str, 8);
	}

	sleep(1);

	buf.Write("Thread1", 8);
}

void TestThread2(trbuf::Trbuf &buf)
{
	char str[16];

	for (auto i = 0; i < 10000; i++)
	{
		buf.Write("Thread2", 8);
		buf.Read(str, 8);
	}

	sleep(1);

	buf.Write("Thread2", 8);
}

int main(void)
{
    trbuf::Tlv<int, int, char[10]> t, t1, t2;
    trbuf::Trbuf buf(8191);
    char http[] = "GET /index HTTP1.1\r\nUser-Agent:Test\r\n\r\nHTTP body is Here";
    char buff[8192];

    std::cout << "-----------------------------" << std::endl;
    std::cout << "Initialize" << std::endl;
    std::cout << buf.GetCapacity() << std::endl;
    std::cout << buf.GetDataSize() << std::endl;
    std::cout << buf.GetFreeSize() << std::endl;
    std::cout << "-----------------------------\n" << std::endl;

    std::cout << "-----------------------------" << std::endl;
    std::cout << "Write: " << buf.Write(http, sizeof(http)) << " bytes" << std::endl;
    std::cout << buf.GetCapacity() << std::endl;
    std::cout << buf.GetDataSize() << std::endl;
    std::cout << buf.GetFreeSize() << std::endl;
    std::cout << "-----------------------------\n" << std::endl;

    std::cout << "-----------------------------\n" << std::endl;
    std::cout << "Read (Copy)" << buf.Read(buff, sizeof(http), O_COPY) << " bytes" << std::endl;
    std::cout << "Read: " << buff << std::endl;
    std::cout << buf.GetCapacity() << std::endl;
    std::cout << buf.GetDataSize() << std::endl;
    std::cout << buf.GetFreeSize() << std::endl;
    std::cout << "-----------------------------\n" << std::endl;

    std::cout << "-----------------------------" << std::endl;
    std::cout << "Read (Move)" << buf.Read(buff, sizeof(http)) << " bytes" << std::endl;
    std::cout << "Read: " << buff << std::endl;
    std::cout << buf.GetCapacity() << std::endl;
    std::cout << buf.GetDataSize() << std::endl;
    std::cout << buf.GetFreeSize() << std::endl;
    std::cout << "-----------------------------\n" << std::endl;

    std::cout << "-----------------------------" << std::endl;
    std::cout << "Write " << buf.Write(http, sizeof(http)) << " bytes" << std::endl;
    std::cout << buf.GetCapacity() << std::endl;
    std::cout << buf.GetDataSize() << std::endl;
    std::cout << buf.GetFreeSize() << std::endl;
    std::cout << "-----------------------------\n" << std::endl;

    std::cout << "-----------------------------" << std::endl;
    memset(buff, 0, sizeof(buff));
    std::cout << "ReadUtil '\\n'; Read: " << buf.ReadUtilCh(buff, '\n') << " bytes: ";
    std::cout << buff << std::endl;

    memset(buff, 0, sizeof(buff));
    std::cout << "ReadUtilStr \"\\r\\n\\r\\n\"; Read: " << buf.ReadUtilStr(buff, "\r\n\r\n") << " bytes: ";
    std::cout << buff << std::endl;

    memset(buff, 0, sizeof(buff));
    std::cout << "ReadUtil '\\0'; Read: " << buf.ReadUtilCh(buff, '\0') << " bytes: ";
    std::cout << buff << std::endl;

    std::cout << buf.GetCapacity() << std::endl;
    std::cout << buf.GetDataSize() << std::endl;
    std::cout << buf.GetFreeSize() << std::endl;
    std::cout << "-----------------------------\n" << std::endl;

    std::cout << "-----------------------------" << std::endl;
    t.type = 1;
    t.length = 10;
    strncpy(t.value, "testtestt", 10);
    
    std::cout << "Write Structure: " << buf.Write(&t, sizeof(t)) << " bytes" << std::endl;
    std::cout << buf.GetCapacity() << std::endl;
    std::cout << buf.GetDataSize() << std::endl;
    std::cout << buf.GetFreeSize() << std::endl;
    std::cout << "-----------------------------\n" << std::endl;

    std::cout << "-----------------------------" << std::endl;
    std::cout << "Read: " << buf.ReadStructure<trbuf::Tlv<int, int, char[10]>>(&t1, O_COPY) << std::endl;
    std::cout << "ReadStructure: " << t1.type << " " << t1.length << " " << t1.value << std::endl;
    std::cout << buf.GetCapacity() << std::endl;
    std::cout << buf.GetDataSize() << std::endl;
    std::cout << buf.GetFreeSize() << std::endl;
    std::cout << "-----------------------------\n" << std::endl;

    std::cout << "-----------------------------" << std::endl;
    std::cout << "Read: " << buf.ReadTLV<int, int, char[10]>(&t2) << std::endl;
    std::cout << "ReadTLV: " << t2.type << " " << t2.length << " " << t2.value << std::endl;
    std::cout << buf.GetCapacity() << std::endl;
    std::cout << buf.GetDataSize() << std::endl;
    std::cout << buf.GetFreeSize() << std::endl;
    std::cout << "-----------------------------\n" << std::endl;

    trbuf::Tlv<char, int, char[0]> *t3, *t4;
    t3 = (decltype(t3))new char[sizeof(*t3)+10];
    t4 = (decltype(t4))new char[sizeof(*t4)+10];
    t3->type = 'a';
    t3->length = 10;
    strcpy(t3->value, "123456789");

    std::cout << "-----------------------------" << std::endl;
    std::cout << "WriteTLV: " << buf.Write((const void*)t3, sizeof(*t3)+10) << " bytes" << std::endl;
    std::cout << buf.GetCapacity() << std::endl;
    std::cout << buf.GetDataSize() << std::endl;
    std::cout << buf.GetFreeSize() << std::endl;
    std::cout << "-----------------------------\n" << std::endl;

    std::cout << "-----------------------------" << std::endl;
    std::cout << "Read: " << buf.ReadTLV<char, int, char[0]>(t4) << std::endl;
    std::cout << "ReadTLV: " << t4->type << " " << t4->length << " " << t4->value << std::endl;
    std::cout << buf.GetCapacity() << std::endl;
    std::cout << buf.GetDataSize() << std::endl;
    std::cout << buf.GetFreeSize() << std::endl;
    std::cout << "-----------------------------" << std::endl;

	std::cout << "-----------------------------------" << std::endl;
	std::cout << "Create 2 Thread: Producer And Consumer" << std::endl;
	std::thread th1(TestThread1, std::ref(buf));
	std::thread th2(TestThread2, std::ref(buf));
	th1.join();
	th2.join();
	std::cout << "Thread Destory" << std::endl;
    std::cout << buf.GetCapacity() << std::endl;
    std::cout << buf.GetDataSize() << std::endl;
    std::cout << buf.GetFreeSize() << std::endl;
    std::cout << "-----------------------------\n" << std::endl;

	std::cout << "-----------------------------------" << std::endl;
    char str1[8],str2[8];
    std::cout << "Read: " << buf.Read(str1, 8) << " bytes :" << str1 << std::endl;
    std::cout << "Read: " << buf.Read(str2, 8) << " bytes :" << str2 << std::endl;
    std::cout << buf.GetCapacity() << std::endl;
    std::cout << buf.GetDataSize() << std::endl;
    std::cout << buf.GetFreeSize() << std::endl;
    std::cout << "-----------------------------" << std::endl;

    return 0;
}