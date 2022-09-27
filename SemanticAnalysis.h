// Ernesto Vaz de Oliveira - 00302470
// Ricco Vasconcellos Constante Soares - 00307968

#pragma once
#include "AbstractSyntaxTree.h"
#include "Symbol.h"
#include "DataType.h"

void analyzeSyntaxTree(SyntaxTreeNode* root);

void analyzeDefinitionList(SyntaxTreeNode* node);
void analyzeProgram(SyntaxTreeNode* node);
void analyzeCommandList(SyntaxTreeNode* commandList, DataType returnType);
void analyzeCommand(SyntaxTreeNode* commandList, DataType returnType);
void analyzeAssignment(SyntaxTreeNode* node);
void analyzeRead(SyntaxTreeNode* node);

void checkSymbolDeclaration(SyntaxTreeNode* node);
void checkFunctionDeclaration(SyntaxTreeNode* node);
void checkFunctionUsage(SyntaxTreeNode* node);
void checkVariableArrayUsage(SyntaxTreeNode* node);

int areTypesIncompatible(DataType t1, DataType t2); 
int areNumericTypesIncompatible(DataType t1, DataType t2); 
int areBooleanTypesIncompatible(DataType t1, DataType t2); 

void checkSymbolUndeclared(Symbol* symbol);
int isArrayListIncompatible(int size, DataType type, SyntaxTreeNode* list);
DataType getExpressionDataType(SyntaxTreeNode* expressionNode);

DataType dataTypeFromTypeNode(SyntaxTreeNode* node);
DataType dataTypeFromLiteralNode(SyntaxTreeNode* node);

void addPrefix(Symbol* symbol);