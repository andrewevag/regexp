#ifndef __REGEXPLEXER_HPP__
#define __REGEXPLEXER_HPP__


#include "CharList.hpp"
#include "regexp.hpp"
#include <cstring>


struct yy_buffer_state;
extern char* yytext;
extern int charcount;
extern Regexp* result;

int yylex();
void yyerror(const char* msg);
typedef struct yy_buffer_state * YY_BUFFER_STATE;
void yy_switch_to_buffer ( YY_BUFFER_STATE new_buffer );
YY_BUFFER_STATE yy_scan_string(const char * str);
void yy_delete_buffer(YY_BUFFER_STATE buffer);


uint8_t getCharFromString(char* s);




#endif

