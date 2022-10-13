// Ernesto Vaz de Oliveira - 00302470
// Ricco Vasconcellos Constante Soares - 00307968

#include "SemanticAnalysis.h"

#include<string.h>
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
            analyzeCommandList(commandList, returnType);
        }
        analyzeProgram(node->children[1]);
    }
}

void analyzeCommandList(SyntaxTreeNode* commandList, DataType returnType) {
    SyntaxTreeNode* commandNode = commandList->children[0];
    analyzeCommand(commandNode, returnType);
    SyntaxTreeNode* next = commandList->children[1];
    if(next != NULL) analyzeCommandList(next, returnType);
}

void analyzeCommand(SyntaxTreeNode* node, DataType returnType) {
    if(node != NULL) {
        switch(node->type) {
            case CommandBlockNode:
                analyzeCommandList(node->children[0], returnType);
                break;
            case AssignmentNode: 
                analyzeAssignment(node);
                break;
            case ReadNode:
                analyzeRead(node);
                break;
            case ReturnNode: {
                DataType expressionType = getExpressionDataType(node->children[0]); 
                if(areNumericTypesIncompatible(expressionType, returnType)){
                    fprintf(stderr, "error: Incompatible type for return in function\n");
                    fflush(stderr);
                    errorCount++;
                }
                break;
            }
            case IfNode: {
                DataType conditionType = getExpressionDataType(node->children[0]);
                if(conditionType != DataTypeBool){
                    fprintf(stderr, "error: If condition must be of boolean type.\n");
                    fflush(stderr);
                    errorCount++;
                }
                analyzeCommand(node->children[1], returnType);
                break;
            }
            case IfElseNode: {
                DataType conditionType = getExpressionDataType(node->children[0]);
                if(conditionType != DataTypeBool){
                    fprintf(stderr, "error: If condition must be of boolean type.\n");
                    fflush(stderr);
                    errorCount++;
                }
                analyzeCommand(node->children[1], returnType);
                analyzeCommand(node->children[2], returnType);
                break;
            }
            case WhileNode: {
                DataType conditionType = getExpressionDataType(node->children[0]);
                if(conditionType != DataTypeBool){
                    fprintf(stderr, "error: While condition must be of boolean type.\n");
                    fflush(stderr);
                    errorCount++;
                }
                analyzeCommand(node->children[1], returnType);
                break;
            }
            case PrintNode: {
                SyntaxTreeNode* argumentList = node->children[0];
                while (argumentList) {
                    if(argumentList->children[0]) {
                        getExpressionDataType(argumentList->children[0]);
                    }
                    argumentList = argumentList->children[1];
                }
            }
            default:
        }
    }
}

void analyzeAssignment(SyntaxTreeNode* node) {
    SyntaxTreeNode* dataNode = node->children[0];
    Symbol* symbol = dataNode->symbol;
    checkSymbolUndeclared(symbol);
    if(symbol->type == SymbolFunction) {
        fprintf(stderr, "error: Cannot assign value to function: %s\n", symbol->name);
        errorCount++;
    }
    else {
        checkVariableArrayUsage(dataNode);
        DataType expectedType = symbol->dataType;
        DataType type = getExpressionDataType(node->children[1]);
        if (areNumericTypesIncompatible(expectedType, type)){
            fprintf(stderr, "error: Incompatible type for assignment to symbol: %s\n", symbol->name);
            errorCount++;
        }
    }
}

void analyzeRead(SyntaxTreeNode* node) {
    SyntaxTreeNode* variableOrArrayNode = node->children[0];
    Symbol* symbol = variableOrArrayNode->symbol;
    checkSymbolUndeclared(symbol);
    if(symbol->type == SymbolFunction) {
        fprintf(stderr, "error: Cannot assign value to function: %s\n", symbol->name);
        errorCount++;
    }
    else checkVariableArrayUsage(variableOrArrayNode);
}

void checkVariableArrayUsage(SyntaxTreeNode* node) {
    Symbol* symbol = node->symbol;
    if(node->type == VariableNode) {
        if(symbol->type == SymbolArray) {
            fprintf(stderr, "error: Attempt to use variable as array: %s\n", symbol->name);
            errorCount++;
        }
    }
    else if(node->type == ArrayNode) {
        if(symbol->type == SymbolVariable) {
            fprintf(stderr, "error: Attempt to index a variable: %s\n", symbol->name);
            errorCount++;
        } 
        else {
            SyntaxTreeNode* arrayIndex = node->children[0];
            DataType indexType = getExpressionDataType(arrayIndex);
            if(areNumericTypesIncompatible(indexType, DataTypeInt)) {
                fprintf(stderr, "error: Non integer index specified for array: %s\n", symbol->name);
                errorCount++;
            } 
        }
    }
}

DataType getExpressionDataType(SyntaxTreeNode* expressionNode) {
    Symbol* symbol = expressionNode->symbol;
    switch (expressionNode->type) {
        case ParenthesesNode:
            return getExpressionDataType(expressionNode->children[0]); 

        case VariableNode:
            checkSymbolUndeclared(symbol);
            if(symbol->type != SymbolVariable) {
                fprintf(stderr, "error: Incorrect usage for variable: %s\n", symbol->name);
                errorCount++;
            }
            return symbol->dataType;
        
        case ArrayNode:
            checkSymbolUndeclared(symbol);
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
        case GreaterEqualNode:
        case LessEqualNode:
        case LessNode:
        case GreaterNode: {
            DataType leftDataType = getExpressionDataType(expressionNode->children[0]);
            DataType rightDataType = getExpressionDataType(expressionNode->children[1]);
            if(areNumericTypesIncompatible(leftDataType, rightDataType)) {
                fprintf(stderr, "error: Incompatible values in expression \n");
                fflush(stderr);
                errorCount++;
            }
            return DataTypeBool;
            }
        case EqualNode:
        case DifferentNode: {
            DataType leftDataType = getExpressionDataType(expressionNode->children[0]);
            DataType rightDataType = getExpressionDataType(expressionNode->children[1]);
            if(areTypesIncompatible(leftDataType, rightDataType)) {
                fprintf(stderr, "error: Incompatible values in expression \n");
                fflush(stderr);
                errorCount++;
            }
            return DataTypeBool;
        }
        case AndNode:
        case OrNode: {
            DataType leftDataType = getExpressionDataType(expressionNode->children[0]);
            DataType rightDataType = getExpressionDataType(expressionNode->children[1]);
            if(areBooleanTypesIncompatible(leftDataType, rightDataType)) {
                fprintf(stderr, "error: Incompatible values in expression \n");
                fflush(stderr);
                errorCount++;
            }
            return DataTypeBool;
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
                           
        case FunctionNode: 
            if(symbol->type != SymbolFunction) {
                fprintf(stderr, "error: Error non-function symbol used as function: %s\n", symbol->name);
                errorCount++;
            }
            else checkFunctionUsage(expressionNode);
            return symbol->dataType;
            
        default:
    }
    return 0; 
}

DataType dataTypeFromTypeNode(SyntaxTreeNode* node) {
    switch(node->type) {
        case IntTypeNode:
            return DataTypeInt;
        case FloatTypeNode:
            return DataTypeFloat;
        case CharTypeNode:
            return DataTypeChar;
        default:
            return None;
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
        case SymbolCharLiteral:
            return DataTypeChar;
        default:
            return None;
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

void checkSymbolUndeclared(Symbol* symbol) {
    if(symbol->type == SymbolIdentifier){
        fprintf(stderr, "error: Symbol undeclared: %s\n", symbol->name);
        errorCount++;
    } 
}

int isArrayListIncompatible(Symbol* array, SyntaxTreeNode* literalList) {
    int actualSize = 0;
    Symbol** values;
    values = malloc(array->arraySize * sizeof(Symbol*));
    while(literalList != NULL) {
        Symbol* literal = literalList->children[0]->symbol;
        values[actualSize] = literal;

        DataType literalType = dataTypeFromLiteralNode(literalList->children[0]);
        if(areTypesIncompatible(array->dataType, literalType)) return 1;
        literalList = literalList->children[1];
        actualSize++;
    }
    array->arrayValues = values;
    if(actualSize != array->arraySize){
        array->arraySize = actualSize;
        return 1;  
    } 
    else return 0;
}

void checkFunctionDeclaration(SyntaxTreeNode* node) {
    Symbol* functionSymbol = node->symbol;
    int argumentCount = 0;
    SyntaxTreeNode* argumentList = node->children[1];
    while(argumentList != NULL) {
        Symbol* argumentSymbol = argumentList->symbol;
        if(argumentSymbol->type != SymbolIdentifier){
            fprintf(stderr, "error: Symbol redeclared: %s\n", argumentSymbol->name);
            errorCount++;
        } else {
            DataType argumentType = dataTypeFromTypeNode(argumentList->children[0]);
            argumentSymbol->dataType = argumentType;
            argumentSymbol->type = SymbolVariable;
        }
        argumentList = argumentList->children[1];
        argumentCount++;
    }
    Symbol** arguments;
    arguments = malloc(argumentCount * sizeof(Symbol*));
    argumentList = node->children[1];
    int index = 0;
    while(argumentList != NULL) {
        arguments[index] = argumentList->symbol;
        argumentList = argumentList->children[1];
        index++;
    }
    functionSymbol->argumentCount = argumentCount;
    functionSymbol->arguments = arguments;
}

void checkFunctionUsage(SyntaxTreeNode* node) {
    Symbol* functionSymbol = node->symbol;
    int argumentCount = 0;
    SyntaxTreeNode* argumentList = node->children[0];
    while(argumentList != NULL) {
        argumentCount++;
        if(argumentCount > functionSymbol->argumentCount) {
            fprintf(stderr, "error: Too many arguments for function: %s\n", functionSymbol->name);
            errorCount++;
            return;
        }
        DataType expectedType = functionSymbol->arguments[argumentCount - 1]->dataType;
        DataType argumentType = getExpressionDataType(argumentList->children[0]);
        if(areTypesIncompatible(expectedType, argumentType)) {
            fprintf(stderr, "error: Incompatible arguments for function: %s\n", functionSymbol->name);
            errorCount++;
            return;
        }
        argumentList->symbol = functionSymbol->arguments[argumentCount-1];
        argumentList = argumentList->children[1];
    }
    if(argumentCount < functionSymbol->argumentCount) {
        fprintf(stderr, "error: Too few arguments for function: %s\n", functionSymbol->name);
        errorCount++;
    }
}

void checkSymbolDeclaration(SyntaxTreeNode* node) {
    Symbol* symbol = node->symbol;

    if(symbol->type != SymbolIdentifier){
        fprintf(stderr, "error: Symbol redeclared: %s\n", symbol->name);
        errorCount++;
        return;
    }

    //save declared data type for symbol
    DataType declaredType = dataTypeFromTypeNode(node->children[0]);
    symbol->dataType = declaredType;

    SymbolType symbolType;
    switch(node->type) {
        case FunctionDefNode:
            symbolType = SymbolFunction;
            checkFunctionDeclaration(node);
            break;
        case VariableDefNode: {
            symbolType = SymbolVariable;
            DataType literalType = dataTypeFromLiteralNode(node->children[1]);
            if(areTypesIncompatible(declaredType, literalType)) {
                fprintf(stderr, "error: Incompatible value literal specified for variable: %s\n", symbol->name);
                errorCount++;
            }
            symbol->initialValue = node->children[1]->symbol;                
            addPrefix(symbol);
            break;
        }
        case ArrayDefNode: {
            symbolType = SymbolArray;
            SyntaxTreeNode* arraySizeNode = node->children[1];
            int declaredSize = atoi(arraySizeNode->symbol->name);
            if(declaredSize <= 0) {
                fprintf(stderr, "error: Non positive integer size specified for array: %s\n", symbol->name);
                errorCount++;
            }
            SyntaxTreeNode* elementList = node->children[2];
            symbol->arraySize = declaredSize;
            if(elementList != NULL) {
                if(isArrayListIncompatible(symbol, elementList)) {
                    fprintf(stderr, "error: Incompatible list for array: %s\n", symbol->name);
                    errorCount++;
                }
            }
            addPrefix(symbol);
            break;
        }
        default:
    }
    symbol->type = symbolType;
    
};

void addPrefix(Symbol* symbol) {
    char* prefixed = malloc(strlen(symbol->name)+5+1);
    strcpy(prefixed, "_var_");
    strcat(prefixed, symbol->name);
    symbol->name = prefixed;
}