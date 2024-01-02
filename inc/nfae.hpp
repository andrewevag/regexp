#pragma once
#include <vector>
#include <set>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <functional>
#include "dfa.hpp"

#define nfae_delta std::vector<std::vector<std::set<size_t> > >

/**
 * @brief I will probably make it available for all ascii characters
 * 
 */
class Nfae{
public:
	/**
	 * @brief Construct a new Nfae object
	 * 
	 * @param num_of_states 
	 * @param starting_state 
	 * @param final_states 
	 */
	Nfae(size_t num_of_states, size_t starting_state, std::initializer_list<size_t> final_states) 
	:_num_of_states(num_of_states), _starting_state(starting_state) {
		//256 for every ascii character and and + 1 for epsilon transitions.
		_delta = std::vector<std::vector<std::set<size_t> > >(num_of_states, std::vector<std::set<size_t> >(256+1, std::set<size_t>()));
		_final_states = std::set<size_t>(final_states);
	}

	~Nfae(){}
	
	/**
	 * @brief Get the Delta transition table
	 * @return nfae_delta* 
	 */
	nfae_delta* getDelta(){
		return &_delta;
	}

	/**
	 * @brief Get the Final States set.
	 * @return std::set<size_t>* 
	 */
	std::set<size_t>* getFinalStates() {
		return &_final_states;
	}

	/**
	 * @brief Get the Starting State 
	 * @return size_t 
	 */
	size_t getStartingState() {
		return _starting_state;
	}

	/**
	 * @brief Get the total number of states
	 * @return size_t 
	 */
	size_t getNumOfStates(){
		return _num_of_states;
	}

	/**
	 * @brief Set the Final States set using this cause you can do std::initializer_list<size_t>({1, 2, ...});
	 * or {1, 2, ...}
	 * @param final_states the initializer list to for the new final states
	 */
	void setFinalStates(std::initializer_list<size_t> final_states){
		_final_states = std::set<size_t>();
		_final_states.insert(final_states);
	}
	
	/**
	 * @brief Set the Starting State 
	 * @param s the new starting state
	 */
	void setStartingState(size_t s){
		_starting_state = s;
	}
	
	/**
	 * @brief Combine two Nfaes by just adding new states to the current nfa for every state
	 * in the the argument one. The starting or the final change do not change. Also fixes the number of states parameter
	 * @param nfa the nfa to draw the transitions from
	 * @return std::function<size_t(size_t)> a function for mapping the states of the before nfa
	 * to states in the new one
	 */
	std::function<size_t(size_t)> mergeAsDifferent(Nfae& nfa){
		//this has errors possibly because of the change during iteration
		for(auto& transition : *nfa.getDelta())
		{
			this->_delta.push_back(
				std::vector<std::set<size_t> > (transition.size(), std::set<size_t>())
			);
			for(size_t i = 0; i < transition.size(); i++){
				for(auto& elem : transition[i]){
					this->_delta.back()[i].insert(elem + this->_num_of_states);
				}
			}
		}
		
		size_t f = this->_num_of_states;
		std::function<size_t(size_t)> ret = [f](size_t from) -> size_t {
			return f + from;
		};
		this->_num_of_states += nfa.getNumOfStates();
		return ret;
	}

	/**
	 * @brief Adds a new state to the nfae with no transitions on it
	 * @return size_t the index of the final state
	 */
	size_t addState(){
		this->_delta.push_back(std::vector<std::set<size_t> > (256+1, std::set<size_t>()));
		this->_num_of_states++;
		return this->_num_of_states-1;
	}

	void addTransition(size_t from, size_t to, u_int16_t input){
		if(from >= this->_num_of_states || to >= this->_num_of_states || input > epsilon)
		{
			fprintf(stderr, "addTransition: you gave me\n from=%ld\nto=%ld\ninput=%u\nnumber_of_states=%ld",
			 from, to, input,this->_num_of_states);
			 exit(EXIT_FAILURE);
		}
		this->_delta[from][input].insert(to);
	}
	
	//-----------------------------------------------------------------
	//-----------------------------------------------------------------
	//----------------------Conversion to a DFA------------------------
	//-----------------------------------------------------------------
	//-----------------------------------------------------------------
	

	/**
	 * @brief Get the set of all states in the eClosure of a given state
	 * 
	 * @param s the input state
	 * @return std::set<size_t> 
	 */
	std::set<size_t> eClosure(size_t s);

	/**
	 * @brief Calculating the eClosure for a set of states
	 * 
	 * @param s the input set 
	 * @return std::set<size_t> 
	 */
	std::set<size_t> eClosure(std::set<size_t> s);

	/**
	 * @brief The set of all states where there is a transition for input from a state in 
	 * the input set
	 * 
	 * @param T the input set
	 * @param input the input character
	 * @return std::set<size_t> 
	 */
	std::set<size_t> move(std::set<size_t> T, unsigned char input);

	Dfa toDfa();
	
	//-----------------------------------------------------------------
	//-----------------------------------------------------------------
	//-----------------------------------------------------------------
	//-----------------------------------------------------------------
	//-----------------------------------------------------------------


	std::ostream& print(std::ostream& out) const{
		out << "Number of States : " << this->_num_of_states << "\n";
		out << "Starting State: " << this->_starting_state << "\n";
		out << "Final States: ";
		for(auto final_state : _final_states)
			out << final_state << " ";
		out << "\n";
		out << "Transitions: \n";
		for(size_t i = 0; i < this->_delta.size(); i++){
			for(size_t j = 0; j < this->_delta[i].size(); j++){
				for(auto elem: this->_delta[i][j])
					out << i << " ---'" << ((char)j) << "'--->" << elem << std::endl;
			}	
		}

		return out;
	}
	friend std::ostream& operator<<(std::ostream& out, const Nfae& nfa){
		return nfa.print(out);
	}


	static const size_t epsilon = 256;
private:
	//helper function
	void eClosure_h(size_t s, std::set<size_t> &seen);


	nfae_delta _delta;
	std::set<size_t> _final_states;
	
	size_t _num_of_states;
	size_t _starting_state;
};
