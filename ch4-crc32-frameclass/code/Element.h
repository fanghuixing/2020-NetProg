#pragma once
#include <iostream>

// ÿ8���ֽ����һ���ֽ�����
#define MAXSIZE 8

// �ֽ�������
class Element
{
private:
		// ��ǰ�ֽ�����
		unsigned char values[MAXSIZE] = { 0, 0, 0, 0, 0, 0, 0, 0 };

		// ָ����һ���ֽ����� ���γ�����
		Element* post = NULL;

		// ��ǰ�ֽ������Ѵ�ֵ�������������������ֽ�����
		int size = 0;

public:
	// ���Ԫ��
	Element* add(unsigned char* values, int size);

	// ��ȡ�±�index����Ӧ���ֽ�
	unsigned char* get(int index);
};

