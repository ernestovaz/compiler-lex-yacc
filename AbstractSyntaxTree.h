// Ernesto Vaz de Oliveira - 00302470
// Ricco Vasconcellos Constante Soares - 00307968

#pragma once

#include "Symbol.h"

#include <stdio.h>

typedef enum syntax_node_type{
    ParenthesesNode,
    AddNode,
    SubNode,
    DivNode,
    ProdNode,
    NegationNode,
    LessNode,
    GreaterNode,
    AndNode,
    OrNode,
    LessEqualNode,
    GreaterEqualNode,
    EqualNode,
    DifferentNode,
    SymbolNode,
    VariableNode,
    VariableDefNode,
    ArrayDefNode,
    ArrayNode,
    FunctionNode,
    ExpressionListNode,
    AssignmentNode,
    ReadNode,
    ReturnNode,
    PrintNode,
    PrintListNode,
    IfNode,
    IfElseNode,
    WhileNode,
    CommandListNode,
    CommandBlockNode,
    LiteralListNode,
    IntTypeNode,
    FloatTypeNode,
    CharTypeNode,
    FunctionDefNode,
    DefinitionListNode,
    ParameterListNode,
}SyntaxNodeType;

typedef struct syntax_tree_node {
    SyntaxNodeType type;
    Symbol* symbol;
    struct syntax_tree_node *children[4];
} SyntaxTreeNode;

SyntaxTreeNode* createAST(
    SyntaxNodeType type, 
    Symbol* symbol, 
    SyntaxTreeNode* c1, 
    SyntaxTreeNode* c2, 
    SyntaxTreeNode* c3, 
    SyntaxTreeNode* c4
);

void printAST(SyntaxTreeNode* node, int level);
