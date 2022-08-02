#pragma once

#include "Symbol.h"

#include <stdio.h>

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
    VariableDefNode,
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
    TypeNode,
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
void decompileAST(SyntaxTreeNode* node, char* filename);

void _decompileExpression(SyntaxTreeNode* node, FILE* file);
void _decompileExpressionList(SyntaxTreeNode* node, FILE* file);

void _decompileSymbol(SyntaxTreeNode* node, FILE* file);

// expressions
void _decompileAdd(SyntaxTreeNode* node, FILE* file);
void _decompileSub(SyntaxTreeNode* node, FILE* file);
void _decompileProd(SyntaxTreeNode* node, FILE* file);
void _decompileDiv(SyntaxTreeNode* node, FILE* file);
void _decompileLess(SyntaxTreeNode* node, FILE* file);
void _decompileGreater(SyntaxTreeNode* node, FILE* file);
void _decompileAnd(SyntaxTreeNode* node, FILE* file);
void _decompileOr(SyntaxTreeNode* node, FILE* file);
void _decompileNegation(SyntaxTreeNode* node, FILE* file);
void _decompileLessEqual(SyntaxTreeNode* node, FILE* file);
void _decompileGreaterEqual(SyntaxTreeNode* node, FILE* file);
void _decompileEqual(SyntaxTreeNode* node, FILE* file);
void _decompileDifferent(SyntaxTreeNode* node, FILE* file);
void _decompileFunction(SyntaxTreeNode* node, FILE* file);

void _decompileArray(SyntaxTreeNode* node, FILE* file);

void _decompileProgram(SyntaxTreeNode* node, FILE* file);
