// 帧- CRC-32 测试

#include "Frame.h"
#include <WinSock2.h>
#include <vector>
#include <iostream>
#include <iomanip> 

using std::fstream;
using std::iostream;
using std::cout;
using std::endl;
using std::ios;
using std::hex;
using std::dec;

// 动态连接库ws2_32.dll的导入库
// An import library (.lib) file contains 
// information the linker needs to resolve 
// external references to exported DLL functions, 
// so the system can locate the specified DLL 
// and exported DLL functions at run time. You 
// can create an import library for your DLL 
// when you build your DLL.
// https://docs.microsoft.com/zh-cn/windows/win32/dlls/dynamic-link-library-creation?redirectedfrom=MSDN#using-an-import-library
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
	cout << "前导码:";
	for (Byte var : this->preamble)
	{
		printf("%X", var);
		
		file.put(var);
	}
	cout << endl;
}

void Frame::writeSFD(fstream &file)
{
	cout << "帧前定界符:";
	file.put(this->start_frame_delimiter);
	printf("%X\n", this->start_frame_delimiter);
	
}

void Frame::writeDestAddr(fstream &file)
{
	cout << "目的地址:";
	for (int i = 0; i < 6; i++)
	{
		file.put(this->dest_addr[i]);		
		printf("%X", this->dest_addr[i]);
	}
	cout << endl;
}

void Frame::writeSrcAddr(fstream &file)
{
	cout << "源地址:";
	for (int i = 0; i < 6; i++)
	{
		file.put(this->src_addr[i]);
		printf("%X", this->src_addr[i]);
	}	
	cout << endl;
	
}

void Frame::writeLength(fstream &file)
{
	cout << "长度:";
	u_short len = htons(this->length);
	file.write((const char *)&len, 2);
	printf("%04X\n", this->length);
}

void Frame::writeData(fstream &file)
{
	cout << "数据:" ;
	file << this->data;
	cout << this->data << endl;

	for (int i = 0; i < MINLEN - this->length; i++)
	{
		file.put(0x00);
	}
	
}



void Frame::writeCrc(fstream &file)
{
	cout << "CRC校验码:";
	for (Byte var : this->crc)
	{
		file.put(var);
		printf("%02X", var);
	}	
	cout << endl;
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