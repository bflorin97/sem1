%{
#include "main.h"
#include <stdio.h>
%}

%%
"class"								return CLASS;

"void"		|
"int"			|
"char"		|
"double"	|
"float"								return TYPE;

"public"		|
"private"		|
"protected"						return ACCESS_MODIFIER;

"const"								return CONSTANT;

[a-zA-Z][_a-zA-Z0-9]*	return NAME;

"{"										return OPEN_BRACKET;

"}"										return CLOSE_BRACKET;

"("										return OPEN_ROUND_BRACKET;

")"										return CLOSE_ROUND_BRACKET;

";"										return SEMICOLON;

","										return COMMA;

"~"										return DESTRUCTOR;

"*"										return ASTERISK;

"&"										return AMPERSAND;

"/*"[a-zA-Z0-9 \n]*"*/"	;

"//"[a-zA-Z0-9 \n]*		;

[ \n\t:]+							;

.											printf("Invalid character\n");
%%

int yywrap() {
	return 1;
}
