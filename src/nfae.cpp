#include "nfae.hpp"
#include "Integer.hpp"
#include <map>

std::set<size_t> Nfae::eClosure(size_t s)
{
	std::set<size_t> ret = std::set<size_t>();
	eClosure_h(s, ret);
	return ret;
}

std::set<size_t> Nfae::eClosure(std::set<size_t> s)
{
	std::set<size_t> ret = std::set<size_t>();
	for(auto state: s){
		eClosure_h(state, ret);
	}
	return ret;
}

void Nfae::eClosure_h(size_t s, std::set<size_t> &seen)
{
	//this algorithm is
	if(seen.find(s) == seen.end()){
		//this is if not found
		seen.insert(s);
		for(auto state : this->_delta[s][Nfae::epsilon]){
			eClosure_h(state, seen);
		}
	}
}


std::set<size_t> Nfae::move(std::set<size_t> T, unsigned char input)
{
	std::set<size_t> ret = std::set<size_t>();
	for(auto state : T)
	{
		for(auto to : this->_delta[state][input]){
			ret.insert(to);
		}
	}
	return ret;
}

std::pair<bool, Integer> available(std::set<Integer>& Dstates, std::set<Integer>& seen){
	for(auto& x: Dstates){
		if(seen.find(x) == seen.end()){
			return std::pair<bool, Integer> (true, x);
		}
	}
	return std::pair<bool, Integer>(false, *Dstates.begin());
}

struct tOfIntegerUint8{
	bool operator() (const std::pair<Integer, uint8_t>& lhs, const std::pair<Integer, uint8_t>& rhs){
		return (!(lhs.first < rhs.first) && !(rhs.first < lhs.first)) ? lhs.second < rhs.second :
		lhs.first < rhs.first;
	}
};
Dfa Nfae::toDfa()
{
	/*
	 * Nfae --> Dfa
	 * at the end of the while Dtran has 
	 * Integer(subset of states), uint8_t  --> Integer(subset of states)
	 * 
	 * for every key we add a new state so we need a 
	 * map(Integer --> size_t) to do the subset to dfa states
	 * 
	 */


	//The definitions for using Dtran to
	//Integer, uint8_t ---> Integer
	std::map<std::pair<Integer, uint8_t>, Integer, tOfIntegerUint8> Dtran;
	std::set<Integer> Dstates = std::set<Integer>();
	std::set<Integer> seen = std::set<Integer>();


	Dstates.insert(Integer(eClosure(this->_starting_state)));
	std::pair<bool, Integer> res = available(Dstates, seen);
	while(res.first){
		Integer T = res.second;
		seen.insert(T);
		
		for(size_t input = 0; input < Nfae::epsilon; input++)
		{
			std::set<size_t> U = eClosure(move(T.toIndeces(), input));
			if(seen.find(Integer(U)) == seen.end()){
				Dstates.insert(Integer(U));
			}
			if(U.size() != 0)
				Dtran.insert(std::pair<std::pair<Integer, uint8_t>, Integer>(std::pair<Integer, uint8_t>(T, input), Integer(U)));
		}
		
		res = available(Dstates, seen);
	}

	// for(auto& pair : Dtran){
	// 	std::cout << pair.first.first << (char)(pair.first.second) << " --> " << pair.second << std::endl;
	// }


	//Here begins the transition from Nfae to Dfa class
	std::map<Integer, size_t> subsetToDfaState;
	Dfa dfa;
	size_t new_state;
	for(auto &pair : Dtran){
		if(subsetToDfaState.find(pair.first.first) == subsetToDfaState.end()){
			//new state
			new_state = dfa.addState();
			subsetToDfaState.insert(std::pair<Integer, size_t>(pair.first.first, new_state));
		}
		if(subsetToDfaState.find(pair.second) == subsetToDfaState.end()){
			//new state
			new_state = dfa.addState();
			subsetToDfaState.insert(std::pair<Integer, size_t>(pair.second, new_state));
		}

		dfa.updateTransition(
			subsetToDfaState[pair.first.first],
			subsetToDfaState[pair.second], 
			pair.first.second
		);
		

	}
	// std::cout << "FINAL STATES ARE: ";
	// for(auto final_state : _final_states){
	// 	std::cout << final_state << " ";
	// }std::cout << std::endl;
	dfa.setStartingState(subsetToDfaState[Integer(eClosure(_starting_state))]);
	std::set<size_t> dfa_final_states;
	for(auto pair : subsetToDfaState){
		for(auto final_state : _final_states){
			// std::cout << "got in here" << std::endl;
			// std::cout << pair.first << std::endl;
			if(pair.first.getDigit(final_state)){
				//it is a final state
				// std::cout << "final_state" << std::endl;
				dfa_final_states.insert(subsetToDfaState[pair.first]);
			}
		}
	}
	dfa.setFinalStates(dfa_final_states);
	// for(auto pair : subsetToDfaState){
	// 	std::cout << pair.first << " -> " << pair.second << std::endl;
	// }
	return dfa;
}


/*
 * the algorithm is
 * Dstates = {}
 * seen = {}
 * Dstates.add(eClosure(starting_state));
 * while(T in Dstates with T not in seen){
 * 	seen.add(T);
 * 	for(auto input : input_symbols){
 * 		U = e_closure(move(T,input))
 * 		if(U not in Dstates)
 * 			Dstates.add(U);
 * 		Dtran[T,input] = U;
 * 	}
 * }
 *
 * maybe they need to be hashed so that they can be identified
 * you could do the 2^i bit set to represent each state
 * Dtran :: Intinf, char -> Intinf
 *
 * either i need a way to key the sets by using very large numbers
 * or bitsets as vector<bool> ... let's go with the bitset one
 *
 *
 */