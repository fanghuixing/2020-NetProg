#pragma once
#include <iostream>
#define MAXSIZE 8
class Element
{
private:
		unsigned char values[MAXSIZE] = { 0, 0, 0, 0, 0, 0, 0, 0 };

		Element* post = NULL;

		// ��ǰ�Ѵ�ֵ��������������post
		int size = 0;

public:
	// ���Ԫ��
	Element* add(unsigned char* values, int size);

	// ��ȡ�±�index����Ӧ���ֽ�
	unsigned char* get(int index);
};

