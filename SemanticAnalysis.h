#pragma once
#include "AbstractSyntaxTree.h"
#include "Symbol.h"

void analyzeSyntaxTree(SyntaxTreeNode* node);

void analyzeDefinitionList(SyntaxTreeNode* node);

void checkSymbolDeclaration(SyntaxTreeNode* node);