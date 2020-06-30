#include "netvector.h"

int netvector::getSize()
{
	return size;
}

void netvector::append(std::string value)
{
	unsigned char* bytes = (unsigned char*)value.data();
	this->tail = this->tail->add(bytes, value.length());
	this->size = this->size + value.length();
}

void netvector::append(unsigned char* array, int len)
{
	this->tail = this->tail->add(array, len);
	this->size = this->size + len;
}



unsigned char* netvector::at(int index)
{
	if (index >= size)
	{
		return NULL;
	}

	return head.get(index);
}




