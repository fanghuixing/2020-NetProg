#pragma once
#include <iostream>
#define MAXSIZE 8
class Element
{
private:
		unsigned char values[MAXSIZE] = { 0, 0, 0, 0, 0, 0, 0, 0 };

		Element* post = NULL;

		// 当前已存值的数量，不包括post
		int size = 0;

public:
	// 添加元素
	Element* add(unsigned char* values, int size);

	// 获取下标index所对应的字节
	unsigned char* get(int index);
};

