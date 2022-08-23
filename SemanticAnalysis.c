#include "SemanticAnalysis.h"

#include "DataType.h"

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

DataType dataTypeFromTypeNode(SyntaxTreeNode* node) {
    switch(node->type) {
        case IntTypeNode:
            return DataTypeInt;
        case FloatTypeNode:
            return DataTypeFloat;
        case CharTypeNode:
            return DataTypeChar;
    }
}

DataType dataTypeFromLiteralNode(SyntaxTreeNode* node) {
    Symbol* symbol = node->symbol;
    switch(symbol->type) {
        case SymbolIntLiteral:
            return DataTypeInt;
        case SymbolFloatLiteral:
            return DataTypeFloat;
        case SymbolStringLiteral:
            return DataTypeString;
    }
}

int areTypesIncompatible(DataType t1, DataType t2) {
    // int and char are interchangeably compatible
    if(t1 == DataTypeInt && t2 == DataTypeChar) return 0;
    else if (t1 == DataTypeChar && t2 == DataTypeInt) return 0;
    else return t1 != t2;
}

int isArrayListIncompatible(int expectedSize, DataType expectedType, SyntaxTreeNode* literalList) {
    int actualSize = 0;
    while(literalList != NULL) {
        DataType literalType = dataTypeFromLiteralNode(literalList->children[0]);
        if(areTypesIncompatible(expectedType, literalType)) return 1;
        literalList = literalList->children[1];
        actualSize++;
    }
    if(actualSize != expectedSize) return 1;
    else return 0;
}

void checkSymbolDeclaration(SyntaxTreeNode* node) {
    Symbol* symbol = node->symbol;

    //save declared data type for symbol
    DataType declaredType = dataTypeFromTypeNode(node->children[0]);
    symbol->dataType = declaredType;

    if(symbol->type != SymbolIdentifier){
        fprintf(stderr, "error: Symbol redeclared: %s\n", symbol->name);
        errorCount++;
    }
    else {
        SymbolType symbolType;
        switch(node->type) {
            case FunctionDefNode:
                symbolType = SymbolFunction;
                break;
            case VariableDefNode:
                symbolType = SymbolVariable;
                DataType literalType = dataTypeFromLiteralNode(node->children[1]);
                if(areTypesIncompatible(declaredType, literalType)) {
                    fprintf(stderr, "error: Incompatible value literal specified for variable: %s\n", symbol->name);
                    errorCount++;
                }
                break;
            case ArrayDefNode:
                symbolType = SymbolArray;
                SyntaxTreeNode* arraySizeNode = node->children[1];
                DataType sizeLiteralType = dataTypeFromLiteralNode(arraySizeNode);
                int declaredSize = atoi(arraySizeNode->symbol->name);
                if(declaredSize <= 0) {
                    fprintf(stderr, "error: Non positive integer size specified for array: %s\n", symbol->name);
                    errorCount++;
                }
                SyntaxTreeNode* elementList = node->children[2];
                if(elementList != NULL) {
                    if(isArrayListIncompatible(declaredSize, declaredType, elementList)) {
                        fprintf(stderr, "error: Incompatible list for array: %s\n", symbol->name);
                        errorCount++;
                    }
                }
                symbol->arraySize = declaredSize;
                break;
        }
        symbol->type = symbolType;
    }
    
};