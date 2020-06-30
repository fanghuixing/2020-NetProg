#pragma once
// ֡
#include <vector>
#include <string>
#include <fstream>
#include <stdlib.h>
#include "netvector.h"

using std::string;
using std::fstream;
using std::array;
using std::vector;
typedef unsigned char Byte;
#define MINLEN 46
#define CRC32 0x04C11DB7
class Frame
{
private: 
	// ǰ����
	Byte preamble[7] = { 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa};

	// ֡ǰ�����
	Byte start_frame_delimiter = 0xab;

	// Ŀ�ĵ�ַ
	Byte* dest_addr;

	// Դ��ַ
	Byte* src_addr;

	// ����
	string data = "Hello world!";

	// ����
	unsigned short  length = data.length();

	
	// crc У����
	Byte crc[4] = { 0x00, 0x00, 0x00, 0x00 };

	void NewBitToCrc(unsigned int& crc, Byte* temp, Byte i);

	netvector* prepareChecingData();

public:
	Frame(Byte* dest_addr, Byte* src_addr, string data);
	void writePreamble(fstream &file);
	void writeSFD(fstream &file);
	void writeDestAddr(fstream &file);
	void writeSrcAddr(fstream &file);
	void writeLength(fstream &file);
	void writeData(fstream &file);
	void writeCrc(fstream &file);
	unsigned int calcCrc();
	/*����CRC��*/
	void setCrc(unsigned int value);

	void frameToFile(fstream &file);

};

