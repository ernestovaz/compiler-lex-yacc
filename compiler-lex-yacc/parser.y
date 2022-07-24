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

program:          		definition_list;
definition_list: 			definition definition_list
                  		| /*empty*/ 
											;

definition:       		function_definition 
                  		| variable_definition 
											;

function_definition:	type TK_IDENTIFIER '(' parameter_list ')' command_block;
parameter_list:       type TK_IDENTIFIER parameter_list
											| /*empty*/
											;
command_block:				'{' command_list '}';
command_list:					command ';' command_list
											| command 
											;
command:							command_block	
											| assignment
											| read
											| return
											| print
											| if
											| if_else
											| while
											| /*empty*/
											;

assignment: 	      	variable ASSIGNMENT expression;
read:									KW_READ variable;
return:								KW_RETURN expression;
if:										KW_IF '(' expression ')' command;
if_else:							KW_IF '(' expression ')' command KW_ELSE command;
while:								KW_WHILE '(' expression ')' command;
print:								KW_PRINT print_element_list;
print_element_list:		LIT_STRING print_element_list
											| expression print_element_list
											| /*empty*/
											;

expression: 		      expression_term
											| '(' expression ')'
											| '~' expression
											| expression '+' expression 
											| expression '-' expression
											| expression '.' expression
											| expression '/' expression
											| expression '<' expression
											| expression '>' expression
											| expression '&' expression
											| expression '|' expression
											| expression OPERATOR_LE  expression
											| expression OPERATOR_GE  expression
											| expression OPERATOR_EQ  expression
											| expression OPERATOR_DIF expression
											| function_call
											;

expression_term:			variable
											| LIT_INTEGER
											| LIT_FLOAT
											| LIT_CHAR
											;


function_call:				TK_IDENTIFIER '(' expression_list ')';
expression_list:			expression expression_list
											| /*empty*/
											;

variable_definition:	type TK_IDENTIFIER '(' literal ')' ';'
									 		| type TK_IDENTIFIER '[' LIT_INTEGER ']' literal_list ';'
											;

variable:							TK_IDENTIFIER 
											| TK_IDENTIFIER '[' expression ']'
											;

type:									KW_CHAR 
											| KW_INT
											| KW_FLOAT
											;

literal:							LIT_INTEGER 
											| LIT_FLOAT
											| LIT_CHAR
											| LIT_STRING
											;

literal_list:					literal literal_list
                      | /*empty*/
											;

%%

int yyerror(char* s) {
    fprintf(stderr, "Syntax error at line %d\n", lineNumber);
    exit(3);
}
