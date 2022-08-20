#define MAX_ASCII_CHAR_CODE 127

#include "y.tab.c"

#include "SymbolTable.h"
#include "AbstractSyntaxTree.h"

#include "Decompilation.h"

#include <stdio.h>

int yylex();
extern char* yytext;
extern int lineCount;
extern SymbolTable* symbolTable; 
extern FILE* yyin;

void initMe(void);
int getLineNumber(void);
int isRunning(void);

char* get_token_name(int char_code){
  switch(char_code){
    case 256:
      return "KW_CHAR";
    case 257:
      return "KW_INT";
    case 258:
      return "KW_FLOAT";
    case 261:
      return "KW_IF";
    case 263:
      return "KW_ELSE";
    case 264:
      return "KW_WHILE";
    case 267:
      return "KW_READ";
    case 268:
      return "KW_PRINT";
    case 269:
      return "ASSIGNMENT";
    case 270:
      return "OPERATOR_LE";
    case 271:
      return "OPERATOR_GE";
    case 272:
      return "OPERATOR_EQ";
    case 273:
      return "OPERATOR_DIF";
    case 280:
      return "TK_IDENTIFIER";
    case 281:
      return "LIT_INTEGER";
    case 282:
      return "LIT_FLOAT";
    case 285:
      return "LIT_CHAR";
    case 286:
      return "LIT_STRING";
    case 290:
      return "TOKEN_ERROR";
  }

  return "FAULTY_TOKEN_CODE";
}

void pretty_print(int char_code){
  if(char_code > MAX_ASCII_CHAR_CODE) {
    char* token_name = get_token_name(char_code);
    fprintf(stderr, "%s ", token_name);
  }
  else {
    fprintf(stderr, "%c ", char_code);
  }
}

int main(int argc, char** argv) {
  char* filename = NULL;
  if (argc >= 2) {
    if (0==(yyin = fopen(argv[1],"r")))
    {
    printf("Cannot open file %s... \n",argv[1]);
    exit(2);
    }
    if (argc == 3) filename = argv[2];
  }
  else{
    exit(1);
  }
  initMe();
  yyparse();
  SyntaxTreeNode* syntaxTree = getSyntaxTreeRoot();
  printAST(syntaxTree, 0);
  if(filename != NULL) decompileAST(syntaxTree, filename);
  fprintf(stderr, "Program successfully parsed.\n");
  exit(0);
}
