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
    if(code != NULL && code->type != TACSymbol) {
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
            case TACSub:
                fprintf(stderr,"SUB");
                break;
            case TACMul:
                fprintf(stderr,"MUL");
                break;
            case TACDiv:
                fprintf(stderr,"DIV");
                break;
            case TACNeg:
                fprintf(stderr,"NEG");
                break;
            case TACLess:
                fprintf(stderr,"LESS");
                break;
            case TACGreater:
                fprintf(stderr,"GREATER");
                break;
            case TACAnd:
                fprintf(stderr,"AND");
                break;
            case TACOr:
                fprintf(stderr,"OR");
                break;
            case TACLeq:
                fprintf(stderr,"LEQ");
                break;
            case TACGeq:
                fprintf(stderr,"GEQ");
                break;
            case TACEq:
                fprintf(stderr,"EQ");
                break;
            case TACDif:
                fprintf(stderr,"Dif");
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

ThreeAddressCode* generateBinaryOperationCode(ThreeAddressCodeType type, ThreeAddressCode* op1, ThreeAddressCode* op2, SymbolTable* table);
ThreeAddressCode* generateUnaryOperationCode(ThreeAddressCodeType type, ThreeAddressCode* op, SymbolTable* table);

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
                result = generateBinaryOperationCode(TACAdd, subtrees[0], subtrees[1], table);
                break;
            case SubNode:
                result = generateBinaryOperationCode(TACSub, subtrees[0], subtrees[1], table);
                break;
            case ProdNode:
                result = generateBinaryOperationCode(TACMul, subtrees[0], subtrees[1], table);
                break;
            case DivNode:
                result = generateBinaryOperationCode(TACDiv, subtrees[0], subtrees[1], table);
                break;
            case LessNode:
                result = generateBinaryOperationCode(TACLess, subtrees[0], subtrees[1], table);
                break;
            case GreaterNode:
                result = generateBinaryOperationCode(TACGreater, subtrees[0], subtrees[1], table);
                break;
            case AndNode:
                result = generateBinaryOperationCode(TACAnd, subtrees[0], subtrees[1], table);
                break;
            case OrNode:
                result = generateBinaryOperationCode(TACOr, subtrees[0], subtrees[1], table);
                break;
            case LessEqualNode:
                result = generateBinaryOperationCode(TACLeq, subtrees[0], subtrees[1], table);
                break;
            case GreaterEqualNode:
                result = generateBinaryOperationCode(TACGeq, subtrees[0], subtrees[1], table);
                break;
            case EqualNode:
                result = generateBinaryOperationCode(TACEq, subtrees[0], subtrees[1], table);
                break;
            case DifferentNode:
                result = generateBinaryOperationCode(TACDif, subtrees[0], subtrees[1], table);
                break;
            case NegationNode:
                result = generateUnaryOperationCode(TACNeg, subtrees[0], table);
                break;
            default:
                result = joinCodes(subtrees[0],joinCodes(subtrees[1],
                joinCodes(subtrees[2],subtrees[3])));
                break;
        }
    }

    return result;
}

ThreeAddressCode* generateBinaryOperationCode(ThreeAddressCodeType type, ThreeAddressCode* op1, ThreeAddressCode* op2, SymbolTable* table) {
    return joinCodes(
        joinCodes(op1, op2), 
        createCode(
            type, 
            op1?op1->result:NULL, 
            op2?op2->result:NULL, 
            insertTemporary(table)
        )
    );
}

ThreeAddressCode* generateUnaryOperationCode(ThreeAddressCodeType type, ThreeAddressCode* op, SymbolTable* table) {
    return joinCodes(
        op, 
        createCode(
            type, 
            op?op->result:NULL, 
            NULL, 
            insertTemporary(table)
        )
    );
}