#pragma once

#include "Symbol.h"

#define TABLE_SIZE 997

typedef struct symbol_table_node {
    char* key;              
    Symbol* symbol;
    struct symbol_table_node* next;
} SymbolTableNode;

typedef struct symbol_table {
    int size;
    int count;
    SymbolTableNode** table;
} SymbolTable;

// External functions
SymbolTable* createSymbolTable();
Symbol* insertSymbol(SymbolTable* table, char* name, SymbolType type);
Symbol* getSymbol(SymbolTable* table, char* name);
void freeSymbolTable(SymbolTable* table);
void printSymbolTable(SymbolTable* table);
