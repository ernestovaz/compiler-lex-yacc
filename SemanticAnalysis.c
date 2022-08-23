#include "SemanticAnalysis.h"


#include<stdlib.h>

int errorCount;

void analyzeSyntaxTree(SyntaxTreeNode* root){
    errorCount = 0;
    analyzeDefinitionList(root);
    analyzeProgram(root);
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
        analyzeDefinitionList(node->children[1]);
    }
}

void analyzeProgram(SyntaxTreeNode* node) {
    if(node != NULL) {
        SyntaxTreeNode* definition = node->children[0];
        if(definition != NULL && definition->type == FunctionDefNode) {
            DataType returnType = definition->symbol->dataType;
            SyntaxTreeNode* commandList = definition->children[2]->children[0];
            //TODO: HANDLE SYMBOL ARGUMENTS
            analyzeFunction(commandList, returnType);
        }
        analyzeProgram(node->children[1]);
    }
}

void analyzeFunction(SyntaxTreeNode* commandList, DataType returnType) {
    SyntaxTreeNode* commandNode = commandList->children[0];
    if(commandNode != NULL) {
        switch(commandNode->type) {
            case ReturnNode:
                DataType expressionType = getExpressionDataType(commandNode->children[0]); 
                if(areTypesIncompatible(expressionType, returnType)){
                    fprintf(stderr, "error: Incompatible type for return in function\n");
                    fflush(stderr);
                    errorCount++;
                }
        }
    }
    SyntaxTreeNode* next = commandList->children[1];
    if(next != NULL) analyzeFunction(next, returnType);
}


DataType getExpressionDataType(SyntaxTreeNode* expressionNode) {
    Symbol* symbol = expressionNode->symbol;
    switch (expressionNode->type) {
        case ParenthesesNode:
            return getExpressionDataType(expressionNode->children[0]); 

        case VariableNode:
            if(isUndeclared(symbol)) {
                fprintf(stderr, "error: Symbol undeclared: %s\n", symbol->name);
                errorCount++;
            }
            if(symbol->type != SymbolVariable) {
                fprintf(stderr, "error: Incorrect usage for variable: %s\n", symbol->name);
                errorCount++;
            }
            return symbol->dataType;
        
        case ArrayNode:
            if(isUndeclared(symbol)) {
                fprintf(stderr, "error: Symbol undeclared: %s\n", symbol->name);
                errorCount++;
            }
            if(symbol->type != SymbolArray) {
                fprintf(stderr, "error: Incorrect usage for array: %s\n", symbol->name);
                errorCount++;
            } else {
                SyntaxTreeNode* arrayIndex = expressionNode->children[0];
                if(getExpressionDataType(arrayIndex) != DataTypeInt) {
                    fprintf(stderr, "error: Non integer index specified for array: %s\n", symbol->name);
                    errorCount++;
                } else {
                    int indexValue = atoi(arrayIndex->symbol->name);
                    if (indexValue < 0 || indexValue >= symbol->arraySize) {
                        fprintf(stderr, "error: Index out of bounds specified for array: %s\n", symbol->name);
                        errorCount++;
                    }
                }
            }
            return symbol->dataType;

        case SymbolNode:
            return dataTypeFromLiteralNode(expressionNode);

        case AddNode:
        case SubNode:
        case ProdNode:
        case DivNode: {
            DataType leftDataType = getExpressionDataType(expressionNode->children[0]);
            DataType rightDataType = getExpressionDataType(expressionNode->children[1]);
            if(areNumericTypesIncompatible(leftDataType, rightDataType)) {
                fprintf(stderr, "error: Incompatible values in expression \n");
                fflush(stderr);
                errorCount++;
            }
            return leftDataType;
        }
        case EqualNode:
        case GreaterEqualNode:
        case LessEqualNode:
        case LessNode:
        case GreaterNode:
        case DifferentNode:
        case AndNode:
        case OrNode: {
            DataType leftDataType = getExpressionDataType(expressionNode->children[0]);
            DataType rightDataType = getExpressionDataType(expressionNode->children[1]);
            if(areBooleanTypesIncompatible(leftDataType, rightDataType)) {
                fprintf(stderr, "error: Incompatible values in expression \n");
                fflush(stderr);
                errorCount++;
            }
            return leftDataType;
        }

        case NegationNode: {
            DataType dataType = getExpressionDataType(expressionNode->children[0]);
            if(dataType != DataTypeBool) {
                fprintf(stderr, "error: Incompatible value in expression \n");
                fflush(stderr);
                errorCount++;
            }
            return dataType;
        }
    }
    return 0; //for now accepts by default, ideally shouldn't
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

int areNumericTypesIncompatible(DataType t1, DataType t2) {
    if(t1 == DataTypeBool || t2 == DataTypeBool) return 1;
    else return areTypesIncompatible(t1, t2);
}

int areBooleanTypesIncompatible(DataType t1, DataType t2) {
    if(t1 != DataTypeBool || t2 != DataTypeBool) return 1;
    else return 0;
}

int isUndeclared(Symbol* symbol) {
    if(symbol->type == SymbolIdentifier) return 1;
    else return 0;
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
                //TODO: FILL THE FUNCTION SYMBOL WITH ARGUMENT COUNT AND TYPES
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