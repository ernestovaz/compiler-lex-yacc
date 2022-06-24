%{

#include "tokens.h"

int g_running = 1;
  
%}

%%

// reserved words
if                      { return KW_IF; }
else                    { return KW_ELSE; }
while                   { return KW_WHILE; }
read                    { return KW_READ; }
print                   { return KW_PRINT; }
return                  { return KW_RETURN; }

// special characters
[;()\[\]{}+\-./<>&|~]   { return yytext[0]; }

// compound operators
\<-                     { return ASSIGNMENT; }
\<=                     { return OPERATOR_LE; }
>=                      { return OPERATOR_GE; }
==                      { return OPERATOR_EQ; }
!=                      { return OPERATOR_DIF; }

// identifiers
[A-z_]*                 { return TK_IDENTIFIER; }

// whitespaces 
[ \t]                   {}    // do nothing

// unexpected characters
.                       { return TOKEN_ERROR; }

// end of file
<<EOF>>                 { g_running = 0;  return 0; }

%%

int yywrap(void) {
  return 1;
}

#include "main.c"

