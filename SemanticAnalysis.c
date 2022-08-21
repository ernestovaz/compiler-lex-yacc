#include "SemanticAnalysis.h"

#include<stdlib.h>

int errorCount;

void analyzeSyntaxTree(SyntaxTreeNode* node){
    errorCount = 0;
    analyzeDefinitionList(node);
    if(errorCount > 0){
        fprintf(stderr, "Found %d semantic error", errorCount);
        if(errorCount > 1) fprintf(stderr, "s");
        fprintf(stderr, ".\n");
        exit(4);
    }
}

void analyzeDefinitionList(SyntaxTreeNode* node) {
    if(node != NULL) {
        checkSymbolDeclaration(node->children[0]);
        if(node->children[1] != NULL) {
            analyzeDefinitionList(node->children[1]);
        }
    }
}

void checkSymbolDeclaration(SyntaxTreeNode* node) {
    Symbol* symbol = node->symbol;
    if(symbol->type != SymbolIdentifier){
        fprintf(stderr, "error: Symbol redeclared: %s\n", symbol->name);
        errorCount++;
    }
    else {
        SymbolType type;
        switch(node->type) {
            case FunctionDefNode:
                type = SymbolFunction;
                break;
            case VariableDefNode:
                type = SymbolVariable;
                break;
            case ArrayDefNode:
                type = SymbolArray;
                break;
        }
        symbol->type = type;
    }
};