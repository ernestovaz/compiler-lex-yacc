// Ernesto Vaz de Oliveira - 00302470
// Ricco Vasconcellos Constante Soares - 00307968

#pragma once

#include "AbstractSyntaxTree.h"

void decompileAST(SyntaxTreeNode* node, char* filename);

// internal functions
void decompileProgram(SyntaxTreeNode* node, FILE* file);
void decompileExpression(SyntaxTreeNode* node, FILE* file);
void decompileExpressionList(SyntaxTreeNode* node, FILE* file);
void decompileSymbol(SyntaxTreeNode* node, FILE* file);
void decompileArray(SyntaxTreeNode* node, FILE* file);
void decompileParentheses(SyntaxTreeNode* node, FILE* file);
void decompileAdd(SyntaxTreeNode* node, FILE* file);
void decompileSub(SyntaxTreeNode* node, FILE* file);
void decompileProd(SyntaxTreeNode* node, FILE* file);
void decompileDiv(SyntaxTreeNode* node, FILE* file);
void decompileLess(SyntaxTreeNode* node, FILE* file);
void decompileGreater(SyntaxTreeNode* node, FILE* file);
void decompileAnd(SyntaxTreeNode* node, FILE* file);
void decompileOr(SyntaxTreeNode* node, FILE* file);
void decompileNegation(SyntaxTreeNode* node, FILE* file);
void decompileLessEqual(SyntaxTreeNode* node, FILE* file);
void decompileGreaterEqual(SyntaxTreeNode* node, FILE* file);
void decompileEqual(SyntaxTreeNode* node, FILE* file);
void decompileDifferent(SyntaxTreeNode* node, FILE* file);
void decompileFunction(SyntaxTreeNode* node, FILE* file);
void decompileType(SyntaxTreeNode* node, FILE* file);
void decompileVariableDef(SyntaxTreeNode* node, FILE* file);
void decompileArrayDef(SyntaxTreeNode* node, FILE* file);
void decompileLiteralList(SyntaxTreeNode* node, FILE* file);
void decompileAssignment(SyntaxTreeNode* node, FILE* file);
void decompileRead(SyntaxTreeNode* node, FILE* file);
void decompileReturn(SyntaxTreeNode* node, FILE* file);
void decompileIf(SyntaxTreeNode* node, FILE* file);
void decompileIfElse(SyntaxTreeNode* node, FILE* file);
void decompileWhile(SyntaxTreeNode* node, FILE* file);
void decompilePrint(SyntaxTreeNode* node, FILE* file);
void decompilePrintList(SyntaxTreeNode* node, FILE* file);
void decompileCommand(SyntaxTreeNode* node, FILE* file);
void decompileCommandList(SyntaxTreeNode* node, FILE* file);
void decompileCommandBlock(SyntaxTreeNode* node, FILE* file);
void decompileFunctionDef(SyntaxTreeNode* node, FILE* file);
void decompileDefinition(SyntaxTreeNode* node, FILE* file);
void decompileDefinitionList(SyntaxTreeNode* node, FILE* file);