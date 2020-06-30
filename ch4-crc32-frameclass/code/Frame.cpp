// 帧- CRC-32 测试
// 4个字节的CRC校验码

#include "Frame.h"
#include <WinSock2.h>
#include <iostream>

using std::fstream;
using std::iostream;
using std::cout;
using std::endl;
using std::ios;
using std::hex;
using std::dec;

// ws2_32.lib是动态连接库ws2_32.dll的导入库
// An import library (.lib) file contains 
// information the linker needs to resolve 
// external references to exported DLL functions, 
// so the system can locate the specified DLL 
// and exported DLL functions at run time. You 
// can create an import library for your DLL 
// when you build your DLL.
// https://docs.microsoft.com/zh-cn/windows/win32/dlls/dynamic-link-library-creation?redirectedfrom=MSDN#using-an-import-library
#pragma comment(lib,"ws2_32.lib") // 不是静态链接库


// 左移1位，如果读到的值是1，则在crc最低位存1
void Frame::NewBitToCrc(unsigned int& crc, Byte* temp, Byte i)
{
	crc <<= 1; // 左移1位
	if ( (*temp) & i )    //如果当前新读取的位是1
		crc ^= 0x00000001; // 把1存储在crc的最低位
}

netvector* Frame::prepareChecingData()
{
	netvector* ckdata = new netvector();
	ckdata->append(this->dest_addr, 6);
	ckdata->append(this->src_addr, 6);
	ckdata->append(htons(this->length)); // 先转成网络字节序，再添加
	ckdata->append(this->data);   
	// 填充0
	ckdata->append(max(MINLEN - this->length, 0) + 4, char(0x00));

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
	netvector* ckdata = prepareChecingData(); // 准备参与CRC计算的数据
	bool xorflag = false; // 是否与生成多项式对应的码组进行模2减法运算
	unsigned int crc = 0; // CRC校验码

	for (unsigned int i = 0; i < ckdata->getSize(); i++)
	{
		Byte* temp = ckdata->at(i); // 取1个字节

		// 0x80 =  1000 0000
		// 对temp字节所有位进行处理
		for (Byte i = (Byte)0x80; i > 0; i >>= 1)
		{ // i : 1000 0000 ~ 0000 0001
			xorflag = false; // 模2减法标志回到false
			// 0x80000000 = 1000 0000 ...
			if (crc & 0x80000000) { // 如果首位是1
				xorflag = true; // 需要去做模2减法
			}
			NewBitToCrc(crc, temp, i); // temp中当前的位是不是1
			if (xorflag) {
				crc ^= (unsigned int)CRC32; // 模2减法
			}
		} // 处理完一个字节

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
		cout << endl << 
			"请按以下格式输入命令行：CrcEncode output_file" << endl;
		return 1;
	}
	   
	// 目的地址
	Byte dest_addr[6] = { 0x00, 0x00, 0xe4, 0x86, 0x3a, 0xdc };
	// 源地址
	Byte src_addr[6] = { 0x00, 0x00, 0x80, 0x1a, 0xe6, 0x65 };
	// 数据
	string data = "Hello world!"; // 实际传输的数据，没有包括填充的0
	
	// 创建帧对象
	Frame fr(dest_addr, src_addr, data);

	// 计算CRC校验码（4字节）
	unsigned int crc = fr.calcCrc();

	unsigned int nv = htonl(crc); // 转成网络字节序    
								  
	fr.setCrc(nv);   // CRC校验码保存到帧中

	fstream outfile;  //创建输出文件流
	
	// 打开输出文件
	outfile.open(argv[1], ios::out | ios::binary | ios::trunc); 

	// 把帧写入文件
	fr.frameToFile(outfile);

	// 关闭文件
	outfile.close();

}