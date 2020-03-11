//  把数据封装成帧

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
	// 当前程序中做输出流
	// 这里变量名和文件名一样
	// 因为我要把这个文件给其他项目做输入文件）
	ofstream input; 
	input.open("input", ios::out | ios::binary);

	//实际待封装数据
	char data[] = "Nankai University was founded in 1919"
		" by the famous patriotic educators in Chinese modern history,"
		" Mr. Zhang Boling and Mr. Yan Xiu.";

	// 目的MAC地址6个字节
	char dst_addr[6] = { char(0x00), char(0x00), char(0xe4),
		char(0x86), char(0x3a), char(0xdc) };

	// 源MAC地址6个字节
	char src_addr[6] = { char(0x00), char(0x00), char(0x80),
			char(0x1a), char(0xe6), char(0x65) };

	// 数据量（字节数）
	int length = strlen(data);

	int from = 0, to = 0;

	// 每个帧最多封装100个字节，可能有多个帧哦
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

		// 写入前导码7个字节
		for (int i = 0; i < 7; i++)
			input.put(char(0xaa));

		// 写入帧前定界符1个字节
		input.put(char(0xab));

		// 写入目的地址
		input.write(dst_addr, sizeof(dst_addr));

		// 写入源地址
		input.write(src_addr, sizeof(src_addr));

		// 当前帧的数据长度（字节数）
		int datalen = to - from;

		// 写入长度值高位字节
		// （既然我们这里最大值是100，那么高位都是0）
		input.put(char(datalen / 256));

		// 写入长度值低位字节
		input.put(char(datalen % 256));

		// 写入具体数据
		for (int i = from; i < to; i++)
		{
			input.put(data[i]); // 写入一个字节
			cout << data[i];
		}
		cout << endl;

		// 如果当前帧的数据长度（字节数）不足46个字节
		// 则用0填满46个字节
		if (datalen < 46)
		{
			for (int i = 0; i < 46 - datalen; i++)
				input.put(char(0x00)); // 每次填一个字节的0
		}

		// 最后写入CRC校验码，暂时不考虑校验
		for (int i = 0; i < 4; i++) {
			input.put(char(0xff));
		}
	}

	input.close(); // 关闭文件流

	return 0;
}