// ֡- CRC-32 ����

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


// ����1λ�����������ֵ��1������crc���λ��1
void Frame::MoveNewBitToCrc(unsigned int& crc, Byte* temp, Byte i)
{
	crc <<= 1;
	if ( (*temp) & i )    //�����ǰ�¶�ȡ��λ��1
		crc ^= 0x00000001;
}

netvector* Frame::prepareChecingData()
{
	netvector* ckdata = new netvector();
	ckdata->append(this->dest_addr, 6);
	ckdata->append(this->src_addr, 6);
	ckdata->append(htons(this->length)); // ��ת�������ֽ��������
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

// ����CRC��
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
			if (crc & 0x80000000) { // �����λ��1
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
		cout << endl << "�밴���¸�ʽ���������У�CrcEncode output_file" << endl;
		return 1;
	}
	   
	// Ŀ�ĵ�ַ
	Byte dest_addr[6] = { 0x00, 0x00, 0xe4, 0x86, 0x3a, 0xdc };
	// Դ��ַ
	Byte src_addr[6] = { 0x00, 0x00, 0x80, 0x1a, 0xe6, 0x65 };
	// ����
	string data = "Hello world!";


	Frame fr(dest_addr, src_addr, data);

	unsigned int crc = fr.calcCrc();

	cout << hex << crc << endl;

	unsigned int nv = htonl(crc); // ת�������ֽ���
	fr.setCrc(nv);

	fstream outfile;  //��������ļ���
	//ע��ios::in����ʡ�ԣ���Ϊ������Ҫ���ļ��ж�ȡ����
	outfile.open(argv[1],
		ios::in |
		ios::out |
		ios::binary |
		ios::trunc); //������ļ�

	fr.frameToFile(outfile);

	outfile.close();

}