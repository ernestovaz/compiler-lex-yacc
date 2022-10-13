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
    SymbolFunction,
    //assigned in code generation
    SymbolLabel
}SymbolType;

typedef struct symbol{
    char* name;
    SymbolType type;
    DataType dataType;
    struct symbol** arguments;
    int arraySize;
    int argumentCount;

    struct symbol* initialValue;
} Symbol;