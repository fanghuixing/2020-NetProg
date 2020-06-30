#include <fstream>
#include <iostream>
using namespace std;

// 写16进制数和写字符串的区别

int main()
{

    char data[100];

    cout << "写16进制0xaa" << endl;
    ofstream outfile;
    outfile.open("afile.dat", ios::trunc);
    outfile.put(0xaa); // 写入16进制的值：0xaa(1个字节)
    outfile.close();

    cout << "从文件中读出来的是：" << endl;
    ifstream infile;
    infile.open("afile.dat");
    cout << hex << infile.get() << endl; // 读取一个字节，并以16进制显示处理
    infile.close();

    cout << endl;

    cout << "写字符串aa" << endl;
    ofstream outfile_STR;
    outfile_STR.open("afile.dat", ios::trunc);
    outfile_STR.write("aa", 2); // 写入字符串"aa"(2个字节)
    outfile_STR.close();

    cout << "从文件中读出来的是：" << endl;
    ifstream infile_STR;
    infile_STR.open("afile.dat");
    cout << hex << infile_STR.get();
    cout << hex << infile_STR.get() << endl;
    infile_STR.close();
    return 0;
}