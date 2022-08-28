// Ernesto Vaz de Oliveira - 00302470
// Ricco Vasconcellos Constante Soares - 00307968

#pragma once
#include "DataType.h"

typedef enum symbol_type{
    //assigned to symbols at syntax parsing
    SymbolCharLiteral,
    SymbolIntLiteral,
    SymbolFloatLiteral,
    SymbolStringLiteral,
    SymbolIdentifier,   
    //assigned to identifiers at semantic analysis
    SymbolVariable,
    SymbolArray,
    SymbolFunction
}SymbolType;

typedef struct symbol{
    char* name;
    SymbolType type;
    DataType dataType;
    int arraySize;
    DataType* arguments;
    int argumentCount;
} Symbol;