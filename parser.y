%token KW_CHAR           
%token KW_INT            
%token KW_FLOAT          

%token KW_IF             
%token KW_ELSE           
%token KW_WHILE          
%token KW_READ           
%token KW_PRINT          
%token KW_RETURN         

%token ASSIGNMENT        
%token OPERATOR_LE       
%token OPERATOR_GE       
%token OPERATOR_EQ       
%token OPERATOR_DIF      

%token TK_IDENTIFIER     

%token LIT_INTEGER       
%token LIT_FLOAT         
%token LIT_CHAR          
%token LIT_STRING        

%token TOKEN_ERROR

%{
  int yyerror(char* s);
%}

%%

PROGRAM:                      GLOBAL_DECLARATION_LIST;
GLOBAL_DECLARATION_LIST:      GLOBAL_DECLARATION  
                              | GLOBAL_DECLARATION GLOBAL_DECLARATION_LIST
                              | /*empty*/ ;
GLOBAL_DECLARATION:           FUNCTION_DECLARATION 
                              | VARIABLE_DECLARATION;

FUNCTION_DECLARATION:         '?' /*implement this*/;
VARIABLE_DECLARATION:         TYPE_KEYWORD IDENTIFIER '(' LITERAL_VALUE ')' ';' /*todo: treat arrays */;

IDENTIFIER:                   TK_IDENTIFIER;
TYPE_KEYWORD:                 KW_CHAR 
                              | KW_INT
                              | KW_FLOAT;

LITERAL_VALUE:                LIT_INTEGER 
                              | LIT_FLOAT
                              | LIT_CHAR
                              | LIT_STRING;


%%

int yyerror(char* s) {
    fprintf(stderr, "Syntax error at line %d\n", lineNumber);
    exit(3);
}
