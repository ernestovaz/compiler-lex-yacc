#pragma once

typedef enum symbol_type{
    SymbolIdentifier,
    SymbolCharLiteral,
    SymbolIntLiteral,
    SymbolFloatLiteral,
    SymbolStringLiteral
}SymbolType;

typedef struct symbol{
    char* name;
    SymbolType type;
} Symbol;