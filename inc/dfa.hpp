#pragma once
#include <iostream>
#include <set>
#include <vector>

//if it is implemented by a 2d table it needs to have a junk state
//like 0 so the

class Dfa
{
public:
	/**
	 * @brief Construct a new Dfa object
	 * Creates a junk state with every character pointing to that state
	 */
	Dfa();
	/**
	 * @brief Destroy the Dfa object
	 * 
	 */
	~Dfa();

	//Getters
	size_t getNumberOfStates() const { return this->_num_of_states;}
	size_t getStartingState() const { return this->_starting_state; }
	std::set<size_t> getFinalStates() const { return this->_final_states; }
	std::vector<std::vector<size_t> > getDelta() const{ return this->_delta; };
	
	//Setters
	void setStartingState(size_t starting_state) { this->_starting_state = starting_state; }
	void setFinalStates(std::set<size_t> final_states) { this->_final_states = final_states; }

	/**
	 * @brief Adds a new state that for every input points to junkState
	 * 
	 * @return size_t the state for future reference
	 */
	size_t addState();

	/**
	 * @brief Update a transition in the dfas delta function
	 * 
	 * @param from from state
	 * @param to to state
	 * @param input input leading to that state
	 */
	void updateTransition(size_t from, size_t to, unsigned char input);

	/**
	 * @brief Do the calculation and accept or reject a string
	 * 
	 * @param s the input string
	 * @return bool true -> accepted, false -> rejected
	 */
	bool match(std::string s);


	friend std::ostream& operator<<(std::ostream& out, const Dfa& dfa);

	static const size_t junkState = 0;
private:
	size_t _num_of_states;
	size_t _starting_state;
	std::set<size_t> _final_states;
	std::vector<std::vector<size_t> > _delta;
	

};

