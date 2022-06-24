%{

#include "tokens.h"

int g_running = 1;
  
%}

%%

if                { return KW_IF; }
else              { return KW_ELSE; }
while             { return KW_WHILE; }
read              { return KW_READ; }
print             { return KW_PRINT; }
return            { return KW_RETURN; }

%%

int yywrap(void) {
  
  return 1;
}


int main(void) {
  while(g_running) {
    int code = yylex();
    fprintf(stderr, "%d", code);
  }
    
  return 0;
}
