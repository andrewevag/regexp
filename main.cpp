#include <iostream>
#include "regexp.hpp"

int main(){
	
	Regexp* r = Regexp::parse("[a-zA-Z]+");
	std::cout << *r << std::endl;
	Dfa d = r->toNfae().toDfa();
	
	std::cout << d.match("hellopeople") << std::endl;
	std::cout << d.match("hello123people") << std::endl;
	std::cout << d.match("hello\npeople") << std::endl;
	
	
	delete r;
	return 0;
	
}
