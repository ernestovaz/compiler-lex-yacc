%{

#include "tokens.h"

int g_running = 1;
  
%}

%%

if                      { return KW_IF; } // reserved words
else                    { return KW_ELSE; }
while                   { return KW_WHILE; }
read                    { return KW_READ; }
print                   { return KW_PRINT; }
return                  { return KW_RETURN; }

[;()\[\]{}+\-./<>&|~]   { return yytext[0]; } //special characters

\<-                     { return ASSIGNMENT; } //compound operators
\<=                     { return OPERATOR_LE; }
>=                      { return OPERATOR_GE; }
==                      { return OPERATOR_EQ; }
!=                      { return OPERATOR_DIF; }

[A-z_]+                 { return TK_IDENTIFIER; } //identifiers

[ \t]                   {}    // whitespaces, do nothing

.                       { return TOKEN_ERROR; }

<<EOF>>                 { g_running = 0;  return 0; }

%%

int yywrap(void) {
  return 1;
}

#include "main.c"

