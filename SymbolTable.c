#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SymbolTable.h"

int _hashFunction(char* key){
    int sum = 0;
    for (int i=0; i<strlen(key); i++){
        sum = (3*sum+(int)key[i]) % TABLE_SIZE;
    }
    return sum;
}

SymbolTableNode* _createNode(char* name, SymbolType type) {
    Symbol* symbol = malloc(sizeof(Symbol));
    symbol->name = (char*) malloc (strlen(name) + 1);
    strcpy(symbol->name, name);
    symbol->type = type;

    SymbolTableNode* node = malloc(sizeof(SymbolTableNode));
    node->key = (char*) malloc (strlen(name) + 1);
    strcpy(node->key, name);
    node->symbol = symbol;

    return node;
}

void _handleCollision(SymbolTableNode* position, SymbolTableNode* node){
    while(position != NULL){
        if (strcmp(position->key, node->key) == 0){
            position->symbol = node->symbol;
            return;
        } 
        if (position->next == NULL) {
            position->next = node;
            return;
        }
        position = position->next;
    }
}

char* _getSymbolTypeName(SymbolType type) {
    switch(type) {
        case SymbolIdentifier:
            return "Identifier";
        case SymbolCharLiteral:
            return "Literal(char)";
        case SymbolIntLiteral:
            return "Literal(int)";
        case SymbolFloatLiteral:
            return "Literal(float)";
        case SymbolStringLiteral:
            return "Literal(string)";
    }
    return "ERROR";
}

void _printSymbol(Symbol symbol){
    char* type = _getSymbolTypeName(symbol.type);
    printf("%-15s %s", type, symbol.name);
}


SymbolTable* createSymbolTable(){
    SymbolTable* table = malloc(sizeof(SymbolTable));
    table->size = TABLE_SIZE;
    table->count = 0;
    table->table = calloc(table->size,sizeof(SymbolTableNode*));
    for(int i=0; i<table->size; i++){
        table->table[i] = NULL;
    }
    return table;
}

Symbol* insertSymbol(SymbolTable* table, char* name, SymbolType type){
    SymbolTableNode* node = _createNode(name, type);

    int index = _hashFunction(name);
    SymbolTableNode* position = table->table[index];
    if (position == NULL){
        if (table->count == table->size){
            printf("Error: Symbols exceed max number in table.\n");
            return NULL;
        }
        table->table[index] = node;
        table->count++;
    }
    else{
        _handleCollision(position, node);
    }

    return node->symbol;
}

Symbol* getSymbol(SymbolTable* table, char* name){
    int index = _hashFunction(name);
    SymbolTableNode* node = table->table[index];

    while(node != NULL){
        if (strcmp(node->key, name) == 0){
            return node->symbol;
        } else if (node->next == NULL){
            return NULL;
        }
        node = node->next;
    }

    return NULL;
}

void freeSymbolTable(SymbolTable* table) {
    SymbolTableNode *node, *next;
    for(int i=0; i<table->size; i++) {
        node = table->table[i];
        while(node != NULL) {
            next = node->next;
            free(node->symbol);
            free(node);
            node = next;
        }
    }
    free(table);
}

void printSymbolTable(SymbolTable* table){
    SymbolTableNode* node;
    printf("-----SYMBOL TABLE-----\n");
    for (int i=0; i<table->size; i++) {
        node = table->table[i];
        if (node != NULL) {
            do {
                printf("[%011d] ", node->key);
                _printSymbol(*(node->symbol));
                printf("\n");
                node = node->next;
            } while(node != NULL);
        }
    }
    printf("----------------------\n");
}
