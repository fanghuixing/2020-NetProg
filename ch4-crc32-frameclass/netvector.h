#pragma once
#include <iostream>
#include "Element.h"
#include <string>
#include <vector>

using std::vector;

class netvector
{
private :
	Element head;
	
	Element* tail = &head;

	int size = 0;
	
public:
	// 返回大小
	int getSize();

	// 添加字符串
	void append(std::string str);

	// 添加字节数组，数组大小size
	void append(unsigned char *array, int size);

	// 取第index（从0开始）个字节
	unsigned char* at(int index);	

	// 添加其他的普通类型数据
	template<typename T>
	void append(T value);

	// 添加amount个value
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
