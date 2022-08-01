#include "AbstractSyntaxTree.h"

#include <stdio.h>
#include <stdlib.h>

char* _nodeTypeName(SyntaxNodeType type) {
    switch(type) {
        case AddNode:
            return "ADD";
        case SubNode:
            return "SUB";
        case ProdNode:
            return "PROD";
        case SymbolNode:
            return "SYMBOL";
    }
    return "ERROR";
}

SyntaxTreeNode* createAST(SyntaxNodeType type, Symbol* symbol, int level, 
    SyntaxTreeNode* c1, SyntaxTreeNode* c2, SyntaxTreeNode* c3, SyntaxTreeNode* c4){

    SyntaxTreeNode* node = malloc(sizeof(SyntaxTreeNode));
    node->type = type;
    node->symbol = symbol;
    node->level = level;
    node->children[0] = c1;
    node->children[1] = c2;
    node->children[2] = c3;
    node->children[3] = c4;
    return node;
}

void printAST(SyntaxTreeNode* node) {
    fprintf(stderr, _nodeTypeName(node->type));
    if(node->symbol != NULL) fprintf(stderr, "%s", node->symbol->name);
    fprintf(stderr, "\n");
    for(int i=0; i<4; i++){
        SyntaxTreeNode* child = node->children[i];
        if(child != NULL) printAST(child);
    }
}