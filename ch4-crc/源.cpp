// ch-4, Ethernet ֡У��,У�������
#include <fstream>
#include <iostream>
#include <string>
#include <WinSock2.h>
#include "Դ.h"
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
		cout << endl << "�밴���¸�ʽ���������У�CrcEncode output_file" << endl;
		return 1;
	}

	fstream outfile;  //��������ļ���
	//ע��ios::in����ʡ�ԣ���Ϊ������Ҫ���ļ��ж�ȡ����
	outfile.open(argv[1],
		ios::in |
		ios::out |
		ios::binary |
		ios::trunc); //������ļ�



	for (int i = 0; i < 7; i++)                    //д��7Bǰ����
	{
		outfile.put(char(0xaa));
	}

	outfile.put(char(0xab));                       //д��֡ǰ�����

	int pos_destination_addr = int(outfile.tellp());         //Ŀ�ĵ�ַ�ֶε�λ��

	ProcessDestinationAddr(outfile);               //����Ŀ�ĵ�ַ

	ProcessSourceAddr(outfile);                   //����Դ��ַ

	ProcessLengthAndData(outfile);                //�������ֶκ������ֶ�

	int pos_crc = int(outfile.tellp());             //���֡У���ֶε�λ��

	outfile.put(char(0x00));      
	outfile.put(char(0x00));
	outfile.put(char(0x00));
	outfile.put(char(0x00));
	//���ݺ�4*8��0

	int total = int(outfile.tellp()) - pos_destination_addr;   //CRCУ�鷶Χ���ֽ���

	outfile.seekg(pos_destination_addr, ios::beg);   //�ļ�������ָ��ָ��Ŀ�ĵ�ַ

	unsigned int crc = 0;                      //��ʼ��CRC����Ϊ0

	CalculateCRCValue(total, outfile, crc);

	outfile.seekp(pos_crc, ios::beg);     //�ļ���д��ָ���ƶ���֡У���ֶ�
	
	cout << endl << "֡У���ֶΣ�" << hex << crc  << endl;
	
	crc = htonl(crc);	
	outfile.write((char*)(&crc), sizeof(crc));

	cout << endl << "֡��װ��CRCУ�����" << endl;
	outfile.close();

	return 0;
}

void CalculateCRCValue(int total, std::fstream& outfile, unsigned int& crc)
{
	//cout<<"У�鷶Χ�����ݣ�";
	bool xorflag = false;						//�Ƿ�����������ı�־
	while (total-- > 0)
	{
		char temp;
		outfile.get(temp);   //ÿ�ζ�ȡһ���ֽ�
		//���У�鷶Χ��ԭʼ���ݣ�16���Ƹ�ʽ
		//�Ҷ����������д��2λ��ȣ������ǰ��0
		//printf("%02X ", unsigned char(temp));

		for (unsigned char i = (unsigned char)0x80; i > 0; i >>= 1)
		{ // i : 1000 0000 ~ 0000 0001
			xorflag = false;
			if (crc & 0x80000000) { // �����λ��1
				xorflag = true;
			}
			MoveNewBitToCrc(crc, temp, i);
			if (xorflag) {
				crc ^= (unsigned int) 0x04C11DB7;
			}
		}
	}
}

// ����1λ�����������ֵ��1������crc���λ��1
void MoveNewBitToCrc(unsigned int& crc, char temp, unsigned char i)
{
	crc <<= 1;
	if (unsigned char(temp) & i)    //�����ǰ�¶�ȡ��λ��1
		crc ^= 0x00000001;
}

/*
����Ŀ�ĵ�ַ
*/
void ProcessDestinationAddr(std::fstream& outfile)
{

	char destination_addr[6] = { char(0x00),char(0x00),
		char(0xe4),char(0x86),
		char(0x3a),char(0xdc) }; //Ŀ�ĵ�ַ

	outfile.write(destination_addr, sizeof(destination_addr)); //д��6BĿ�ĵ�ַ
}

/*
����Դ��ַ
*/
void ProcessSourceAddr(std::fstream& outfile)
{
	char source_addr[6] = { char(0x00), char(0x00),
		char(0x80), char(0x1a),
		char(0xe6), char(0x65) }; //Դ��ַ

	outfile.write(source_addr, sizeof(source_addr)); //д��6BԴ��ַ
}

/*
�����Ⱥ������ֶ�
*/
void ProcessLengthAndData(std::fstream& outfile)
{
	char data[] = "Hello world!";				//����
	int data_length = strlen(data);				//������ݵĳ���
	outfile.put(char(data_length / 256));	     //д�볤���ֶε����ֽ�(��λ)
	outfile.put(char(data_length % 256));        //д�볤���ֶε�β�ֽ�(��λ)
	cout << endl << "�����ֶΣ�" << hex << data_length
		<< dec << "(" << data_length << ")" << endl;

	outfile.write(data, data_length);    //д�������ֶ�
	cout << "�����ֶΣ�" << data << endl;

	// �������46�ֽھ����0
	for (int i = 0; i < 46 - data_length; i++) outfile.put(0x00);
	
}
