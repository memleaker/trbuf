#include <iostream>

#include <RingBuffer.h>

int main(void)
{
	char str[10];
	char str3[] = "123456789ewrwewerwerwerwer";
	char str4[sizeof(str3)] = {0};
	char str5[48] = "12345612345612345612345612345612345612345612345";
	char str6[48];
	trbuf::RingBuffer buf(63);

	std::cout << "-----------------------------------" << std::endl;
	std::cout << "Init" << std::endl;
	std::cout << "Cap: " << buf.Capacity() << std::endl;
	std::cout << "Used: " << buf.UsedSize()	<< std::endl;
	std::cout << "Free: " << buf.FreeSize()	<< std::endl;
	std::cout << "Isempty: " << buf.IsEmpty()	<< std::endl;
	std::cout << "IsFull: " << buf.IsFull()	<< std::endl;
	std::cout << "Find 'c'" << buf.FindChOffset('c') << std::endl;
	std::cout << "-----------------------------------" << std::endl;

	std::cout << "-----------------------------------" << std::endl;
	std::cout << "Write:" << buf.CopyToRing("hello", 6) << std::endl;
	std::cout << "Cap: " << buf.Capacity() << std::endl;
	std::cout << "Used: " << buf.UsedSize()	<< std::endl;
	std::cout << "Free: " << buf.FreeSize()	<< std::endl;
	std::cout << "Isempty: " << buf.IsEmpty()	<< std::endl;
	std::cout << "IsFull: " << buf.IsFull()	<< std::endl;
	std::cout << "Find 'o'" << buf.FindChOffset('o') << std::endl;
	std::cout << "-----------------------------------" << std::endl;

	std::cout << "-----------------------------------" << std::endl;
	std::cout << "Read: " << buf.CopyFromRing(str, 6) << std::endl;
	std::cout << "ReadContent: " << str << std::endl;
	std::cout << "Cap: " << buf.Capacity() << std::endl;
	std::cout << "Used: " << buf.UsedSize()	<< std::endl;
	std::cout << "Free: " << buf.FreeSize()	<< std::endl;
	std::cout << "Isempty: " << buf.IsEmpty()	<< std::endl;
	std::cout << "IsFull: " << buf.IsFull()	<< std::endl;
	std::cout << "Find 'o'" << buf.FindChOffset('o') << std::endl;
	std::cout << "-----------------------------------" << std::endl;

	std::cout << "-----------------------------------" << std::endl;
	std::cout << "Move: " << buf.MoveFromRing(str, 6) << std::endl;
	std::cout << "MoveContent: " << str << std::endl;
	std::cout << "Cap: " << buf.Capacity() << std::endl;
	std::cout << "Used: " << buf.UsedSize()	<< std::endl;
	std::cout << "Free: " << buf.FreeSize()	<< std::endl;
	std::cout << "Isempty: " << buf.IsEmpty()	<< std::endl;
	std::cout << "IsFull: " << buf.IsFull()	<< std::endl;
	std::cout << "Find 'o'" << buf.FindChOffset('o') << std::endl;
	std::cout << "-----------------------------------" << std::endl;

	std::cout << "-----------------------------------" << std::endl;
	std::cout << "Write:" << buf.CopyToRing(str3, sizeof(str3)) << std::endl;
	std::cout << "Cap: " << buf.Capacity() << std::endl;
	std::cout << "Used: " << buf.UsedSize()	<< std::endl;
	std::cout << "Free: " << buf.FreeSize()	<< std::endl;
	std::cout << "Isempty: " << buf.IsEmpty()	<< std::endl;
	std::cout << "IsFull: " << buf.IsFull()	<< std::endl;
	std::cout << "Find 'q'" << buf.FindChOffset('q') << std::endl;
	std::cout << "-----------------------------------" << std::endl;

	std::cout << "-----------------------------------" << std::endl;
	std::cout << "Move: " << buf.MoveFromRing(str4, sizeof(str4)) << std::endl;
	std::cout << "MoveContent: " << str4 << std::endl;
	std::cout << "Cap: " << buf.Capacity() << std::endl;
	std::cout << "Used: " << buf.UsedSize()	<< std::endl;
	std::cout << "Free: " << buf.FreeSize()	<< std::endl;
	std::cout << "Isempty: " << buf.IsEmpty()	<< std::endl;
	std::cout << "IsFull: " << buf.IsFull()	<< std::endl;
	std::cout << "Find 'q'" << buf.FindChOffset('q') << std::endl;
	std::cout << "-----------------------------------" << std::endl;

	std::cout << "-----------------------------------" << std::endl;
	std::cout << "Write:" << buf.CopyToRing(str5, sizeof(str5)) << std::endl;
	std::cout << "Cap: " << buf.Capacity() << std::endl;
	std::cout << "Used: " << buf.UsedSize()	<< std::endl;
	std::cout << "Free: " << buf.FreeSize()	<< std::endl;
	std::cout << "Isempty: " << buf.IsEmpty()	<< std::endl;
	std::cout << "IsFull: " << buf.IsFull()	<< std::endl;
	std::cout << "Find '2'" << buf.FindChOffset('2') << std::endl;
	std::cout << "-----------------------------------" << std::endl;

	std::cout << "-----------------------------------" << std::endl;
	std::cout << "Move: " << buf.MoveFromRing(str6, sizeof(str6)) << std::endl;
	std::cout << "MoveContent: " << str6 << std::endl;
	std::cout << "Cap: " << buf.Capacity() << std::endl;
	std::cout << "Used: " << buf.UsedSize()	<< std::endl;
	std::cout << "Free: " << buf.FreeSize()	<< std::endl;
	std::cout << "Isempty: " << buf.IsEmpty()	<< std::endl;
	std::cout << "IsFull: " << buf.IsFull()	<< std::endl;
	std::cout << "Find '2'" << buf.FindChOffset('2') << std::endl;
	std::cout << "-----------------------------------" << std::endl;

	std::cout << "Create 2 Thread: Producer Consumer" << std::endl;

	return 0;
}
