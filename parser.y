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

PROGRAM:                      GLOBAL_DEFINITION_LIST;
GLOBAL_DEFINITION_LIST:       GLOBAL_DEFINITION  GLOBAL_DEFINITION_LIST
                              | GLOBAL_DEFINITION
                              | /*empty*/ ;
GLOBAL_DEFINITION:            FUNCTION_DEFINITION 
                              | VARIABLE_DEFINITION;

FUNCTION_DEFINITION:          FUNCTION_HEADER FUNCTION_BODY;
FUNCTION_HEADER:              TYPE_KEYWORD IDENTIFIER '(' PARAMETER_LIST ')' 
FUNCTION_BODY:                COMMAND_BLOCK:
PARAMETER_LIST:               PARAMETER PARAMETER_LIST
                              | PARAMETER
                              | /*empty*/;
PARAMETER:                    TYPE_KEYWORD IDENTIFIER;
COMMAND_BLOCK:                '{' COMMAND_LIST '}';
COMMAND_LIST:                 /* TO BE IMPLEMENTED */;


VARIABLE_DEFINITION:          SINGLE_VARIABLE_DECLARATION
                              | ARRAY_DEFINITION;
SINGLE_VARIABLE_DECLARATION:  TYPE_KEYWORD IDENTIFIER '(' LITERAL_VALUE ')' ';';
ARRAY_DEFINITION:             TYPE_KEYWORD IDENTIFIER '[' ARRAY_SIZE_VALUE ']' LITERAL_VALUE_LIST ';'
ARRAY_SIZE_VALUE:             LIT_INTEGER;


IDENTIFIER:                   TK_IDENTIFIER;
TYPE_KEYWORD:                 KW_CHAR 
                              | KW_INT
                              | KW_FLOAT;

LITERAL_VALUE_LIST:           LITERAL_VALUE LITERAL_VALUE_LIST
                              | LITERAL_VALUE 
                              | /*empty*/;

LITERAL_VALUE:                LIT_INTEGER 
                              | LIT_FLOAT
                              | LIT_CHAR
                              | LIT_STRING;


%%

int yyerror(char* s) {
    fprintf(stderr, "Syntax error at line %d\n", lineNumber);
    exit(3);
}
