#pragma once
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>


/*
 * TODO Change this so it uses memory more efficiently
 *      At the moment uses 8x the memory it needs by storing booleans
 */
class Integer {
public:
	Integer();
	Integer(int num);
	Integer(size_t size);
	Integer(std::set<size_t> s);
	~Integer();
	

	bool getDigit(size_t index) const;
	void setDigit(size_t index, bool value);
	size_t getAllocatedSize() const;
	bool posNthAce(size_t n, size_t& result) const;
	std::set<size_t> toIndeces() const;

	friend bool operator<(const Integer& lhs, const Integer& rhs);
	friend std::ostream& operator<<(std::ostream& out, const Integer &i);
private:	

	
	std::vector<bool> _digits;
};

