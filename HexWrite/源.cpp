#include <fstream>
#include <iostream>
using namespace std;

// д16��������д�ַ���������

int main()
{

    char data[100];

    cout << "д16����0xaa" << endl;
    ofstream outfile;
    outfile.open("afile.dat", ios::trunc);
    outfile.put(0xaa); // д��16���Ƶ�ֵ��0xaa(1���ֽ�)
    outfile.close();

    cout << "���ļ��ж��������ǣ�" << endl;
    ifstream infile;
    infile.open("afile.dat");
    cout << hex << infile.get() << endl; // ��ȡһ���ֽڣ�����16������ʾ����
    infile.close();

    cout << endl;

    cout << "д�ַ���aa" << endl;
    ofstream outfile_STR;
    outfile_STR.open("afile.dat", ios::trunc);
    outfile_STR.write("aa", 2); // д���ַ���"aa"(2���ֽ�)
    outfile_STR.close();

    cout << "���ļ��ж��������ǣ�" << endl;
    ifstream infile_STR;
    infile_STR.open("afile.dat");
    cout << hex << infile_STR.get();
    cout << hex << infile_STR.get() << endl;
    infile_STR.close();
    return 0;
}