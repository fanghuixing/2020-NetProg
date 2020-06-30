
#include<fstream>
#include<stdlib.h>
#include <iostream>


using std::cout;
using std::fstream;
using std::endl;
using std::hex;
using std::dec;
using std::streampos;
using std::ios;
using std::string;



//������������в���ȷ���������ʾ���˳�
int main(int argc, char* argv[])
{
	if (argc != 3) //��������в���
	{
		cout << endl << "�밴���¸�ʽ����������:FrameEncapsulation input_file"
			<< " out_file" << endl;
		return 1;
	}



	fstream infile; //���������ļ���
	infile.open(argv[1], ios::in | ios::binary); //�������ļ�



	if (!infile.is_open())
	{
		cout << "�޷��������ļ�" << endl;
		return 1;
	}


	//��ȡ�����ļ��ֽ�
	//char c;
	int length = 0; ////���ݳ���
	//��ָ���Ƶ��ļ�ĩβ
	infile.seekg(0, ios::end);
	//��ȡ�ļ������ֽ�
	length = infile.tellg();

	//�����ַ�ָ�벢�����ļ����ȳ�ʼ��
	char* data = new char[length];

	// ��ղ��ļ�ָ���Ѿ����ļ���β��---------------------
	// ����Ҫ�ص��ļ���ͷ���ܶ�������---------------------
	infile.seekg(ios::beg);

	//���ļ����ݶ��뵽�ַ�ָ��data��
	infile.read(data, length);


	fstream outfile; //��������ļ���
	outfile.open(argv[2], ios::out | ios::in | ios::binary | ios::trunc); //�Զ����Ʒ�ʽ�򿪲��ɶ�д�����ļ����ڣ����������



	int nframenum = 0;//֡���
	int from = 0, to = 0;

	while (length > 0)
	{
		if (length >= 100) {
			from = to;
			to = from + 100;
			length = length - 100;
		}
		else {
			from = to;
			to = from + length;
			length = 0;
		}


		nframenum++;
		cout << endl << "֡" << nframenum << "��ʼ��װ" << endl;


		//д��ǰ����
		for (int i = 0; i < 7; i++)
		{
			outfile.put((char)0xaa);
		}
		outfile.put((char)0xab);//д��֡ǰ�����

		//д��Ŀ�ĵ�ַ
		char des_add[] = { char(0x00),char(0x00),char(0xE4),char(0x86),char(0x3A),char(0xDC) };
		outfile.write(des_add, 6);

		//д��Դ��ַ
		char sou_add[] = { char(0x00),char(0x00),char(0x85),char(0x1B),char(0xE6),char(0x78) };
		outfile.write(sou_add, 6);
		//д�����ݳ���
		int datalen = to - from;
		outfile.put(char(datalen / 256));
		outfile.put(char(datalen % 256));

		cout << endl << "�����ֶ�:";
		
		// ʹ��hex��16���ƣ������ʱ���ö����ݼ�char---------------
		cout <<hex<< datalen / 256 << " "<<dec;
		cout <<hex<< datalen % 256 <<dec;

		//��data����д�뵽����ļ���
		outfile.write(data, length);

		//��������ļ����Ȳ��㣬�򲹳�0
		if (datalen < 46)
		{
			for (int i = 0; i < 46 - datalen; i++)
				outfile.put(char(0x00));
		}




		cout << endl << "�����ֶΣ�";
		for (int i = from; i < to; i++)
		{
			cout << data[i];
		}
		cout << endl;
		/*delete[] data;*/

		//д��֡У���ֶ�
		for (int i = 0; i < 4; i++) {
			outfile.put(char(0xff));
		}
	};


	cout << endl << "����֡�ļ�" << argv[2] << "��װ���" << endl;
	//�ر��ļ�
	outfile.close();
	infile.close();

	return 0;

}