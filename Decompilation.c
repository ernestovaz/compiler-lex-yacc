#include "Decompilation.h"

#include "Symbol.h"

void decompileProgram(SyntaxTreeNode* node, FILE* file) {
    decompileDefinitionList(node, file);
}

void decompileDefinitionList(SyntaxTreeNode* node, FILE* file) {
    if(node != NULL) {
        decompileDefinition(node->children[0], file);
        if(node->children[1] != NULL){
            fprintf(file, "\n");
            decompileDefinitionList(node->children[1], file);
        }
    }
}

void decompileDefinition(SyntaxTreeNode* node, FILE* file) {
    switch(node->type){
        case FunctionDefNode:
            decompileFunctionDef(node, file);
            break;
        case VariableDefNode:
            decompileVariableDef(node, file);
            break;
        case ArrayDefNode:
            decompileArrayDef(node, file);
            break;
    }
}

void decompileExpression(SyntaxTreeNode* node, FILE* file) {
    switch(node->type){
        case ParenthesesNode:
            decompileParentheses(node, file);
            break;
        case SymbolNode:
        case VariableNode:
            decompileSymbol(node, file);
            break;
        case ArrayNode:
            decompileArray(node, file);
            break;
        case NegationNode:
            decompileNegation(node, file);
            break;
        case AddNode:
            decompileAdd(node, file);
            break;
        case SubNode:
            decompileSub(node, file);
            break;
        case ProdNode:
            decompileProd(node, file);
            break;
        case DivNode:
            decompileDiv(node, file);
            break;
        case LessNode:
            decompileLess(node, file);
            break;
        case GreaterNode:
            decompileGreater(node, file);
            break;
        case AndNode:
            decompileAnd(node, file);
            break;
        case OrNode:
            decompileOr(node, file);
            break;
        case LessEqualNode:
            decompileLessEqual(node, file);
            break;
        case GreaterEqualNode:
            decompileGreaterEqual(node, file);
            break;
        case EqualNode:
            decompileEqual(node, file);
            break;
        case DifferentNode:
            decompileDifferent(node, file);
            break;
        case FunctionNode:
            decompileFunction(node, file);
            break;
    }
}

void decompileExpressionList(SyntaxTreeNode* node, FILE* file){
    if(node != NULL) {
        decompileExpression(node->children[0], file);
        if(node->children[1] != NULL){
            fprintf(file, " ");
            decompileExpressionList(node->children[1], file);
        }
    }
}

void decompileArray(SyntaxTreeNode* node, FILE* file) {
    decompileSymbol(node, file);
    fprintf(file, "[");
    decompileExpression(node->children[0], file);
    fprintf(file, "]");
}

void decompileSymbol(SyntaxTreeNode* node, FILE* file) {
    fprintf(file, node->symbol->name);
}

void decompileParentheses(SyntaxTreeNode* node, FILE* file) {
    fprintf(file, "(");
    if(node->children[0] != NULL) decompileExpression(node->children[0], file);
    fprintf(file, ")");
}

void decompileAdd(SyntaxTreeNode* node, FILE* file) {
    decompileExpression(node->children[0], file);
    fprintf(file, " + ");
    decompileExpression(node->children[1], file);
}

void decompileSub(SyntaxTreeNode* node, FILE* file) {
    decompileExpression(node->children[0], file);
    fprintf(file, " - ");
    decompileExpression(node->children[1], file);
}

void decompileProd(SyntaxTreeNode* node, FILE* file) {
    decompileExpression(node->children[0], file);
    fprintf(file, " . ");
    decompileExpression(node->children[1], file);
}

void decompileDiv(SyntaxTreeNode* node, FILE* file) {
    decompileExpression(node->children[0], file);
    fprintf(file, " - ");
    decompileExpression(node->children[1], file);
}

void decompileLess(SyntaxTreeNode* node, FILE* file) {
    decompileExpression(node->children[0], file);
    fprintf(file, " < ");
    decompileExpression(node->children[1], file);
}

void decompileGreater(SyntaxTreeNode* node, FILE* file) {
    decompileExpression(node->children[0], file);
    fprintf(file, " > ");
    decompileExpression(node->children[1], file);
}

void decompileAnd(SyntaxTreeNode* node, FILE* file) {
    decompileExpression(node->children[0], file);
    fprintf(file, " & ");
    decompileExpression(node->children[1], file);
}

void decompileOr(SyntaxTreeNode* node, FILE* file) {
    decompileExpression(node->children[0], file);
    fprintf(file, " | ");
    decompileExpression(node->children[1], file);
}

void decompileNegation(SyntaxTreeNode* node, FILE* file) {
    fprintf(file, "~");
    decompileExpression(node->children[0], file);
}

void decompileLessEqual(SyntaxTreeNode* node, FILE* file) {
    decompileExpression(node->children[0], file);
    fprintf(file, " <= ");
    decompileExpression(node->children[1], file);
}

void decompileGreaterEqual(SyntaxTreeNode* node, FILE* file) {
    decompileExpression(node->children[0], file);
    fprintf(file, " >= ");
    decompileExpression(node->children[1], file);
}

void decompileEqual(SyntaxTreeNode* node, FILE* file) {
    decompileExpression(node->children[0], file);
    fprintf(file, " == ");
    decompileExpression(node->children[1], file);
}

void decompileDifferent(SyntaxTreeNode* node, FILE* file) {
    decompileExpression(node->children[0], file);
    fprintf(file, " != ");
    decompileExpression(node->children[1], file);
}

void decompileFunction(SyntaxTreeNode* node, FILE* file) {
    fprintf(file, node->symbol->name);
    fprintf(file, "(");
    decompileExpressionList(node->children[0], file);
    fprintf(file, ")");
}

void decompileType(SyntaxTreeNode* node, FILE* file) {
    switch (node->type) {
        case FloatTypeNode:
            fprintf(file, "float");
            break;
        case IntTypeNode:
            fprintf(file, "int");
            break;
        case CharTypeNode:
            fprintf(file, "char");
            break;
    }
}

void decompileVariableDef(SyntaxTreeNode* node, FILE* file) {
    decompileType(node->children[0], file);
    fprintf(file, " ");
    decompileSymbol(node, file);
    fprintf(file, "(");
    decompileSymbol(node->children[1], file);
    fprintf(file, ");");
}

void decompileArrayDef(SyntaxTreeNode* node, FILE* file) {
    decompileType(node->children[0], file);
    fprintf(file, " ");
    decompileSymbol(node, file);
    fprintf(file, "[");
    decompileSymbol(node->children[1], file);
    fprintf(file, "] ");
    decompileLiteralList(node->children[2], file);
    fprintf(file, ";");
}

void decompileLiteralList(SyntaxTreeNode* node, FILE* file) {
    if(node != NULL) {
        decompileSymbol(node->children[0], file);
        if(node->children[1] != NULL){
            fprintf(file, " ");
            decompileLiteralList(node->children[1], file);
        }
    }
}

void decompileAssignment(SyntaxTreeNode* node, FILE* file) {
    decompileSymbol(node->children[0], file);
    fprintf(file, " <- ");
    decompileExpression(node->children[1], file);
}

void decompileRead(SyntaxTreeNode* node, FILE* file) {
    fprintf(file, "read ");
    switch(node->children[0]->type) {
        case VariableNode:
            decompileSymbol(node->children[0], file);
            break;
        case ArrayNode:
            decompileArray(node->children[0], file);
            break;
    }
}

void decompileReturn(SyntaxTreeNode* node, FILE* file) {
    fprintf(file, "return ");
    decompileExpression(node->children[0], file);
}

void decompileIf(SyntaxTreeNode* node, FILE* file) {
    fprintf(file, "if(");
    decompileExpression(node->children[0], file);
    fprintf(file, ")");
    decompileCommand(node->children[1], file);
}

void decompileIfElse(SyntaxTreeNode* node, FILE* file) {
    fprintf(file, "if(");
    decompileExpression(node->children[0], file);
    fprintf(file, ")");
    decompileCommand(node->children[1], file);
    fprintf(file, " else ");
    decompileCommand(node->children[2], file);
}

void decompileWhile(SyntaxTreeNode* node, FILE* file) {
    fprintf(file, "while(");
    decompileExpression(node->children[0], file);
    fprintf(file, ")");
    decompileCommand(node->children[1], file);
}

void decompilePrint(SyntaxTreeNode* node, FILE* file) {
    fprintf(file, "print ");
    decompilePrintList(node->children[0], file);
}

void decompilePrintList(SyntaxTreeNode* node, FILE* file) {
    if(node != NULL) {
        if(node->symbol) decompileSymbol(node, file);
        else decompileExpression(node->children[0], file);
        if(node->children[1] != NULL){
            fprintf(file, " ");
            decompilePrintList(node->children[1], file);
        }
    }
}

void decompileCommand(SyntaxTreeNode* node, FILE* file) {
    if(node != NULL) {
        switch(node->type) {
            case CommandBlockNode:
                decompileCommandBlock(node, file);
                break;
            case AssignmentNode:
                decompileAssignment(node, file);
                break;
            case ReadNode:
                decompileRead(node, file);
                break;
            case ReturnNode:
                decompileReturn(node, file);
                break;
            case PrintNode:
                decompilePrint(node, file);
                break;
            case IfNode:
                decompileIf(node, file);
                break;
            case IfElseNode:
                decompileIfElse(node, file);
                break;
            case WhileNode:
                decompileWhile(node, file);
                break;
        }
    }
}

void decompileCommandList(SyntaxTreeNode* node, FILE* file) {
    if(node != NULL) {
        decompileCommand(node->children[0], file);
        if(node->children[1] != NULL){
            fprintf(file, ";\n");
            decompileCommandList(node->children[1], file);
        }
    }
}

void decompileCommandBlock(SyntaxTreeNode* node, FILE* file) {
    fprintf(file, "{\n");
    decompileCommandList(node->children[0], file);
    fprintf(file, "\n}");
}

void decompileParameterList(SyntaxTreeNode* node, FILE* file){
    if(node != NULL) {
        decompileType(node->children[0], file);
        fprintf(file, " ");
        decompileSymbol(node, file);
        if(node->children[1] != NULL){
            fprintf(file, " ");
            decompileParameterList(node->children[1], file);
        }
    }
}


void decompileFunctionDef(SyntaxTreeNode* node, FILE* file) {
    decompileType(node->children[0], file);
    fprintf(file, " ");
    decompileSymbol(node, file);
    fprintf(file, "(");
    decompileParameterList(node->children[1], file);
    fprintf(file, ")");
    decompileCommandBlock(node->children[2], file);
}


void decompileAST(SyntaxTreeNode* node, char* filename) {
    FILE* file = fopen(filename, "w"); 
    decompileProgram(node, file);
    fclose(file); 
}
