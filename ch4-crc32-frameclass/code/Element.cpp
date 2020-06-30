#include "Element.h"



// 添加多个值amount>=1
Element* Element::add(unsigned char* newvals, int amount)
{
	int index = 0;
	while(size < MAXSIZE && index < amount) {
		values[size++] = newvals[index++];
	}

	// 如果还有值没有加入
	if (amount > index) {
		if (post == NULL)
		{
			Element* newElement = new Element();
			post = newElement;
		}

		return post->add(newvals + index, amount - index);
	}

	return this;
}

unsigned char* Element::get(int index)
{
	if (index < size)
		return values + index;

	if (this->post == NULL)
	{
		return NULL;
	}

	return this->post->get(index - size);
}
