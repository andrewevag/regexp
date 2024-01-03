#pragma once
#include <iostream>
#include "nfae.hpp"
#include <list>

class Regexp{
public:
	Regexp(){}
	virtual ~Regexp() = default;
	virtual std::ostream& print(std::ostream& out) const = 0;
	static Regexp* parse(std::string s);
	virtual Nfae toNfae() const = 0;
	virtual Regexp* copy() const = 0;
private:
};

std::ostream& operator<<(std::ostream& out, const Regexp& r);

class RegexpChar : public Regexp{
public:
	RegexpChar(char c) :_c(c){}
	~RegexpChar(){}
	virtual std::ostream& print(std::ostream& out) const override {
		if(this->_c == '\n'){
			out << "\\n";
		}
		else if(this->_c == '\t'){
			out << "\\t";
		}
		else if(this->_c == '\r'){
			out << "\\r";
		}
		else if(this->_c == '+' || this->_c == '.' || this->_c == '*' || this->_c == '(' || this->_c == ')' || this->_c == ' '){
			out << "\\" << this->_c;
		}
		else{
			out << this->_c;
		}
		return out;
	}
	Nfae toNfae() const  override {
		//single character NFA
		// >0 ---_c---> (1)
		Nfae e(2, 0, {1});
		e.addTransition(0, 1, this->_c);
		return e;
	}

	Regexp* copy() const override {
		return new RegexpChar(this->_c);
	}

private:
	char _c;
};

class RegexpPlus : public Regexp{
public:
	RegexpPlus(Regexp* left, Regexp* right) : _left(left), _right(right) {}
	~RegexpPlus(){
		delete this->_left;
		delete this->_right;
	}
	std::ostream& print(std::ostream& out) const override {
		out << "(";
		_left->print(out);
		out << "|";
		_right->print(out);
		out << ")";
		return out;
	}
	Nfae toNfae() const override {
		//plus Nfa 
		//>some 			---epsilon-->  starting_of_first
		//>some 			---epsilon-->  starting_of_second
		//final_of_first 	---epsilon-->  new_final
		//final_of_second 	---epsilon-->  new_final
		//so this needs to new states and merging of the two 
		
		Nfae leftnfae = _left->toNfae();
		Nfae rightnfae = _right->toNfae();

		auto mapper = leftnfae.mergeAsDifferent(rightnfae);
		size_t new_start = leftnfae.addState(), new_end = leftnfae.addState();
		
		//new epsilons from the starting one
		leftnfae.addTransition(new_start, leftnfae.getStartingState(), Nfae::epsilon);
		leftnfae.addTransition(new_start, mapper(rightnfae.getStartingState()), Nfae::epsilon);

		//new epsilons from the finals if done correctly this iteration will only be once
		for(auto left_final : *leftnfae.getFinalStates()){
			leftnfae.addTransition(left_final, new_end, Nfae::epsilon);
		}
		for(auto right_final : *rightnfae.getFinalStates()){
			leftnfae.addTransition(mapper(right_final), new_end, Nfae::epsilon);
		}

		//fix start and final states.
		leftnfae.setStartingState(new_start);
		leftnfae.setFinalStates({new_end});

		return leftnfae;
	};

	Regexp* copy() const override {
		return new RegexpPlus(this->_left->copy(), this->_right->copy());
	}
private:
	Regexp* _left, *_right;
};

class RegexpMult : public Regexp{
public:
	RegexpMult(Regexp* left, Regexp* right) : _left(left), _right(right) {}
	~RegexpMult(){
		delete this->_left;
		delete this->_right;
	}
	std::ostream& print(std::ostream& out) const override {
		out << "(" ;
		_left->print(out);
		out << ".";
		_right->print(out);
		out << ")";
		return out;
	}
	Nfae toNfae() const override {
		Nfae leftnfa = _left->toNfae(), rightnfa = _right->toNfae();

		auto mapper = leftnfa.mergeAsDifferent(rightnfa);
		//new_start ---epsilon---> left_start
		//left_end  ---epsilon---> right_start
		//right_end ---epsilon---> new_end
		size_t new_start = leftnfa.addState(), new_end = leftnfa.addState();

		leftnfa.addTransition(new_start, leftnfa.getStartingState(), Nfae::epsilon);
		for(auto final_state : *leftnfa.getFinalStates()){
			leftnfa.addTransition(final_state, mapper(rightnfa.getStartingState()), Nfae::epsilon);
		}
		for(auto final_state : *rightnfa.getFinalStates()){
			leftnfa.addTransition(mapper(final_state), new_end, Nfae::epsilon);
		}

		//fix start and end
		leftnfa.setStartingState(new_start);
		leftnfa.setFinalStates({new_end});
		return leftnfa;
	}

	Regexp* copy() const override {
		return new RegexpMult(this->_left->copy(), this->_right->copy());
	}

private:
	Regexp* _left, *_right;
};

class RegexpStar : public Regexp{
public:
	RegexpStar(Regexp* child) : _child(child){}
	~RegexpStar(){
		delete this->_child;
	}
	std::ostream& print(std::ostream& out) const override {
		out << "(";
		_child->print(out);
		out << ")*";
		return out;
	}
	
	Nfae toNfae() const override {
		/*
		 * new_start ---epsilon---> old_start
		 * new_start ---epsilon---> new_end
		 * old_end   ---epsilon---> old_start
		 * old_end   ---epsilon---> new_end
		 */
		Nfae nfa = _child->toNfae();
		size_t new_start = nfa.addState(), new_end = nfa.addState();

		nfa.addTransition(new_start, nfa.getStartingState(), Nfae::epsilon);
		nfa.addTransition(new_start, new_end, Nfae::epsilon);
		for(auto final_state : *nfa.getFinalStates()){
			nfa.addTransition(final_state, nfa.getStartingState(), Nfae::epsilon);
			nfa.addTransition(final_state, new_end, Nfae::epsilon);
		}
		//fix starting state ending state
		nfa.setStartingState(new_start);
		nfa.setFinalStates({new_end});
		return nfa;
	}

	Regexp* copy() const override {
		return new RegexpStar(this->_child->copy());
	}
private: 
	Regexp* _child;
};

class RegexpRange : public Regexp {
public:
	RegexpRange(bool inverse, std::set<uint8_t> vchar) {
		if(!inverse){
			this->_vchar = vchar;
		}else{
			for(size_t i = 0; i < 256; i++){
				if(vchar.find(i) == vchar.end()){
					this->_vchar.insert(i);
				}
			}
		}
	}
	RegexpRange(bool inverse, std::list<uint8_t> vchar2) {
		std::set<uint8_t> vchar(vchar2.begin(), vchar2.end());
		if(!inverse){
			this->_vchar = vchar;
			
		}else{
			for(size_t i = 0; i < 256; i++){
				if(vchar.find(i) == vchar.end()){
					this->_vchar.insert(i);
				}
			}
		}
	}
	~RegexpRange(){	}

	virtual std::ostream& print(std::ostream& out) const override {
		out << "[";
		for(auto elem : this->_vchar){
			if(elem == '\n'){
			out << "\\n";
			}
			else if(elem == '\t'){
				out << "\\t";
			}
			else if(elem == '\r'){
				out << "\\r";
			}
			else if(elem == '+' || elem == '.' || elem == '*' || elem == '(' || elem == ')' || elem == ' '){
				out << "\\" << elem;
			}
			else{
				out << elem;
			}
		}
		out << "]";
		return out;
	};
	Nfae toNfae() const override {
		Nfae e(2, 0, {1});
		for(auto elem : this->_vchar){
			e.addTransition(0, 1, elem);
		}
		return e;
	};

	Regexp* copy() const override { 
		return new RegexpRange(false, this->_vchar);
	}

private:
	std::set<uint8_t> _vchar;
};

Regexp* parse(std::string s);
