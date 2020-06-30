#pragma once
#include <iostream>
#include "Element.h"
#include <string>

// 序列类，可直接用于数据帧或包的封装
// 通过维护字节数组的链表实现数据存储访问
class netvector
{
private :
	// 首字节数组
	Element head;
	
	// 尾字节数组
	Element* tail = &head;

	// 已经存储的字节个数
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
