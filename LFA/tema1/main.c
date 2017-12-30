#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FUNCTIONS 100
#define MAX_ARGUMENTS 100

extern int yylex();
extern int yylineno;
extern char* yytext;

typedef struct {
  char* type;
  char* name;
  int argc;
  char** argv;
} function;

typedef struct {
  char* name;
  int num_functions;
  int num_constructors;
  int destructor;
  int copy_constructor;

  function* functions;
  function* constructors;
} class;

char* decrypt(int token) {
  switch (token) {
    case CLASS: return "CLASS";
    case TYPE: return "TYPE";
    case ACCESS_MODIFIER: return "ACCESS_MODIFIER";
    case NAME: return "NAME";
    case OPEN_BRACKET: return "OPEN_BRACKET";
    case CLOSE_BRACKET: return "CLOSE_BRACKET";
    case OPEN_ROUND_BRACKET: return "OPEN_ROUND_BRACKET";
    case CLOSE_ROUND_BRACKET: return "CLOSE_ROUND_BRACKET";
    case SEMICOLON: return "SEMICOLON";
    default: return "Error decrypting string\n";
  }
}

void error(char* function_name, char espected) {
  printf("Syntax error: espected '%c' but found '%s' in function %s\n",
    espected,
    yytext,
    function_name);
}

int main(int argc, char** argv) {
  int token;
  class my_class;

  while((token = yylex())) {
    switch (token) {
      case CLASS: {
        token = yylex();
        my_class.name = strdup(yytext);
        my_class.functions = malloc(sizeof(function) * MAX_FUNCTIONS);
        my_class.constructors = malloc(sizeof(function) * MAX_FUNCTIONS);
        my_class.num_functions = 0;
        my_class.num_constructors = 0;
        my_class.copy_constructor = -1;
        my_class.destructor = 0;

        token = yylex();
        if (token != OPEN_BRACKET) {
          error("CLASS", '{');
        }
        break;
      }
      case TYPE: {
        function new_function;
        new_function.type = strdup(yytext);
        token = yylex();
        new_function.name = strdup(yytext);
        token = yylex();
        if (token == OPEN_ROUND_BRACKET) {
          new_function.argc = 0;
          new_function.argv = malloc(sizeof(char*) * MAX_ARGUMENTS);
          while ((token = yylex()) != CLOSE_ROUND_BRACKET) {
            new_function.argv[new_function.argc++] = strdup(yytext);
            token = yylex();
            if (token != NAME) {
              if (token == CLOSE_ROUND_BRACKET) {
                break;
              } else {
                continue;
              }
            }
            token = yylex();
            if (token == CLOSE_ROUND_BRACKET) {
              break;
            }
          }
          my_class.functions[my_class.num_functions++] = new_function;
        }
        break;
      }
      case NAME: {
        function new_function;
        new_function.name = strdup(yytext);
        token = yylex();
        if (token == OPEN_ROUND_BRACKET) {
          new_function.argc = 0;
          new_function.argv = malloc(sizeof(char*) * MAX_ARGUMENTS);
          while ((token = yylex()) != CLOSE_ROUND_BRACKET) {
            if (token == CONSTANT) {
              new_function.argv[new_function.argc] = strdup("const ");
              token = yylex();
              if (!strcmp(yytext, my_class.name)) {
                my_class.copy_constructor = my_class.num_constructors;
              }
              strcat(new_function.argv[new_function.argc++], yytext);
            }
            else {
              new_function.argv[new_function.argc++] = strdup(yytext);
            }

            token = yylex();
            if (token == ASTERISK || token == AMPERSAND) {
              strcat(new_function.argv[new_function.argc - 1], yytext);
              token = yylex();
            }
            if (token != NAME) {
              if (token == CLOSE_ROUND_BRACKET) {
                break;
              } else {
                continue;
              }
            }
            token = yylex();
            if (token == CLOSE_ROUND_BRACKET) {
              break;
            }
          }
          my_class.constructors[my_class.num_constructors++] = new_function;
        } else {
          error("NAME", '(');
        }
        break;
      }
      case DESTRUCTOR: { /*~*/
        token = yylex(); /*NAME*/
        token = yylex(); /*(*/
        token = yylex(); /*)*/
        token = yylex(); /*;*/
        my_class.destructor = 1;
        break;
      }
    }
  }

  printf("\n\nclass %s\n", my_class.name);
  printf("-constructors: %d\n", my_class.num_constructors -
      (my_class.num_constructors == 0 ? 0 :
        my_class.copy_constructor == -1 ? 0 : 1));
  for (int i = 0; i < my_class.num_constructors; i++) {
    if (i == my_class.copy_constructor) {
      continue;
    }
    printf("\t%s(", my_class.constructors[i].name);
    for (int j = 0; j < my_class.constructors[i].argc; j++) {
      printf("%s", my_class.constructors[i].argv[j]);
      if (j + 1 < my_class.constructors[i].argc) {
        printf(", ");
      }
    }
    printf(");\n");
  }
  printf("-copy constructor: %d\n", my_class.copy_constructor == -1 ? 0 : 1);
  if (my_class.copy_constructor != -1) {
    int i = my_class.copy_constructor;
    printf("\t%s(", my_class.constructors[i].name);
    for (int j = 0; j < my_class.constructors[i].argc; j++) {
      printf("%s", my_class.constructors[i].argv[j]);
      if (j + 1 < my_class.constructors[i].argc) {
        printf(", ");
      }
    }
    printf(");\n");
  }
  printf("-destructor: %d\n", my_class.destructor);
  if (my_class.destructor == 1) {
    printf("\t~%s();\n", my_class.name);
  }
  printf("-functions: %d\n", my_class.num_functions);
  for (int i = 0; i < my_class.num_functions; i++) {
    printf("\t%s %s(", my_class.functions[i].type, my_class.functions[i].name);
    for (int j = 0; j < my_class.functions[i].argc; j++) {
      printf("%s", my_class.functions[i].argv[j]);
      if (j + 1 < my_class.functions[i].argc) {
        printf(", ");
      }
    }
    printf(");\n");
  }


  return 0;
}
