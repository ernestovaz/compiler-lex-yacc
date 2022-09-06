#include "IntermediaryCode.h"

#include <stdlib.h>

ThreeAddressCode* createCode(
    ThreeAddressCodeType type, 
    Symbol* op1, 
    Symbol* op2, 
    Symbol* res) {

    ThreeAddressCode* code = malloc(sizeof(ThreeAddressCode));
    code->type = type;
    code->operator1 = op1;
    code->operator2 = op2;
    code->result = res;

    code->previous  = NULL;
    code->next      = NULL;

    return code;
}

void printCode(ThreeAddressCode* code) {
    if(code != NULL) {
        fprintf(stderr,"TAC_");
        switch(code->type) {
            case TACSymbol:
                fprintf(stderr,"SYMBOL");
                break;
            case TACMove:
                fprintf(stderr,"MOVE");
                break;
            case TACAdd:
                fprintf(stderr,"ADD");
                break;
            case TACMul:
                fprintf(stderr,"MUL");
                break;
            case TACLabel:
                fprintf(stderr,"LABEL");
                break;
            case TACBeginFun:
                fprintf(stderr,"BEGIN_FUN");
                break;
            case TACEndFun:
                fprintf(stderr,"END_FUN");
                break;
            case TACJump:
                fprintf(stderr,"JUMP");
                break;
            case TACCall:
                fprintf(stderr,"CALL");
                break;
            case TACArg:
                fprintf(stderr,"ARG");
                break;
            case TACRet:
                fprintf(stderr,"RET");
                break;
            case TACPrint:
                fprintf(stderr,"PRINT");
                break;
            case TACRead:
                fprintf(stderr,"READ");
                break;
        }
        fprintf(stderr, "(");

        if(code->operator1 != NULL) fprintf(stderr, "%s", code->operator1->name);
        
        fprintf(stderr,",");

        if(code->operator2 != NULL) fprintf(stderr, "%s", code->operator2->name);

        fprintf(stderr,",");

        if(code->result != NULL) fprintf(stderr, "%s", code->result->name);

        fprintf(stderr,")\n");
    }
}

void printCodeList(ThreeAddressCode* code){
    if(code != NULL) {
        printCodeList(code->previous);
        printCode(code);
    }
}

ThreeAddressCode* joinCodes(ThreeAddressCode* c1, ThreeAddressCode* c2) {
    if(c1 == NULL) return c2;
    if(c2 == NULL) return c1;

    ThreeAddressCode* pointer;  
    for(pointer = c2; pointer->previous != NULL; pointer = pointer->previous);
    
    pointer->previous = c1;
    return c2;
}

ThreeAddressCode* generateCode(SyntaxTreeNode* node, SymbolTable* table){
    ThreeAddressCode* result = NULL;
    ThreeAddressCode* subtrees[4];

    if(node != NULL){
        for(int i=0; i<4; i++)
            subtrees[i] = generateCode(node->children[i], table);

        switch(node->type) {
            case VariableNode:
            case SymbolNode: 
                result = createCode(TACSymbol, NULL, NULL, node->symbol);
                break;
            case AddNode:
                result = joinCodes(joinCodes(subtrees[0], subtrees[1]),
                    createCode(TACAdd, 
                subtrees[0]?subtrees[0]->result:NULL, subtrees[1]?subtrees[1]->result:NULL, 
                insertTemporary(table)));
                break;
            default:
                result = joinCodes(subtrees[0],joinCodes(subtrees[1],
                joinCodes(subtrees[2],subtrees[3])));
                break;
        }
    }

    return result;
}