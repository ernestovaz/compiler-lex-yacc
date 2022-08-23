// Ernesto Vaz de Oliveira - 00302470
// Ricco Vasconcellos Constante Soares - 00307968

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
int areNumericTypesIncompatible(DataType t1, DataType t2); 
int areBooleanTypesIncompatible(DataType t1, DataType t2); 

int isUndeclared(Symbol* symbol);
int isArrayListIncompatible(int size, DataType type, SyntaxTreeNode* list);
DataType getExpressionDataType(SyntaxTreeNode* expressionNode);

DataType dataTypeFromTypeNode(SyntaxTreeNode* node);
DataType dataTypeFromLiteralNode(SyntaxTreeNode* node);