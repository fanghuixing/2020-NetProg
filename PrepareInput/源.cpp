//  �����ݷ�װ��֡

#include <fstream>
#include <iostream>

/*
 Output stream class to operate on files.
 Objects of this class maintain a filebuf object as their 
 internal stream buffer, which performs input/output operations 
 on the file they are associated with (if any).
*/
using std::ofstream; 

/*
 Base class for all stream classes using 
 narrow characters(of type char)
*/
using std::ios;

/*
 Standard output stream
 Object of class ostream that represents the standard 
 output stream oriented to narrow characters(of type char).

 It corresponds to the C stream stdout.

 The standard output stream is the default destination 
 of characters determined by the environment. 

 This destination may be shared with more standard 
 objects (such as cerr or clog).

 As an object of class ostream, characters can be written to 
 it either as formatted data using the insertion operator 
 (operator<<) or as unformatted data, using member 
 functions such as write.

 The object is declared in header <iostream> with external linkage 
 and static duration: it lasts the entire duration of the program.
*/
using std::cout;

/*
Insert newline and flush
Inserts a new-line character and flushes the stream.

Its behavior is equivalent to calling os.put('\n') 
(or os.put(os.widen('\n')) for character types other than char), 
and then os.flush().
*/
using std::endl;


int main() {
	// ��ǰ�������������
	// ������������ļ���һ��
	// ��Ϊ��Ҫ������ļ���������Ŀ�������ļ���
	ofstream input; 
	input.open("input", ios::out | ios::binary);

	//ʵ�ʴ���װ����
	char data[] = "Nankai University was founded in 1919"
		" by the famous patriotic educators in Chinese modern history,"
		" Mr. Zhang Boling and Mr. Yan Xiu.";

	// Ŀ��MAC��ַ6���ֽ�
	char dst_addr[6] = { char(0x00), char(0x00), char(0xe4),
		char(0x86), char(0x3a), char(0xdc) };

	// ԴMAC��ַ6���ֽ�
	char src_addr[6] = { char(0x00), char(0x00), char(0x80),
			char(0x1a), char(0xe6), char(0x65) };

	// ���������ֽ�����
	int length = strlen(data);

	int from = 0, to = 0;

	// ÿ��֡����װ100���ֽڣ������ж��֡Ŷ
	while (length > 0) {
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

		// д��ǰ����7���ֽ�
		for (int i = 0; i < 7; i++)
			input.put(char(0xaa));

		// д��֡ǰ�����1���ֽ�
		input.put(char(0xab));

		// д��Ŀ�ĵ�ַ
		input.write(dst_addr, sizeof(dst_addr));

		// д��Դ��ַ
		input.write(src_addr, sizeof(src_addr));

		// ��ǰ֡�����ݳ��ȣ��ֽ�����
		int datalen = to - from;

		// д�볤��ֵ��λ�ֽ�
		// ����Ȼ�����������ֵ��100����ô��λ����0��
		input.put(char(datalen / 256));

		// д�볤��ֵ��λ�ֽ�
		input.put(char(datalen % 256));

		// д���������
		for (int i = from; i < to; i++)
		{
			input.put(data[i]); // д��һ���ֽ�
			cout << data[i];
		}
		cout << endl;

		// �����ǰ֡�����ݳ��ȣ��ֽ���������46���ֽ�
		// ����0����46���ֽ�
		if (datalen < 46)
		{
			for (int i = 0; i < 46 - datalen; i++)
				input.put(char(0x00)); // ÿ����һ���ֽڵ�0
		}

		// ���д��CRCУ���룬��ʱ������У��
		for (int i = 0; i < 4; i++) {
			input.put(char(0xff));
		}
	}

	input.close(); // �ر��ļ���

	return 0;
}