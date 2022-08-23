#pragma once
#include "AbstractSyntaxTree.h"
#include "Symbol.h"
#include "DataType.h"

void analyzeSyntaxTree(SyntaxTreeNode* root);

void analyzeDefinitionList(SyntaxTreeNode* node);
void analyzeProgram(SyntaxTreeNode* node);
void analyzeFunction(SyntaxTreeNode* commandList, DataType returnType);

void checkSymbolDeclaration(SyntaxTreeNode* node);

int areTypesIncompatible(DataType t1, DataType t2); 
int isArrayListIncompatible(int size, DataType type, SyntaxTreeNode* list);
int isExpressionInvalid(SyntaxTreeNode* expressionNode, DataType expectedType);

DataType dataTypeFromTypeNode(SyntaxTreeNode* node);
DataType dataTypeFromLiteralNode(SyntaxTreeNode* node);