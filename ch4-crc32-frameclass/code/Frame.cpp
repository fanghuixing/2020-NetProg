// ֡- CRC-32 ����
// 4���ֽڵ�CRCУ����

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

// ws2_32.lib�Ƕ�̬���ӿ�ws2_32.dll�ĵ����
// An import library (.lib) file contains 
// information the linker needs to resolve 
// external references to exported DLL functions, 
// so the system can locate the specified DLL 
// and exported DLL functions at run time. You 
// can create an import library for your DLL 
// when you build your DLL.
// https://docs.microsoft.com/zh-cn/windows/win32/dlls/dynamic-link-library-creation?redirectedfrom=MSDN#using-an-import-library
#pragma comment(lib,"ws2_32.lib") // ���Ǿ�̬���ӿ�


// ����1λ�����������ֵ��1������crc���λ��1
void Frame::NewBitToCrc(unsigned int& crc, Byte* temp, Byte i)
{
	crc <<= 1; // ����1λ
	if ( (*temp) & i )    //�����ǰ�¶�ȡ��λ��1
		crc ^= 0x00000001; // ��1�洢��crc�����λ
}

netvector* Frame::prepareChecingData()
{
	netvector* ckdata = new netvector();
	ckdata->append(this->dest_addr, 6);
	ckdata->append(this->src_addr, 6);
	ckdata->append(htons(this->length)); // ��ת�������ֽ��������
	ckdata->append(this->data);   
	// ���0
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
	cout << "ǰ����:";
	for (Byte var : this->preamble)
	{
		printf("%X", var);
		
		file.put(var);
	}
	cout << endl;
}

void Frame::writeSFD(fstream &file)
{
	cout << "֡ǰ�����:";
	file.put(this->start_frame_delimiter);
	printf("%X\n", this->start_frame_delimiter);	
}

void Frame::writeDestAddr(fstream &file)
{
	cout << "Ŀ�ĵ�ַ:";
	for (int i = 0; i < 6; i++)
	{
		file.put(this->dest_addr[i]);		
		printf("%X", this->dest_addr[i]);
	}
	cout << endl;
}

void Frame::writeSrcAddr(fstream &file)
{
	cout << "Դ��ַ:";
	for (int i = 0; i < 6; i++)
	{
		file.put(this->src_addr[i]);
		printf("%X", this->src_addr[i]);
	}	
	cout << endl;
	
}

void Frame::writeLength(fstream &file)
{
	cout << "����:";
	u_short len = htons(this->length);
	file.write((const char *)&len, 2);
	printf("%04X\n", this->length);
}

void Frame::writeData(fstream &file)
{
	cout << "����:" ;
	file << this->data;
	cout << this->data << endl;

	for (int i = 0; i < MINLEN - this->length; i++)
	{
		file.put(0x00);
	}
	
}



void Frame::writeCrc(fstream &file)
{
	cout << "CRCУ����:";
	for (Byte var : this->crc)
	{
		file.put(var);
		printf("%02X", var);
	}	
	cout << endl;
}

// ����CRC��
unsigned int Frame::calcCrc()
{
	netvector* ckdata = prepareChecingData(); // ׼������CRC���������
	bool xorflag = false; // �Ƿ������ɶ���ʽ��Ӧ���������ģ2��������
	unsigned int crc = 0; // CRCУ����

	for (unsigned int i = 0; i < ckdata->getSize(); i++)
	{
		Byte* temp = ckdata->at(i); // ȡ1���ֽ�

		// 0x80 =  1000 0000
		// ��temp�ֽ�����λ���д���
		for (Byte i = (Byte)0x80; i > 0; i >>= 1)
		{ // i : 1000 0000 ~ 0000 0001
			xorflag = false; // ģ2������־�ص�false
			// 0x80000000 = 1000 0000 ...
			if (crc & 0x80000000) { // �����λ��1
				xorflag = true; // ��Ҫȥ��ģ2����
			}
			NewBitToCrc(crc, temp, i); // temp�е�ǰ��λ�ǲ���1
			if (xorflag) {
				crc ^= (unsigned int)CRC32; // ģ2����
			}
		} // ������һ���ֽ�

	}
	
	delete ckdata;

	return crc;
}

// ����CRC��
void Frame::setCrc(unsigned int value)
{
	memcpy(this->crc, &value, 4); 
}

// ��֡д���ļ�
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
			"�밴���¸�ʽ���������У�CrcEncode output_file" << endl;
		return 1;
	}
	   
	// Ŀ�ĵ�ַ
	Byte dest_addr[6] = { 0x00, 0x00, 0xe4, 0x86, 0x3a, 0xdc };
	// Դ��ַ
	Byte src_addr[6] = { 0x00, 0x00, 0x80, 0x1a, 0xe6, 0x65 };
	// ����
	string data = "Hello world!"; // ʵ�ʴ�������ݣ�û�а�������0
	
	// ����֡����
	Frame fr(dest_addr, src_addr, data);

	// ����CRCУ���루4�ֽڣ�
	unsigned int crc = fr.calcCrc();

	unsigned int nv = htonl(crc); // ת�������ֽ���    
								  
	fr.setCrc(nv);   // CRCУ���뱣�浽֡��

	fstream outfile;  //��������ļ���
	
	// ������ļ�
	outfile.open(argv[1], ios::out | ios::binary | ios::trunc); 

	// ��֡д���ļ�
	fr.frameToFile(outfile);

	// �ر��ļ�
	outfile.close();

}