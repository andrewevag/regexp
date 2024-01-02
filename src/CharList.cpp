#include "CharList.hpp"


CharList::CharList(){

}
CharList::CharList(std::initializer_list<uint8_t> l)
{
	for(auto elem : l ){
		this->_data.push_back(elem);
	}
}
CharList::~CharList()
{
	
}

void CharList::push_front(uint8_t elem)
{
	this->_data.push_front(elem);
}

void CharList::setList(std::list<uint8_t> l)
{
	this->_data = l;
}
std::list<uint8_t> CharList::getList()
{
	return this->_data;
}