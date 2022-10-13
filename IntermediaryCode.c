#include "IntermediaryCode.h"

#include <stdlib.h>

ThreeAddressCode* generateBinaryOperationCode(ThreeAddressCodeType type, ThreeAddressCode* op1, ThreeAddressCode* op2, SymbolTable* table);
ThreeAddressCode* generateUnaryOperationCode(ThreeAddressCodeType type, ThreeAddressCode* op, SymbolTable* table);
ThreeAddressCode* generateAccessCode(Symbol* symbol, ThreeAddressCode* index, SymbolTable* table);
ThreeAddressCode* generateFunctionCode(Symbol* symbol, ThreeAddressCode* c1);
ThreeAddressCode* generateIfElse(ThreeAddressCode* c1, ThreeAddressCode* c2, ThreeAddressCode* c3, SymbolTable* table);
ThreeAddressCode* generateIf(ThreeAddressCode* c1, ThreeAddressCode* c2, SymbolTable* table);
ThreeAddressCode* generateWhile(ThreeAddressCode* c1, ThreeAddressCode* c2, SymbolTable* table);
ThreeAddressCode* generateMoveCode(ThreeAddressCode* c1, ThreeAddressCode* c2);
ThreeAddressCode* generateWhile(ThreeAddressCode* c1, ThreeAddressCode* c2, SymbolTable* table);

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
            case FunctionDefNode:
                result = generateFunctionCode(node->symbol, subtrees[2]);
                break;
            
            case VariableNode:
            case SymbolNode: 
                result = createCode(TACSymbol, NULL, NULL, node->symbol);
                break;
                
            case ArrayNode:
                result = generateAccessCode(node->symbol, subtrees[0], table);
                break;

            case AssignmentNode:
                result = generateMoveCode(subtrees[1], subtrees[0]);
                break;

            case ReadNode: 
                result = joinCodes(subtrees[0], createCode(TACRead, NULL, NULL, subtrees[0]?subtrees[0]->result:NULL));
                break;

            case PrintListNode:
                if(node->symbol)
                    result = joinCodes(createCode(TACPrint, NULL, NULL, node->symbol), subtrees[1]);
                else 
                    result = joinCodes(subtrees[0], joinCodes(createCode(TACPrint, NULL, NULL, subtrees[0]?subtrees[0]->result:NULL), subtrees[1]));
                break;
                
            case ReturnNode:
                result = joinCodes(subtrees[0], createCode(TACRet, NULL, NULL, subtrees[0]?subtrees[0]->result:NULL));
                break;
                
            case IfNode:
                result = generateIf(subtrees[0], subtrees[1], table);
                break;

            case IfElseNode:
                result = generateIfElse(subtrees[0], subtrees[1], subtrees[2], table);
                break;
                
            case WhileNode:
                result = generateWhile(subtrees[0], subtrees[1], table);
                break;
                
            case FunctionNode: 
                result = joinCodes(subtrees[0], createCode(TACCall, node->symbol, NULL, insertTemporary(table, node->symbol->dataType)));
                break;
            
            case ExpressionListNode:{
                //result = joinCodes(createCode(TACArg, NULL, NULL, subtrees[0]?subtrees[0]->result:NULL), subtrees[1]);
                //ThreeAddressCode* argument = createCode(TACSymbol, NULL, NULL, node->symbol);
                //result = generateMoveCode(subtrees[0], argument);
                result = joinCodes(createCode(TACMove, subtrees[0]?subtrees[0]->result:NULL, NULL, node->symbol), subtrees[1]);
            }
                break;
            
            // Operators
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
            insertTemporary(table, op1->result->dataType)
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
            insertTemporary(table, op->result->dataType)
        )
    );
}

ThreeAddressCode* generateMoveCode(ThreeAddressCode* c1, ThreeAddressCode* c2) {
    return joinCodes(
        joinCodes(c1,c2), 
        createCode(
            TACMove, 
            c1?c1->result:NULL,
            NULL, 
            c2?c2->result:NULL
        )
    );
    
}

ThreeAddressCode* generateAccessCode(Symbol* symbol, ThreeAddressCode* index, SymbolTable* table) {
    return joinCodes(
        index, 
        createCode(
            TACAccess, 
            symbol,
            index?index->result:NULL, 
            insertTemporary(table, symbol->dataType)
        )
    );
}

ThreeAddressCode* generateIf(ThreeAddressCode* c1, ThreeAddressCode* c2, SymbolTable* table) {
    ThreeAddressCode* jumpCode = NULL;
    ThreeAddressCode* labelCode = NULL;
    Symbol* label = NULL;
    label = insertLabel(table);
    jumpCode = createCode(TACJumpF, c1?c1->result:NULL, NULL, label);
    jumpCode->previous = c1;
    labelCode = createCode(TACLabel, NULL, NULL, label);
    labelCode->previous = c2;
    return joinCodes(jumpCode, labelCode);
}

ThreeAddressCode* generateIfElse(ThreeAddressCode* c1, ThreeAddressCode* c2, ThreeAddressCode* c3, SymbolTable* table) {
    ThreeAddressCode* elseLabelCode = NULL;
    ThreeAddressCode* elseJumpCode = NULL;
    ThreeAddressCode* ifJumpCode = NULL;
    ThreeAddressCode* ifLabelCode = NULL;

    Symbol* elseLabel = NULL;
    Symbol* ifLabel = NULL;
    elseLabel = insertLabel(table);
    ifLabel = insertLabel(table);


    elseJumpCode = createCode(TACJumpF, c1?c1->result:NULL, NULL, elseLabel);
    elseJumpCode->previous = c1;

    ifJumpCode = createCode(TACJump, NULL, NULL, ifLabel);
    ifJumpCode->previous = c2;

    elseLabelCode = createCode(TACLabel, NULL, NULL, elseLabel);
    
    ifLabelCode = createCode(TACLabel, NULL, NULL, ifLabel);
    ifLabelCode->previous = c3;

    return joinCodes(joinCodes(elseJumpCode, ifJumpCode), joinCodes(elseLabelCode, ifLabelCode));
}

ThreeAddressCode* generateWhile(ThreeAddressCode* c1, ThreeAddressCode* c2, SymbolTable* table) {
    ThreeAddressCode* loopLabelCode = NULL;
    ThreeAddressCode* loopJumpCode = NULL;
    ThreeAddressCode* ifJumpCode = NULL;
    ThreeAddressCode* ifLabelCode = NULL;

    Symbol* loopLabel = NULL;
    Symbol* ifLabel = NULL;
    loopLabel = insertLabel(table);
    ifLabel = insertLabel(table);

    loopLabelCode = createCode(TACLabel, NULL, NULL, loopLabel);
    loopLabelCode->previous = c1;

    ifJumpCode = createCode(TACJumpF, c1?c1->result:NULL, NULL, ifLabel);

    loopJumpCode = createCode(TACJump, NULL, NULL, loopLabel);
    loopJumpCode->previous = c2;
    
    ifLabelCode = createCode(TACLabel, NULL, NULL, ifLabel);

    return joinCodes(joinCodes(loopLabelCode, ifJumpCode), joinCodes(loopJumpCode, ifLabelCode));
}

ThreeAddressCode* generateFunctionCode(Symbol* symbol, ThreeAddressCode* c1) {
    ThreeAddressCode* functionStart = NULL;
    ThreeAddressCode* functionEnd = NULL;
    functionStart = createCode(TACBeginFun, NULL, NULL, symbol);
    functionEnd = createCode(TACEndFun, NULL, NULL, symbol);
    functionEnd->previous = c1;

    return joinCodes(functionStart, functionEnd);
}

ThreeAddressCode* reverseCode(ThreeAddressCode* last){
    ThreeAddressCode* ptr;
    for(ptr = last; ptr->previous; ptr = ptr->previous)
        ptr->previous->next = ptr;

    return ptr;
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
            case TACAccess:
                fprintf(stderr,"ACCESS");
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
            case TACJumpF:
                fprintf(stderr,"JUMPF");
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
