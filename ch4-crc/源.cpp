// ch-4, Ethernet 帧校验,校验码计算
#include <fstream>
#include <iostream>
#include <string>
#include <WinSock2.h>
#include "源.h"
#include <vector> 
#pragma comment(lib,"ws2_32.lib")

using std::ios;
using std::cout;
using std::endl;
using std::fstream;
using std::hex;
using std::dec;
using std::string;
using std::vector;
using std::begin;
using std::end;



int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cout << endl << "请按以下格式输入命令行：CrcEncode output_file" << endl;
		return 1;
	}

	fstream outfile;  //创建输出文件流
	//注意ios::in不能省略，因为我们需要从文件中读取数据
	outfile.open(argv[1],
		ios::in |
		ios::out |
		ios::binary |
		ios::trunc); //打开输出文件



	for (int i = 0; i < 7; i++)                    //写入7B前导码
	{
		outfile.put(char(0xaa));
	}

	outfile.put(char(0xab));                       //写入帧前定界符

	int pos_destination_addr = int(outfile.tellp());         //目的地址字段的位置

	ProcessDestinationAddr(outfile);               //处理目的地址

	ProcessSourceAddr(outfile);                   //处理源地址

	ProcessLengthAndData(outfile);                //处理长度字段和数据字段

	int pos_crc = int(outfile.tellp());             //获得帧校验字段的位置

	outfile.put(char(0x00));      
	outfile.put(char(0x00));
	outfile.put(char(0x00));
	outfile.put(char(0x00));
	//数据后补4*8个0

	int total = int(outfile.tellp()) - pos_destination_addr;   //CRC校验范围的字节数

	outfile.seekg(pos_destination_addr, ios::beg);   //文件（读）指针指向目的地址

	unsigned int crc = 0;                      //初始化CRC余数为0

	CalculateCRCValue(total, outfile, crc);

	outfile.seekp(pos_crc, ios::beg);     //文件（写）指针移动到帧校验字段
	
	cout << endl << "帧校验字段：" << hex << crc  << endl;
	
	crc = htonl(crc);	
	outfile.write((char*)(&crc), sizeof(crc));

	cout << endl << "帧封装与CRC校验完成" << endl;
	outfile.close();

	return 0;
}

void CalculateCRCValue(int total, std::fstream& outfile, unsigned int& crc)
{
	//cout<<"校验范围内数据：";
	bool xorflag = false;						//是否进行异或运算的标志
	while (total-- > 0)
	{
		char temp;
		outfile.get(temp);   //每次读取一个字节
		//输出校验范围内原始数据，16进制格式
		//右对齐输出，大写，2位宽度，不足的前补0
		//printf("%02X ", unsigned char(temp));

		for (unsigned char i = (unsigned char)0x80; i > 0; i >>= 1)
		{ // i : 1000 0000 ~ 0000 0001
			xorflag = false;
			if (crc & 0x80000000) { // 如果首位是1
				xorflag = true;
			}
			MoveNewBitToCrc(crc, temp, i);
			if (xorflag) {
				crc ^= (unsigned int) 0x04C11DB7;
			}
		}
	}
}

// 左移1位，如果读到的值是1，则在crc最低位存1
void MoveNewBitToCrc(unsigned int& crc, char temp, unsigned char i)
{
	crc <<= 1;
	if (unsigned char(temp) & i)    //如果当前新读取的位是1
		crc ^= 0x00000001;
}

/*
处理目的地址
*/
void ProcessDestinationAddr(std::fstream& outfile)
{

	char destination_addr[6] = { char(0x00),char(0x00),
		char(0xe4),char(0x86),
		char(0x3a),char(0xdc) }; //目的地址

	outfile.write(destination_addr, sizeof(destination_addr)); //写入6B目的地址
}

/*
处理源地址
*/
void ProcessSourceAddr(std::fstream& outfile)
{
	char source_addr[6] = { char(0x00), char(0x00),
		char(0x80), char(0x1a),
		char(0xe6), char(0x65) }; //源地址

	outfile.write(source_addr, sizeof(source_addr)); //写入6B源地址
}

/*
处理长度和数据字段
*/
void ProcessLengthAndData(std::fstream& outfile)
{
	char data[] = "Hello world!";				//数据
	int data_length = strlen(data);				//获得数据的长度
	outfile.put(char(data_length / 256));	     //写入长度字段的首字节(高位)
	outfile.put(char(data_length % 256));        //写入长度字段的尾字节(低位)
	cout << endl << "长度字段：" << hex << data_length
		<< dec << "(" << data_length << ")" << endl;

	outfile.write(data, data_length);    //写入数据字段
	cout << "数据字段：" << data << endl;

	// 如果不够46字节就填充0
	for (int i = 0; i < 46 - data_length; i++) outfile.put(0x00);
	
}
