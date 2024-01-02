%{
#include "regexplexer.hpp"
#include <cstdio>

%}

%union {
	Regexp *r;
	CharList *vchar;
	uint8_t c;
}

%type<r> regexp fact starterm term pterm
%type<vchar> char_list
%token<c> T_char
%%

regexp:
	regexp '|' fact 		{ $$ = new RegexpPlus($1, $3); result = $$;}
|   fact            		{ $$ = $1; result = $$; }
;
fact:
	pterm fact   		{ $$ = new RegexpMult($1, $2); }
|   pterm				{ $$ = $1; }
;

pterm:
	starterm '+'		{ $$ = new RegexpMult( $1, new RegexpStar($1->copy()) ); }
|	starterm			{ $$ = $1; }
;

starterm: 
	term '*'				{ $$ = new RegexpStar($1); }
| 	term					{ $$ = $1; }
;

term:
	T_char 					{ $$ = new RegexpChar( getCharFromString(yytext) );}
| 	'(' regexp ')' 			{ $$ = $2; }
| 	'[' '^' char_list ']' 	{ $$ = new RegexpRange(true, ($3->getList()));  }
|   '[' char_list ']'		{ $$ = new RegexpRange(false, ($2->getList())); }
;

char_list:
	T_char '-' char_list	{ for(size_t i = $3->getList().front()-1; i >= (size_t) $1 ; i--){ $3->push_front((uint8_t)i);} $$ = $3;}
|	T_char char_list		{ $2->push_front($1); $$ = $2; }
|	T_char					{ $$ = new CharList({$1}); }
;

%%