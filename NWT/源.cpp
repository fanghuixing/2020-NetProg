#include "fstream"
#include "iostream"
using namespace std;

int main() {
	ofstream file;     // ��������ļ��������ڴ����ļ���д����Ϣ
	file.open("file", ios::out | ios::binary); // �ļ���λ�����ƣ���ģʽ
	char data[] = "21706032013���������ߺ�����ɵ��ǽ�ԭʼ���ݽ��м򵥵ķ�װ��֡������ļ�����ֻ��һ�μ򵥵Ĳ���sddfsdfsdfsdfsfsfeqrfqefwewefsf";
	char dst_addr[6] = { char(0x00), char(0x00), char(0xe4), char(0x86), char(0x3a), char(0xdc) };//д��6BĿ�ĵ�ַ
	char src_addr[6] = { char(0x00), char(0x00), char(0x80), char(0x1a), char(0xe6), char(0x65) };//д��6BԴ��ַ
	int length = strlen(data);  //���ַ�������
	int from = 0, to = 0, frame = 0;
	while (length > 0) {
		if (length >= 100) {		//����100B�ж�
			from = to;
			to = from + 100;
			length = length - 100;
		}
		else {
			from = to;
			to = from + length;
			length = 0;
		}
		frame++;
		cout << endl << "֡" << frame << "��ʼ��װ" << endl;
		for (int i = 0; i < 7; i++)  //д��7Bǰ����
			file.put(char(0xaa));
		file.put(char(0xab));		//д��֡ǰ�����
		file.write(dst_addr, sizeof(dst_addr));
		file.write(src_addr, sizeof(src_addr));
		int datalen = to - from;
		file.put(char(datalen / 256)); //���ó���д�����ݳ����ֶ�
		file.put(char(datalen % 256));
		cout << endl << "�����ֶΣ�" << hex << datalen
			<< dec << "(" << datalen << ")" << endl;
		cout << endl << "�����ֶΣ�";
		for (int i = from; i < to; i++) //д������
		{
			file.put(data[i]);
			cout << data[i];
		}
		cout << endl << endl;
		if (datalen < 46)
		{
			for (int i = 0; i < 46 - datalen; i++)
				file.put(char(0x00)); //���ݺ�8��0
		}
		for (int i = 0; i < 4; i++) {
			file.put(char(0xff));
		}
	}
	file.close(); //�ر������ļ�
	return 0;
}