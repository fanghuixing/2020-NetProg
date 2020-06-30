
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



//如果输入命令行不正确，则输出提示后退出
int main(int argc, char* argv[])
{
	if (argc != 3) //检查命令行参数
	{
		cout << endl << "请按如下格式输入命令行:FrameEncapsulation input_file"
			<< " out_file" << endl;
		return 1;
	}



	fstream infile; //创建输入文件流
	infile.open(argv[1], ios::in | ios::binary); //打开输入文件



	if (!infile.is_open())
	{
		cout << "无法打开输入文件" << endl;
		return 1;
	}


	//读取输入文件字节
	//char c;
	int length = 0; ////数据长度
	//将指针移到文件末尾
	infile.seekg(0, ios::end);
	//获取文件的总字节
	length = infile.tellg();

	//创建字符指针并根据文件长度初始化
	char* data = new char[length];

	// 你刚才文件指针已经到文件结尾了---------------------
	// 现在要回到文件开头才能读到内容---------------------
	infile.seekg(ios::beg);

	//将文件数据读入到字符指针data中
	infile.read(data, length);


	fstream outfile; //创建输出文件流
	outfile.open(argv[2], ios::out | ios::in | ios::binary | ios::trunc); //以二进制方式打开并可读写，如文件存在，则清楚内容



	int nframenum = 0;//帧序号
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
		cout << endl << "帧" << nframenum << "开始封装" << endl;


		//写入前导码
		for (int i = 0; i < 7; i++)
		{
			outfile.put((char)0xaa);
		}
		outfile.put((char)0xab);//写入帧前定界符

		//写入目的地址
		char des_add[] = { char(0x00),char(0x00),char(0xE4),char(0x86),char(0x3A),char(0xDC) };
		outfile.write(des_add, 6);

		//写入源地址
		char sou_add[] = { char(0x00),char(0x00),char(0x85),char(0x1B),char(0xE6),char(0x78) };
		outfile.write(sou_add, 6);
		//写入数据长度
		int datalen = to - from;
		outfile.put(char(datalen / 256));
		outfile.put(char(datalen % 256));

		cout << endl << "长度字段:";
		
		// 使用hex（16进制）输出的时候不用对数据加char---------------
		cout <<hex<< datalen / 256 << " "<<dec;
		cout <<hex<< datalen % 256 <<dec;

		//将data内容写入到输出文件中
		outfile.write(data, length);

		//如果输入文件长度不足，则补充0
		if (datalen < 46)
		{
			for (int i = 0; i < 46 - datalen; i++)
				outfile.put(char(0x00));
		}




		cout << endl << "数据字段：";
		for (int i = from; i < to; i++)
		{
			cout << data[i];
		}
		cout << endl;
		/*delete[] data;*/

		//写入帧校验字段
		for (int i = 0; i < 4; i++) {
			outfile.put(char(0xff));
		}
	};


	cout << endl << "数据帧文件" << argv[2] << "封装完成" << endl;
	//关闭文件
	outfile.close();
	infile.close();

	return 0;

}