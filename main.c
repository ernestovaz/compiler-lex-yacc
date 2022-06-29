#define MAX_ASCII_CHAR_CODE 127

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

int main(void) {
  while(running) {
    int code = yylex();
    if(code) pretty_print(code);
  }
  fprintf(stderr, "Read %d lines!\n", lineNumber);
  return 0;
}
