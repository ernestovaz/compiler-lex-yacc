// Ernesto Vaz de Oliveira - 00302470
// Ricco Vasconcellos Constante Soares - 00307968

#include "AbstractSyntaxTree.h"

#include <stdlib.h>
#include <string.h>

char* _nodeTypeName(SyntaxNodeType type) {
    switch(type) {
        case ParenthesesNode:
            return "PARENTHESES";
        case AddNode:
            return "ADD";
        case SubNode:
            return "SUB";
        case ProdNode:
            return "PROD";
        case NegationNode:
            return "NEGATION";
        case LessNode:
            return "LESS THAN";
        case GreaterNode:
            return "GREATER THAN";
        case AndNode:
            return "AND";
        case OrNode:
            return "OR";
        case LessEqualNode:
            return "LESS THAN OR EQUAL";
        case GreaterEqualNode:
            return "GREATER THAN OR EQUAL";
        case EqualNode:
            return "EQUAL";
        case DifferentNode:
            return "DIFFERENT";
        case SymbolNode:
            return "SYMBOL";
        case VariableNode:
            return "VARIABLE";
        case VariableDefNode:
            return "VARIABLE DEFINITION";
        case ArrayDefNode:
            return "ARRAY DEFINITION";
        case ArrayNode:
            return "ARRAY";
        case FunctionNode:
            return "FUNCTION";
        case ExpressionListNode:
            return "EXPRESSION LIST ELEMENT";
        case AssignmentNode:
            return "ASSIGNMENT";
        case ReadNode:
            return "READ";
        case ReturnNode:
            return "RETURN";
        case PrintNode:
            return "PRINT";
        case PrintListNode:
            return "PRINT LIST ELEMENT";
        case IfNode:
            return "IF";
        case IfElseNode:
            return "IF ELSE";
        case WhileNode:
            return "WHILE";
        case CommandBlockNode:
            return "COMMAND BLOCK";
        case CommandListNode:
            return "COMMAND LIST";
        case LiteralListNode:
            return "LITERAL LIST ELEMENT";
        case IntTypeNode:
            return "INT TYPE";
        case CharTypeNode:
            return "CHAR TYPE";
        case FloatTypeNode:
            return "FLOAT TYPE";
        case DefinitionListNode:
            return "DEFINITION LIST ELEMENT";
        case FunctionDefNode:
            return "FUNCTION DEFINITION";
        case ParameterListNode:
            return "PARAMETER LIST ELEMENT";
    }
    return "ERROR";
}

SyntaxTreeNode* createAST(SyntaxNodeType type, Symbol* symbol,
    SyntaxTreeNode* c1, SyntaxTreeNode* c2, SyntaxTreeNode* c3, SyntaxTreeNode* c4){

    SyntaxTreeNode* node = malloc(sizeof(SyntaxTreeNode));
    node->type = type;
    node->symbol = symbol;
    node->children[0] = c1;
    node->children[1] = c2;
    node->children[2] = c3;
    node->children[3] = c4;
    return node;
}

void printAST(SyntaxTreeNode* node, int level) {
    if(node != NULL) {
        for(int i=0; i<level; i++) fprintf(stderr," ");
        if(level>0) fprintf(stderr, "|_");
        fprintf(stderr, _nodeTypeName(node->type));
        fprintf(stderr, " ");
        if(node->symbol != NULL) fprintf(stderr, "%s", node->symbol->name);
        fprintf(stderr, "\n");
        for(int i=0; i<4; i++){
            SyntaxTreeNode* child = node->children[i];
            printAST(child, level+1);
        }
    }
}