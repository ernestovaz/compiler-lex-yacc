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
void decompileAST(SyntaxTreeNode* node, char* filename);

void _decompileProgram(SyntaxTreeNode* node, FILE* file);

void _decompileExpression(SyntaxTreeNode* node, FILE* file);
void _decompileExpressionList(SyntaxTreeNode* node, FILE* file);

void _decompileSymbol(SyntaxTreeNode* node, FILE* file);
void _decompileArray(SyntaxTreeNode* node, FILE* file);

// expressions
void _decompileParentheses(SyntaxTreeNode* node, FILE* file);
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



void _decompileType(SyntaxTreeNode* node, FILE* file);
void _decompileVariableDef(SyntaxTreeNode* node, FILE* file);
void _decompileArrayDef(SyntaxTreeNode* node, FILE* file);
void _decompileLiteralList(SyntaxTreeNode* node, FILE* file);
void _decompileAssignment(SyntaxTreeNode* node, FILE* file);
void _decompileRead(SyntaxTreeNode* node, FILE* file);
void _decompileReturn(SyntaxTreeNode* node, FILE* file);
void _decompileIf(SyntaxTreeNode* node, FILE* file);
void _decompileIfElse(SyntaxTreeNode* node, FILE* file);
void _decompileWhile(SyntaxTreeNode* node, FILE* file);
void _decompilePrint(SyntaxTreeNode* node, FILE* file);
void _decompilePrintList(SyntaxTreeNode* node, FILE* file);
void _decompileCommand(SyntaxTreeNode* node, FILE* file);
void _decompileCommandList(SyntaxTreeNode* node, FILE* file);
void _decompileCommandBlock(SyntaxTreeNode* node, FILE* file);
void _decompileFunctionDef(SyntaxTreeNode* node, FILE* file);
void _decompileDefinition(SyntaxTreeNode* node, FILE* file);
void _decompileDefinitionList(SyntaxTreeNode* node, FILE* file);
