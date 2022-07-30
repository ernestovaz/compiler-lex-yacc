%{

#include "y.tab.h"
#include "SymbolTable.h"

int running  = 1;
int lineNumber = 1;
SymbolTable* symbolTable = NULL;
FILE* yyin;

int getLineNumber(void) {
  return lineNumber;
}

int isRunning(void) {
  return running;
}

void initMe(void) {
  symbolTable = createSymbolTable();
}
  
%}

%x COMMENT

%%

%{



%}


char                      { return KW_CHAR; } // reserved words
int                       { return KW_INT; }
float                     { return KW_FLOAT; }
if                        { return KW_IF; } 
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

[A-Za-z_]+                { insertSymbol(symbolTable, yytext, SymbolIdentifier); //identifiers
                            return TK_IDENTIFIER; } 

[0-9]+,[0-9]+             { insertSymbol(symbolTable, yytext, SymbolFloatLiteral); //literals
                            return LIT_FLOAT; } 
[0-9]+                    { insertSymbol(symbolTable, yytext, SymbolIntLiteral);
                            return LIT_INTEGER; } 
'([^\'\n\t]|\\'|\\n)'     { insertSymbol(symbolTable, yytext, SymbolCharLiteral); 
                            return LIT_CHAR; } 
\"([^\"\n\t]|\\\"|\\n)+\"  { insertSymbol(symbolTable, yytext, SymbolStringLiteral); 
                            return LIT_STRING; }

##.*                      {} // comment, ignore line

\\#                       { BEGIN COMMENT; }

<*>\n                     { lineNumber++; } 

[ \t]                     {} // whitespaces, do nothing

.                         { return TOKEN_ERROR; }

<<EOF>>                   { running = 0; return 0; }

<COMMENT>([^#\n]|#)       {} // ignore 
<COMMENT>#\\              { BEGIN 0; } // end comment mode 

%%

int yywrap(void) {
  printSymbolTable(symbolTable);
  return 1;
}

#include "y.tab.c"
#include "main.c"

