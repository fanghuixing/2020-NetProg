#pragma once
#include <iostream>
#define MAXSIZE 8
class Element
{
private:
		unsigned char values[MAXSIZE] = { 0, 0, 0, 0, 0, 0, 0, 0 };

		Element* pre = NULL;

		Element* post = NULL;

		// 当前已存值的数量
		int size = 0;

public:
	Element* add(unsigned char* values, int size);

	unsigned char* get(int index);
};

