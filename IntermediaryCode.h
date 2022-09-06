// Ernesto Vaz de Oliveira - 00302470
// Ricco Vasconcellos Constante Soares - 00307968

#pragma once

#include "Symbol.h"

#include "AbstractSyntaxTree.h"
#include "SymbolTable.h"

typedef enum three_address_code_type{
    TACSymbol,
    TACMove,
    TACAccess,
    TACAdd,
    TACSub,
    TACMul,
    TACDiv,
    TACNeg,
    TACLess,
    TACGreater,
    TACAnd,
    TACOr,
    TACLeq,
    TACGeq,
    TACEq,
    TACDif,
    TACLabel,
    TACBeginFun,
    TACEndFun,
    TACJump,
    TACJumpF,
    TACCall,
    TACArg,
    TACRet,
    TACPrint,
    TACRead
} ThreeAddressCodeType;

typedef struct three_address_code{
    ThreeAddressCodeType type;
    Symbol* operator1;
    Symbol* operator2;
    Symbol* result;
    struct three_address_code* previous;
    struct three_address_code* next;
} ThreeAddressCode;

ThreeAddressCode* createCode(ThreeAddressCodeType type, Symbol* op1, Symbol* op2, Symbol* res);
void printCode(ThreeAddressCode* code);
void printCodeList(ThreeAddressCode* code);
ThreeAddressCode* joinCodes(ThreeAddressCode* c1, ThreeAddressCode* c2);

ThreeAddressCode* generateCode(SyntaxTreeNode* node, SymbolTable* table);