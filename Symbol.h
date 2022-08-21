#pragma once

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
} Symbol;