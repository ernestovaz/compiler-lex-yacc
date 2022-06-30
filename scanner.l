%{

#include "tokens.h"
#include "HashTable.h"

int running  = 1;
int lineNumber = 1;
HashTable* hashTable = NULL;

int getLineNumber(void) {
  return lineNumber;
}

int isRunning(void) {
  return running;
}
  
%}

%x COMMENT

%%

%{
if(hashTable == NULL) hashTable = createTable();



%}

if                        { return KW_IF; } // reserved words
else                      { return KW_ELSE; }
while                     { return KW_WHILE; }
read                      { return KW_READ; }
print                     { return KW_PRINT; }
return                    { return KW_RETURN; }

[;()\[\]{}+\-./<>&|~]     { return yytext[0]; } //special characters


\<-                       { return ASSIGNMENT; } //compound operators
\<=                       { return OPERATOR_LE; }
>=                        { return OPERATOR_GE; }
==                        { return OPERATOR_EQ; }
!=                        { return OPERATOR_DIF; }

[A-Za-z_]+                { insert(hashTable, yytext, yytext);
                              return TK_IDENTIFIER; } //identifiers

[0-9]+,[0-9]+             { return LIT_FLOAT; } //literals 
[0-9]+                    { return LIT_INTEGER; } 
'([^\'\n\t]|\\'|\\n)'     { return LIT_CHAR; } 
\"([^\"\n\t]|\\\"|\\n)\"  { return LIT_STRING; }

##.*                      {} // comment, ignore line

\\#                       { BEGIN COMMENT; }

<*>\n                     { lineNumber++; } 

[ \t]                     {} // whitespaces, do nothing

.                         { return TOKEN_ERROR; }

<<EOF>>                   { running = 0; return 0; }

<COMMENT>([^#\n]|#)       {} // ignore 
<COMMENT>#\/              { BEGIN 0; } // ignore 

%%

int yywrap(void) {
  printTable(hashTable);
  return 1;
}

#include "main.c"

