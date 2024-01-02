#include "dfa.hpp"


Dfa::Dfa(/* args */)
{
	//just creating the junk state @0
	_num_of_states = 1;
	_starting_state = 0;
	_final_states = {};
	_delta = std::vector<std::vector<size_t> >();
	_delta.push_back(std::vector<size_t>(256, 0));
}

Dfa::~Dfa()
{
}

size_t Dfa::addState()
{
	//by default i have all new states pointing to junk state
	_num_of_states++;
	_delta.push_back(std::vector<size_t>(256, 0));
	return _delta.size()-1;
}
void Dfa::updateTransition(size_t from, size_t to, unsigned char input)
{
	if(from >= _delta.size() || to >= _delta.size()){
		fprintf(stderr, "updateTransition: you gave from=%ld, to=%ld when number_of_states=%ld\n", 
		from, to, _delta.size());
		throw "updateTransition: Invalid Range";
	}
	_delta[from][input] = to;
}
bool Dfa::match(std::string s)
{
	size_t curr_state = _starting_state;
	for(auto input: s){
		curr_state = _delta[curr_state][input];
	}
	if(_final_states.find(curr_state) != _final_states.end()){
		return true;
	}else return false;
}


std::ostream& operator<<(std::ostream& out, const Dfa& dfa)
{
	std::cout << "Number of States : " << dfa.getNumberOfStates() << std::endl;
	std::cout << "Starting state : " << dfa.getStartingState() << std::endl;
	std::cout << "final States : ";
	for(auto x : dfa.getFinalStates()){
		std::cout << x << " ";
	}std::cout << std::endl;

	//write non junk states
	auto delta = dfa.getDelta();
	for(size_t i = 1; i < delta.size(); i++){
		for(size_t j = 0; j < delta[i].size(); j++){
			if(delta[i][j] != Dfa::junkState){
				std::cout << i << " ---'" << (char)j << "'--> " << delta[i][j] << std::endl; 
			}
		}
	}
	return out;

}