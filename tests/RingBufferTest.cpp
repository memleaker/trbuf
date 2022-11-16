#include <iostream>
#include <thread>

#include <unistd.h>

#include <RingBuffer.h>

void TestThread1(trbuf::RingBuffer &buf)
{
	char str[16];

	for (auto i = 0; i < 10000; i++)
	{
		buf.CopyToRing("Thread1", 8);
		buf.MoveFromRing(str, 8);
	}

	sleep(1);

	buf.CopyToRing("Thread1", 8);
}

void TestThread2(trbuf::RingBuffer &buf)
{
	char str[16];

	for (auto i = 0; i < 10000; i++)
	{
		buf.CopyToRing("Thread2", 8);
		buf.MoveFromRing(str, 8);
	}

	sleep(1);

	buf.CopyToRing("Thread2", 8);
}

int main(void)
{
	char str[10];
	char str3[] = "123456789ewrwewerwerwerwer";
	char str4[sizeof(str3)] = {0};
	char str5[48] = "12345612345612345612345612345612345612345612345";
	char str6[48];
	char str7[8];
	char str8[8];
	trbuf::RingBuffer buf(63);

	std::cout << "-----------------------------------" << std::endl;
	std::cout << "Init" << std::endl;
	std::cout << "Cap: " << buf.Capacity() << std::endl;
	std::cout << "Used: " << buf.UsedSize()	<< std::endl;
	std::cout << "Free: " << buf.FreeSize()	<< std::endl;
	std::cout << "Isempty: " << buf.IsEmpty()	<< std::endl;
	std::cout << "IsFull: " << buf.IsFull()	<< std::endl;
	std::cout << "Find 'c' " << buf.FindChOffset('c') << std::endl;
	std::cout << "Find \"el\" " << buf.FindStrOffset("el") << std::endl;
	std::cout << "-----------------------------------" << std::endl;

	std::cout << "-----------------------------------" << std::endl;
	std::cout << "Write:" << buf.CopyToRing("hello", 6) << std::endl;
	std::cout << "WriteContent: hello" << std::endl;
	std::cout << "Cap: " << buf.Capacity() << std::endl;
	std::cout << "Used: " << buf.UsedSize()	<< std::endl;
	std::cout << "Free: " << buf.FreeSize()	<< std::endl;
	std::cout << "Isempty: " << buf.IsEmpty()	<< std::endl;
	std::cout << "IsFull: " << buf.IsFull()	<< std::endl;
	std::cout << "Find 'o' " << buf.FindChOffset('o') << std::endl;
	std::cout << "Find \"el\" " << buf.FindStrOffset("el") << std::endl;
	std::cout << "-----------------------------------" << std::endl;

	std::cout << "-----------------------------------" << std::endl;
	std::cout << "Read: " << buf.CopyFromRing(str, 6) << std::endl;
	std::cout << "ReadContent: " << str << std::endl;
	std::cout << "Cap: " << buf.Capacity() << std::endl;
	std::cout << "Used: " << buf.UsedSize()	<< std::endl;
	std::cout << "Free: " << buf.FreeSize()	<< std::endl;
	std::cout << "Isempty: " << buf.IsEmpty()	<< std::endl;
	std::cout << "IsFull: " << buf.IsFull()	<< std::endl;
	std::cout << "Find 'o' " << buf.FindChOffset('o') << std::endl;
	std::cout << "Find \"el\" " << buf.FindStrOffset("el") << std::endl;
	std::cout << "-----------------------------------" << std::endl;

	std::cout << "-----------------------------------" << std::endl;
	std::cout << "Move: " << buf.MoveFromRing(str, 6) << std::endl;
	std::cout << "MoveContent: " << str << std::endl;
	std::cout << "Cap: " << buf.Capacity() << std::endl;
	std::cout << "Used: " << buf.UsedSize()	<< std::endl;
	std::cout << "Free: " << buf.FreeSize()	<< std::endl;
	std::cout << "Isempty: " << buf.IsEmpty()	<< std::endl;
	std::cout << "IsFull: " << buf.IsFull()	<< std::endl;
	std::cout << "Find 'o' " << buf.FindChOffset('o') << std::endl;
	std::cout << "Find \"el\" " << buf.FindStrOffset("el") << std::endl;
	std::cout << "-----------------------------------" << std::endl;

	std::cout << "-----------------------------------" << std::endl;
	std::cout << "Write:" << buf.CopyToRing(str3, sizeof(str3)) << std::endl;
	std::cout << "Write Content: " << str3 << std::endl;
	std::cout << "Cap: " << buf.Capacity() << std::endl;
	std::cout << "Used: " << buf.UsedSize()	<< std::endl;
	std::cout << "Free: " << buf.FreeSize()	<< std::endl;
	std::cout << "Isempty: " << buf.IsEmpty()	<< std::endl;
	std::cout << "IsFull: " << buf.IsFull()	<< std::endl;
	std::cout << "Find 'q' " << buf.FindChOffset('q') << std::endl;
	std::cout << "Find \"werw\" " << buf.FindStrOffset("werw") << std::endl;
	std::cout << "-----------------------------------" << std::endl;

	std::cout << "-----------------------------------" << std::endl;
	std::cout << "Move: " << buf.MoveFromRing(str4, sizeof(str4)) << std::endl;
	std::cout << "MoveContent: " << str4 << std::endl;
	std::cout << "Cap: " << buf.Capacity() << std::endl;
	std::cout << "Used: " << buf.UsedSize()	<< std::endl;
	std::cout << "Free: " << buf.FreeSize()	<< std::endl;
	std::cout << "Isempty: " << buf.IsEmpty()	<< std::endl;
	std::cout << "IsFull: " << buf.IsFull()	<< std::endl;
	std::cout << "Find 'q' " << buf.FindChOffset('q') << std::endl;
	std::cout << "Find \"werw\" " << buf.FindStrOffset("werw") << std::endl;
	std::cout << "-----------------------------------" << std::endl;

	std::cout << "-----------------------------------" << std::endl;
	std::cout << "Write:" << buf.CopyToRing(str5, sizeof(str5)) << std::endl;
	std::cout << "Write Content: " << str5 << std::endl;
	std::cout << "Cap: " << buf.Capacity() << std::endl;
	std::cout << "Used: " << buf.UsedSize()	<< std::endl;
	std::cout << "Free: " << buf.FreeSize()	<< std::endl;
	std::cout << "Isempty: " << buf.IsEmpty()	<< std::endl;
	std::cout << "IsFull: " << buf.IsFull()	<< std::endl;
	std::cout << "Find '2' " << buf.FindChOffset('2') << std::endl;
	std::cout << "Find \"561234\" " << buf.FindStrOffset("561234") << std::endl;
	std::cout << "-----------------------------------" << std::endl;

	std::cout << "-----------------------------------" << std::endl;
	std::cout << "Move: " << buf.MoveFromRing(str6, sizeof(str6)) << std::endl;
	std::cout << "MoveContent: " << str6 << std::endl;
	std::cout << "Cap: " << buf.Capacity() << std::endl;
	std::cout << "Used: " << buf.UsedSize()	<< std::endl;
	std::cout << "Free: " << buf.FreeSize()	<< std::endl;
	std::cout << "Isempty: " << buf.IsEmpty()	<< std::endl;
	std::cout << "IsFull: " << buf.IsFull()	<< std::endl;
	std::cout << "Find '2' " << buf.FindChOffset('2') << std::endl;
	std::cout << "Find \"561234\" " << buf.FindStrOffset("561234") << std::endl;
	std::cout << "-----------------------------------" << std::endl;

	std::cout << "-----------------------------------" << std::endl;
	std::cout << "Create 2 Thread: Producer And Consumer" << std::endl;
	std::thread th1(TestThread1, std::ref(buf));
	std::thread th2(TestThread2, std::ref(buf));
	th1.join();
	th2.join();
	std::cout << "Thread Destory" << std::endl;
	std::cout << "Cap: " << buf.Capacity() << std::endl;
	std::cout << "Used: " << buf.UsedSize()	<< std::endl;
	std::cout << "Free: " << buf.FreeSize()	<< std::endl;
	std::cout << "Isempty: " << buf.IsEmpty()	<< std::endl;
	std::cout << "IsFull: " << buf.IsFull()	<< std::endl;
	std::cout << "-----------------------------------" << std::endl;

	std::cout << "-----------------------------------" << std::endl;
	buf.MoveFromRing(str7, 8);
	std::cout << "Move From Buf: " << str7 << std::endl;
	buf.MoveFromRing(str8, 8);
	std::cout << "Move From Buf: " << str8 << std::endl;
	std::cout << "Cap: " << buf.Capacity() << std::endl;
	std::cout << "Used: " << buf.UsedSize()	<< std::endl;
	std::cout << "Free: " << buf.FreeSize()	<< std::endl;
	std::cout << "Isempty: " << buf.IsEmpty()	<< std::endl;
	std::cout << "IsFull: " << buf.IsFull()	<< std::endl;
	std::cout << "-----------------------------------" << std::endl;

	return 0;
}
