#pragma once

#include "Symbol.h"

typedef enum syntax_node_type{
    AddNode,
    SubNode,
    DivNode,
    ProdNode,
    SymbolNode
}SyntaxNodeType;

typedef struct syntax_tree_node {
    SyntaxNodeType type;
    Symbol* symbol;
    struct syntax_tree_node *children[4];
    int level;
} SyntaxTreeNode;

SyntaxTreeNode* createAST(
    SyntaxNodeType type, 
    Symbol* symbol, 
    int level,
    SyntaxTreeNode* c1, 
    SyntaxTreeNode* c2, 
    SyntaxTreeNode* c3, 
    SyntaxTreeNode* c4
);

void printAST(SyntaxTreeNode* node);