#pragma once
#include <iostream>
#include "Element.h"
#include <string>

// �����࣬��ֱ����������֡����ķ�װ
// ͨ��ά���ֽ����������ʵ�����ݴ洢����
class netvector
{
private :
	// ���ֽ�����
	Element head;
	
	// β�ֽ�����
	Element* tail = &head;

	// �Ѿ��洢���ֽڸ���
	int size = 0;
	
public:
	// ���ش�С
	int getSize();

	// ����ַ���
	void append(std::string str);

	// ����ֽ����飬�����Сsize
	void append(unsigned char *array, int size);

	// ȡ��index����0��ʼ�����ֽ�
	unsigned char* at(int index);	

	// �����������ͨ��������
	template<typename T>
	void append(T value);

	// ���amount��value
	template<typename T>
	void append(int amount, T value);

	
};

template<typename T>
inline void netvector::append(T value)
{
	unsigned char* bytes = (unsigned char*)&value;
	this->append(bytes, sizeof(value));
}

template<typename T>
inline void netvector::append(int amount, T value)
{
	for (int i = 0; i < amount; i++)
	{
		this->append(value);
	}
}
