#include "fstream"
#include "iostream"
using namespace std;

int main() {
	ofstream file;     // 创建输出文件流，用于创建文件并写入信息
	file.open("file", ios::out | ios::binary); // 文件的位置名称，打开模式
	char data[] = "21706032013计算机二班高寒，完成的是将原始数据进行简单的封装成帧并输出文件，这只是一次简单的测试sddfsdfsdfsdfsfsfeqrfqefwewefsf";
	char dst_addr[6] = { char(0x00), char(0x00), char(0xe4), char(0x86), char(0x3a), char(0xdc) };//写入6B目的地址
	char src_addr[6] = { char(0x00), char(0x00), char(0x80), char(0x1a), char(0xe6), char(0x65) };//写入6B源地址
	int length = strlen(data);  //求字符串长度
	int from = 0, to = 0, frame = 0;
	while (length > 0) {
		if (length >= 100) {		//进行100B判断
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
		cout << endl << "帧" << frame << "开始封装" << endl;
		for (int i = 0; i < 7; i++)  //写入7B前导码
			file.put(char(0xaa));
		file.put(char(0xab));		//写入帧前定界符
		file.write(dst_addr, sizeof(dst_addr));
		file.write(src_addr, sizeof(src_addr));
		int datalen = to - from;
		file.put(char(datalen / 256)); //将该长度写入数据长度字段
		file.put(char(datalen % 256));
		cout << endl << "长度字段：" << hex << datalen
			<< dec << "(" << datalen << ")" << endl;
		cout << endl << "数据字段：";
		for (int i = from; i < to; i++) //写入数据
		{
			file.put(data[i]);
			cout << data[i];
		}
		cout << endl << endl;
		if (datalen < 46)
		{
			for (int i = 0; i < 46 - datalen; i++)
				file.put(char(0x00)); //数据后补8个0
		}
		for (int i = 0; i < 4; i++) {
			file.put(char(0xff));
		}
	}
	file.close(); //关闭输入文件
	return 0;
}