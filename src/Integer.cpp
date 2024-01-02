#include "Integer.hpp"
#include <algorithm>

Integer::Integer(){
	this->_digits = std::vector<bool>();
};

Integer::Integer(int num)
{
	_digits = std::vector<bool>();
	for(size_t i = 0; i < sizeof(num)*8; i++){
		_digits.push_back((num >> i & 0x1));
	}
	while(_digits.back() == false) _digits.pop_back();
}

Integer::Integer(size_t size)
{
	_digits = std::vector<bool>(size, false);
}

Integer::Integer(std::set<size_t> s)
{
	size_t max = 0;
	for(auto elem : s){
		max = std::max(max, elem);
	}
	_digits = std::vector<bool>(max+1, false);
	for(auto elem : s){
		_digits[elem] = true;
	}
}


Integer::~Integer()
{
}

bool Integer::getDigit(size_t index) const
{
	if(index >= this->_digits.size())
		return false;
	return this->_digits[index];
}

void Integer::setDigit(size_t index, bool value)
{
	if(index >= this->_digits.size()){
		std::vector<bool> v(index+1, _digits.get_allocator());
		v[index] = value;
		this->_digits = v;
	}else{
		this->_digits[index] = value;
	}
}

size_t Integer::getAllocatedSize() const{
	return this->_digits.size();
}




bool Integer::posNthAce(size_t n, size_t& result) const
{
	size_t k = n;
	for(size_t i = this->_digits.size()-1; i > 0; i--){
		if(this->_digits[i] == true){
			k--;
			if(k==0){
				result = i;
				return true;
			}
				
		}
	}
	if(k == 1){
		if(this->_digits[0]){
			result = 0;
			return true;
		}
		else return false;
	}
	return false;
}


std::set<size_t> Integer::toIndeces() const {
	std::set<size_t> res;
	for(size_t i = 0; i < this->_digits.size(); i++){
		if(this->_digits[i]){
			res.insert(i);
		}
	}
	return res;
}


bool operator<(const Integer& lhs, const Integer& rhs)
{
	size_t result1, result2;
		bool l, r;
		for(size_t j = 1; j < std::max(lhs._digits.size(), rhs._digits.size()); j++){
			l = lhs.posNthAce(j, result1);
			r = rhs.posNthAce(j, result2);
			if(l && !r){
				return false;
			}
			else if(!l && r){
				return true;
			}
			else if(!l && !r){
				return false;
			}
			else{ //l && r
				if(result1 < result2)
					return true;
				else if(result1 > result2)
					return false;
			}
		}
		return false;
}


std::ostream& operator<<(std::ostream& out, const Integer &i)
{
	for(auto digit : i._digits ){
		out << ((int)digit);
	}
	return out;
}
