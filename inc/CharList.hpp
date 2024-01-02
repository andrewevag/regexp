#pragma once
#include <iostream>
#include <list>
#include <cstdlib>


class CharList
{

public:
	CharList();
	CharList(std::initializer_list<uint8_t> l);
	~CharList();

	void push_front(uint8_t elem);
	void setList(std::list<uint8_t> l);
	std::list<uint8_t> getList();
private:
	std::list<uint8_t> _data;
};

