#include "regexp.hpp"
#include <vector>
#include <string>
#include "regexplexer.hpp"
#include "regexpparser.hpp"



std::ostream& operator<<(std::ostream& out, const Regexp& r)
{
	return r.print(out);
}



/**
 * The grammar this is supposed to parse.
 * 
 * 
 * <regexp>   ::= <fact> {| <regexp>} | <fact>
 * <fact>     ::= <pterm> <fact>   | <pterm>
 * <pterm>    ::= <starterm> + | <starterm>
 * <starterm> ::= <term> * | <term> 
 * <term>     ::= <char> | ( <regexp> ) | [<char_list>] | [^<char_list>]
 * <char_list>::= <char> <char_list> | <char> - <char> <char_list> | <char> 
 * 
 * 
 */
Regexp* result;
Regexp* Regexp::parse(std::string s)
{
	result = nullptr;
	char* inputbuf = strdup(s.c_str());
	YY_BUFFER_STATE buffer = yy_scan_string(inputbuf);
	yy_switch_to_buffer( buffer );
	if(yyparse() != 0){
		throw "fail to parse";
	}
	yy_delete_buffer(buffer);
	return result;
}

