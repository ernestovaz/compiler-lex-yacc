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

%left '|'
%left '&'
%left OPERATOR_EQ OPERATOR_DIF
%left '<' '>' OPERATOR_LE OPERATOR_GE
%left '+' '-'
%left '.' '/'
%right '~'

%{
  int yyerror(char* s);
%}

%%

PROGRAM:                      GLOBAL_DEFINITION_LIST;
GLOBAL_DEFINITION_LIST:       GLOBAL_DEFINITION  GLOBAL_DEFINITION_LIST
                              | /*empty*/ ;
GLOBAL_DEFINITION:            FUNCTION_DEFINITION 
                              | VARIABLE_DEFINITION;

FUNCTION_DEFINITION:          FUNCTION_HEADER COMMAND_BLOCK;
FUNCTION_HEADER:              TYPE_KEYWORD IDENTIFIER '(' PARAMETER_LIST ')' 
PARAMETER_LIST:               PARAMETER PARAMETER_LIST
                              | /*empty*/;
PARAMETER:                    TYPE_KEYWORD IDENTIFIER;
COMMAND_BLOCK:                '{' COMMAND_LIST '}';
COMMAND_LIST:                 COMMAND ';' COMMAND_LIST
			      									| COMMAND ;
COMMAND:		      						COMMAND_BLOCK	
			 												| ASSIGNMENT_COMMAND
															| READ_COMMAND
															| RETURN_COMMAND
															| PRINT_COMMAND
			 												| /*empty*/;

ASSIGNMENT_COMMAND: 	      	VARIABLE_IDENTIFIER ASSIGNMENT EXPRESSION;
READ_COMMAND:									KW_READ VARIABLE_IDENTIFIER;
RETURN_COMMAND:								KW_RETURN EXPRESSION;
PRINT_COMMAND:								KW_PRINT PRINTABLE_ELEMENT_LIST;
PRINTABLE_ELEMENT_LIST:				LIT_STRING PRINTABLE_ELEMENT_LIST
															| EXPRESSION PRINTABLE_ELEMENT_LIST
															| /*empty*/;

EXPRESSION_TERM:			      	VARIABLE_IDENTIFIER
							 								| LIT_INTEGER
															| LIT_CHAR;

EXPRESSION: 		      				EXPRESSION_TERM 
															| '(' EXPRESSION ')'
			      									| EXPRESSION '+' EXPRESSION 
			      									| EXPRESSION '-' EXPRESSION
			      									| EXPRESSION '.' EXPRESSION
			      									| EXPRESSION '/' EXPRESSION
			      									| EXPRESSION '<' EXPRESSION
			      									| EXPRESSION '>' EXPRESSION
			      									| EXPRESSION '&' EXPRESSION
			      									| EXPRESSION '|' EXPRESSION
			      									| '~' EXPRESSION
			      									| EXPRESSION OPERATOR_LE EXPRESSION
			      									| EXPRESSION OPERATOR_GE EXPRESSION
			      									| EXPRESSION OPERATOR_EQ EXPRESSION
			      									| EXPRESSION OPERATOR_DIF EXPRESSION
			      									| /*empty*/;

VARIABLE_DEFINITION:          SINGLE_VARIABLE_DECLARATION
                              | ARRAY_DEFINITION;
SINGLE_VARIABLE_DECLARATION:  TYPE_KEYWORD IDENTIFIER '(' LITERAL_VALUE ')' ';';
ARRAY_DEFINITION:             TYPE_KEYWORD IDENTIFIER '[' ARRAY_SIZE_VALUE ']' LITERAL_VALUE_LIST ';'
ARRAY_SIZE_VALUE:             LIT_INTEGER;


VARIABLE_IDENTIFIER:					IDENTIFIER 
									 						| IDENTIFIER '[' EXPRESSION ']'

IDENTIFIER:                   TK_IDENTIFIER;
TYPE_KEYWORD:                 KW_CHAR 
                              | KW_INT
                              | KW_FLOAT;

LITERAL_VALUE_LIST:           LITERAL_VALUE LITERAL_VALUE_LIST
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
