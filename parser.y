%{
  #include "AbstractSyntaxTree.h"
  #include "Symbol.h"
  int yyerror(char* s);
%}

%union{
    Symbol* symbol;
    SyntaxTreeNode* syntaxNode;
}

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

%token<symbol> TK_IDENTIFIER     
%token<symbol> LIT_INTEGER       
%token<symbol> LIT_FLOAT         
%token<symbol> LIT_CHAR          
%token<symbol> LIT_STRING        

%token TOKEN_ERROR

%left '|'
%left '&'
%left OPERATOR_EQ OPERATOR_DIF
%left '<' '>' OPERATOR_LE OPERATOR_GE
%left '+' '-'
%left '.' '/'
%right '~'

%type<syntaxNode> expression
%type<syntaxNode> expression_term
%type<syntaxNode> variable
%type<syntaxNode> function_call
%type<syntaxNode> expression_list
%type<syntaxNode> assignment
%type<syntaxNode> read
%type<syntaxNode> return
%type<syntaxNode> print
%type<syntaxNode> print_element_list
%type<syntaxNode> if
%type<syntaxNode> if_else
%type<syntaxNode> while
%type<syntaxNode> command_block
%type<syntaxNode> command_list
%type<syntaxNode> command
%type<syntaxNode> literal
%type<syntaxNode> literal_list
%type<syntaxNode> type

%%

program:              definition_list;
definition_list:      definition definition_list
                      | /*empty*/
                      ;

definition:           function_definition
                      | variable_definition
                      ;

function_definition:  type TK_IDENTIFIER '(' parameter_list ')' command_block;
parameter_list:       type TK_IDENTIFIER parameter_list
                      | /*empty*/
                      ;
command_block:        '{' command_list '}'  {$$=createAST(CommandBlockNode, NULL, $2, NULL, NULL, NULL);}
                      ;
command_list:         command ';' command_list {$$=createAST(CommandListNode, NULL, $1, $3, NULL, NULL);}
                      | command {$$=createAST(CommandListNode, NULL, $1, NULL, NULL, NULL);}
                      ;
command:              command_block {$$=$1;}
                      | assignment {$$=$1;}
                      | read {$$=$1;}
                      | return {$$=$1;}
                      | print {$$=$1;}
                      | if {$$=$1;}
                      | if_else {$$=$1;}
                      | while {$$=$1;}
                      | /*empty*/ {$$=NULL;}
                      ;

assignment:           variable ASSIGNMENT expression     {$$=createAST(AssignmentNode, NULL, $1, $3, NULL, NULL);}
                      ;
read:                 KW_READ variable {$$=createAST(ReadNode, NULL, $2, NULL, NULL, NULL);}
                      ;
return:               KW_RETURN expression {$$=createAST(ReturnNode, NULL, $2, NULL, NULL, NULL);}
                      ;
if:                   KW_IF '(' expression ')' command  {$$=createAST(IfNode, NULL, $3, $5, NULL, NULL);}
                      ;
if_else:              KW_IF '(' expression ')' command KW_ELSE command {$$=createAST(IfElseNode, NULL, $3, $5, $7, NULL);}
                      ;
while:                KW_WHILE '(' expression ')' command;
print:                KW_PRINT print_element_list {$$=createAST(PrintNode, NULL, $2, NULL, NULL, NULL);}
                      ;
print_element_list:   LIT_STRING print_element_list {$$=createAST(PrintListNode, $1, $2, NULL, NULL, NULL);}
                      | expression print_element_list {$$=createAST(PrintListNode, NULL, $1, $2, NULL, NULL);}
                      | /*empty*/ {$$=NULL;}
                      ;

expression:           expression_term                       {$$=$1;}
                      | '(' expression ')'                  {$$=$2;}
                      | '~' expression                      {$$=createAST(NegationNode, NULL, $2, NULL, NULL, NULL);}
                      | expression '+' expression           {$$=createAST(AddNode, NULL, $1, $3, NULL, NULL);}
                      | expression '-' expression           {$$=createAST(SubNode, NULL, $1, $3, NULL, NULL);}
                      | expression '.' expression           {$$=createAST(ProdNode, NULL, $1, $3, NULL, NULL);}
                      | expression '/' expression           {$$=createAST(DivNode, NULL, $1, $3, NULL, NULL);}
                      | expression '<' expression           {$$=createAST(LessNode, NULL, $1, $3, NULL, NULL);}
                      | expression '>' expression           {$$=createAST(GreaterNode, NULL, $1, $3, NULL, NULL);}
                      | expression '&' expression           {$$=createAST(AndNode, NULL, $1, $3, NULL, NULL);}
                      | expression '|' expression           {$$=createAST(OrNode, NULL, $1, $3, NULL, NULL);}
                      | expression OPERATOR_LE  expression  {$$=createAST(LessEqualNode, NULL, $1, $3, NULL, NULL);}
                      | expression OPERATOR_GE  expression  {$$=createAST(GreaterEqualNode, NULL, $1, $3, NULL, NULL);}
                      | expression OPERATOR_EQ  expression  {$$=createAST(EqualNode, NULL, $1, $3, NULL, NULL);}
                      | expression OPERATOR_DIF expression  {$$=createAST(DifferentNode, NULL, $1, $3, NULL, NULL);}
                      | function_call                       {$$=$1;}
                      ;

expression_term:      variable      {$$=$1;}
                      | LIT_INTEGER {$$=createAST(SymbolNode, $1, NULL, NULL, NULL, NULL);}
                      | LIT_FLOAT   {$$=createAST(SymbolNode, $1, NULL, NULL, NULL, NULL);}
                      | LIT_CHAR    {$$=createAST(SymbolNode, $1, NULL, NULL, NULL, NULL);}
                      ;


function_call:        TK_IDENTIFIER '(' expression_list ')' {$$=createAST(FunctionNode, $1, $3, NULL, NULL, NULL);}
                      ;
expression_list:      expression expression_list {$$=createAST(ExpressionListNode, NULL, $1, $2, NULL, NULL);}
                      | /*empty*/ {$$=NULL;}
                      ;

variable_definition:  type TK_IDENTIFIER '(' literal ')' ';'
                      | type TK_IDENTIFIER '[' LIT_INTEGER ']' literal_list ';'
                      ;

variable:             TK_IDENTIFIER {$$=createAST(VariableNode, $1, NULL, NULL, NULL, NULL);}
                      | TK_IDENTIFIER '[' expression ']' {$$=createAST(ArrayNode, $1, $3, NULL, NULL, NULL);}
                      ;

type:                 KW_CHAR {$$=createAST(TypeNode, NULL, NULL, NULL, NULL, NULL);}
                      | KW_INT {$$=createAST(TypeNode, NULL, NULL, NULL, NULL, NULL);}
                      | KW_FLOAT {$$=createAST(TypeNode, NULL, NULL, NULL, NULL, NULL);}
                      ;

literal:              LIT_INTEGER {$$=$1;}
                      | LIT_FLOAT {$$=$1;}
                      | LIT_CHAR {$$=$1;}
                      | LIT_STRING {$$=$1;}
                      ;

literal_list:         literal literal_list {$$=createAST(LiteralListNode, NULL, $1, $2, NULL, NULL);}
                      | /*empty*/
                      ;

%%

int yyerror(char* s) {
    fprintf(stderr, "Syntax error at line %d\n", lineNumber);
    exit(3);
}
