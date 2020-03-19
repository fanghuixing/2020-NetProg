// 帧- CRC-32 测试

#include "Frame.h"
#include <WinSock2.h>
#include <vector>
#include <iostream>


using std::fstream;
using std::iostream;
using std::vector;
using std::cout;
using std::hex;
using std::endl;
using std::ios;

#pragma comment(lib,"ws2_32.lib")


// 左移1位，如果读到的值是1，则在crc最低位存1
void Frame::MoveNewBitToCrc(unsigned int& crc, Byte* temp, Byte i)
{
	crc <<= 1;
	if ( (*temp) & i )    //如果当前新读取的位是1
		crc ^= 0x00000001;
}

netvector* Frame::prepareChecingData()
{
	netvector* ckdata = new netvector();
	ckdata->append(this->dest_addr, 6);
	ckdata->append(this->src_addr, 6);
	ckdata->append(htons(this->length)); // 先转成网络字节序，再添加
	ckdata->append(this->data);    	   	
	ckdata->append(MINLEN - this->length + 4, char(0x00));

	return ckdata;
}


Frame::Frame(Byte* dest_addr, Byte* src_addr, string data)
{
	this->dest_addr = dest_addr;
	this->src_addr = src_addr;
	this->data = data;
	this->length = data.length();
}

void Frame::writePreamble(fstream &file)
{	
	for (Byte var : this->preamble)
	{
		file.put(var);
	}
	
}

void Frame::writeSFD(fstream &file)
{
	file.put(this->start_frame_delimiter);
}

void Frame::writeDestAddr(fstream &file)
{
	for (int i = 0; i < 6; i++)
	{
		file.put(this->dest_addr[i]);
	}
	
}

void Frame::writeSrcAddr(fstream &file)
{
	for (int i = 0; i < 6; i++)
	{
		file.put(this->src_addr[i]);
	}	
	
}

void Frame::writeLength(fstream &file)
{
	u_short len = htons(this->length);
	file.write((const char *)&len, 2);
}

void Frame::writeData(fstream &file)
{
	file << this->data;

	for (int i = 0; i < MINLEN - this->length; i++)
	{
		file.put(0x00);
	}
	
}



void Frame::writeCrc(fstream &file)
{

	for (Byte var : this->crc)
	{
		file.put(var);
	}	

}

// 计算CRC码
unsigned int Frame::calcCrc()
{
	netvector* ckdata = prepareChecingData();
	bool xorflag = false;
	unsigned int crc = 0;

	for (unsigned int i = 0; i < ckdata->getSize(); i++)
	{
		Byte* temp = ckdata->at(i);

		for (Byte i = (Byte)0x80; i > 0; i >>= 1)
		{ // i : 1000 0000 ~ 0000 0001
			xorflag = false;
			if (crc & 0x80000000) { // 如果首位是1
				xorflag = true;
			}
			MoveNewBitToCrc(crc, temp, i);
			if (xorflag) {
				crc ^= (unsigned int)CRC32;
			}
		}

	}
	
	delete ckdata;

	return crc;
}

// 设置CRC码
void Frame::setCrc(unsigned int value)
{
	memcpy(this->crc, &value, 4); 
}

// 把帧写入文件
void Frame::frameToFile(fstream &file)
{
	this->writePreamble(file);
	this->writeSFD(file);
	this->writeDestAddr(file);
	this->writeSrcAddr(file);
	this->writeLength(file);
	this->writeData(file);
	this->writeCrc(file);
}

int main(int argc, char* argv[]) {

	if (argc != 2)
	{
		cout << endl << "请按以下格式输入命令行：CrcEncode output_file" << endl;
		return 1;
	}
	   
	// 目的地址
	Byte dest_addr[6] = { 0x00, 0x00, 0xe4, 0x86, 0x3a, 0xdc };
	// 源地址
	Byte src_addr[6] = { 0x00, 0x00, 0x80, 0x1a, 0xe6, 0x65 };
	// 数据
	string data = "Hello world!";


	Frame fr(dest_addr, src_addr, data);

	unsigned int crc = fr.calcCrc();

	cout << hex << crc << endl;

	unsigned int nv = htonl(crc); // 转成网络字节序
	fr.setCrc(nv);

	fstream outfile;  //创建输出文件流
	//注意ios::in不能省略，因为我们需要从文件中读取数据
	outfile.open(argv[1],
		ios::in |
		ios::out |
		ios::binary |
		ios::trunc); //打开输出文件

	fr.frameToFile(outfile);

	outfile.close();

}