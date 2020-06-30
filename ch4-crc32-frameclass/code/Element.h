#pragma once
#include <iostream>

// 每8个字节组成一个字节数组
#define MAXSIZE 8

// 字节数组类
class Element
{
private:
		// 当前字节数组
		unsigned char values[MAXSIZE] = { 0, 0, 0, 0, 0, 0, 0, 0 };

		// 指向下一个字节数组 （形成链表）
		Element* post = NULL;

		// 当前字节数组已存值的数量，不包括其他字节数组
		int size = 0;

public:
	// 添加元素
	Element* add(unsigned char* values, int size);

	// 获取下标index所对应的字节
	unsigned char* get(int index);
};

