#pragma once

#include "Symbol.h"

typedef enum syntax_node_type{
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
    ArrayNode,
    FunctionNode,
    ListNode,
    AssignmentNode,
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