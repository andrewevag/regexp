%{
#include "regexplexer.hpp"
#include <cstdio>
#include <cstdlib>
#include "regexpparser.hpp"

int charcount = 0;
%}

%option noyywrap
%option noinput nounput


%%

[\*\(\)\[\]\-\^\|\+] { charcount++; return yytext[0]; }
\\. 		   { charcount+=2; yylval.c = getCharFromString ( yytext ); return T_char; }
. 			   { charcount++; yylval.c = getCharFromString (yytext); return T_char; }
\n			   { yyerror("Lexing Error Didn't not expect a new line!"); }
%%

void yyerror(const char* msg)
{
	fprintf(stderr, "Error: %s@%d\n", msg, charcount);
	exit(EXIT_FAILURE);
}
uint8_t getCharFromString(char* s){
	uint8_t c;
	if(s[0] == '\\'){
		if(strlen(s) < 2){
			fprintf(stderr, "getCharFromString: got \\ but no character after that!\n");
			exit(EXIT_FAILURE);
		}
		switch(s[1]) {
			case '\\': 
				c = '\\';
				break;
			case 'n': 
				c = '\n'; 
				break; 
			case 't':
				c = '\t';
				break; 
			case 'r' :
				c = '\r';
				break;
			default:
				c=s[1]; 
		}
		return c;
	}else{
		return (uint8_t)s[0];
	}
}
