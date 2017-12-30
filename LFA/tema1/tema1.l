
%{
#include <stdio.h>
#include <string.h>

typedef struct {
	char* name;
	char* args;
} function;

typedef struct {
	char* name;
	
	function* functions;
	function* constructors;
	function* destructors;
	
	int pos_functions;	
	int pos_constructors;
	int pos_destructors;
} class;

int num_functions = 100;
int num_constructors = 100;
int num_destructors = 100;
int num_classes = 100;

class* classes;
int pos_classes = 0;
%}

name ([a-z]|[A-Z]|[0-9]|_)+{spaces}
type (void|char|int|double|float){spaces}
bracket \(({spaces}(const)?{spaces}{type}{spaces}[\*&]?{name}{spaces}|,|{spaces}{type}{spaces})*\)
class class{spaces}
spaces [ ]*
access (public|private|protected|default):

%%
{class} {
	
	classes[pos_classes].pos_destructors = 0;
	classes[pos_classes].pos_constructors = 0;
	classes[pos_classes].pos_functions = 0;
	
	classes[pos_classes].functions = malloc(sizeof(function) * num_functions);
	classes[pos_classes].constructors = malloc(sizeof(function) * num_constructors);
	classes[pos_classes].destructors = malloc(sizeof(function) * num_destructors);
	
	register int c;
	int poz = 0, size = 20;
	char* word = malloc(sizeof(char) * 20);

  while ( (c = input()) != ' ' && c != EOF ) {
  	word[poz++] = c;
  	if (poz == size) {
  		size += 10;
  		word = realloc(word, sizeof(char) * size);
  	}
  }
	
	classes[pos_classes].name = strdup(word);
}

{type}{name}{bracket}; {
	char* p = NULL, *line = NULL;
	
	line = strdup(yytext);
	p = strtok(line, "()");
	
	classes[pos_classes].functions[classes[pos_classes].pos_functions].name = strdup(p);
	p = strtok(NULL, "()");
	
	classes[pos_classes].functions[classes[pos_classes].pos_functions].args = strdup(p);
	classes[pos_classes].pos_functions++;
}

~{name}{bracket}; {
	char* p = NULL, *line = NULL;
	line = strdup(yytext);
	p = strtok(line, "()");
	
	classes[pos_classes].destructors[classes[pos_classes].pos_destructors].name = strdup(p);
	p = strtok(NULL, "()");
	
	if (strchr(p, ';') >= 0) {
		classes[pos_classes].destructors[classes[pos_classes].pos_destructors].args = strdup("");
	} else {
		classes[pos_classes].destructors[classes[pos_classes].pos_destructors].args = strdup(p);
	}
	classes[pos_classes].pos_destructors++;
	
	free(line);
}

{name}{bracket}; {
	char* p = NULL, *line = NULL;
	line = strdup(yytext);
	p = strtok(line, ")(");
	
	classes[pos_classes].constructors[classes[pos_classes].pos_constructors].name = strdup(p);
	p = strtok(NULL, ")(");
	
	if (strchr(p, ';') >= 0) {
		classes[pos_classes].constructors[classes[pos_classes].pos_constructors].args = strdup("");
	} else {
		classes[pos_classes].constructors[classes[pos_classes].pos_constructors].args = strdup(p);
	}
	classes[pos_classes].pos_constructors++;
	
	free(line);
}

"}" {
	pos_classes++;
}

"//" {
	register int c;
  while ( (c = input()) != '\n' );
}

"/*" {
	register int c;
    while ( (c = input()) != '*' && c != EOF );
      	/* eat up text of comment */

		if ( c == '*' ) {
			while ( (c = input()) == '*' );
        if ( c == '/' )
          break;    /* found the end */
    }

    if ( c == EOF ) {
      printf( "EMF in comment" );
      break;
    }
}
%%

void main() {
	classes = malloc(sizeof(class) * num_classes);
	
	yylex();
	for (int i = 0; i < pos_classes; i++) {
		printf("class %s\n", classes[i].name);
		printf("-constructori: %d\n", classes[i].pos_constructors);
		
		for (int j = 0; j < classes[i].pos_constructors; j++) {
			printf("\t%s(%s);\n", classes[i].constructors[j].name, classes[i].constructors[j].args);
		}
		
		printf("-destructor: %d\n", classes[i].pos_destructors);
		for (int j = 0; j < classes[i].pos_destructors; j++) {
			printf("\t%s(%s);\n", classes[i].destructors[j].name, classes[i].destructors[j].args);
		}
		
		printf("-metode: %d\n", classes[i].pos_functions);
		for (int j = 0; j < classes[i].pos_functions; j++) {
			printf("\t%s(%s);\n", classes[i].functions[j].name, classes[i].functions[j].args);
		}
		printf("\n");
	}
}

/*
{class}{name}\{ {
	printf("CLASS START[%s]", yytext);
	num--;
	if (num == 0) yyless(0);
}
{type}{name}{bracket}; printf("FUNCTION");
{name}{bracket}; printf("CONSTRUCTOR");
~{name}{bracket}; printf("DESTRUCTOR");
{type}({name}|{spaces}|,|\*)+; printf("DECLARATION");
{bracket} printf("BRACKET ");
{access} printf("ACCESS ");
\} printf("CLASS END");
{type} printf("TYPE ");
{name} printf("NAME ");
*?

/*
class[ ]+{name}\{ prints("CLASS DEFINITION START");
\/\/.* printf("DESCRIPTION");
\} printf("CLASS DEFINITION END");

{type}[ ]+{name}[ ]*bracket; printf("FUNCTION");
 {name}+[ ]*\({name}*\); printf("CONSTRUCTOR");
~{name}+[ ]*\({name}*\); printf("DESTRUCTOR");
*/
