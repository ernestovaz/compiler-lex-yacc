#pragma once
#include "SymbolTable.h"
#include "IntermediaryCode.h"

void generateAssembly(ThreeAddressCode* first, SymbolTable* table);
void generateSymbolTableAssembly(SymbolTable* table, FILE* file);