#include "AbstractSyntaxTree.h"

#include <stdlib.h>
#include <string.h>

char* _nodeTypeName(SyntaxNodeType type) {
    switch(type) {
        case AddNode:
            return "ADD";
        case SubNode:
            return "SUB";
        case ProdNode:
            return "PROD";
        case NegationNode:
            return "NEGATION";
        case LessNode:
            return "LESS THAN";
        case GreaterNode:
            return "GREATER THAN";
        case AndNode:
            return "AND";
        case OrNode:
            return "OR";
        case LessEqualNode:
            return "LESS THAN OR EQUAL";
        case GreaterEqualNode:
            return "GREATER THAN OR EQUAL";
        case EqualNode:
            return "EQUAL";
        case DifferentNode:
            return "DIFFERENT";
        case SymbolNode:
            return "SYMBOL";
        case VariableNode:
            return "VARIABLE";
        case ArrayNode:
            return "ARRAY";
        case FunctionNode:
            return "FUNCTION";
        case ListNode:
            return "LIST";
    }
    return "ERROR";
}

SyntaxTreeNode* createAST(SyntaxNodeType type, Symbol* symbol,
    SyntaxTreeNode* c1, SyntaxTreeNode* c2, SyntaxTreeNode* c3, SyntaxTreeNode* c4){

    SyntaxTreeNode* node = malloc(sizeof(SyntaxTreeNode));
    node->type = type;
    node->symbol = symbol;
    node->children[0] = c1;
    node->children[1] = c2;
    node->children[2] = c3;
    node->children[3] = c4;
    return node;
}

void printAST(SyntaxTreeNode* node, int level) {
    if(node != NULL) {
        for(int i=0; i<level; i++) fprintf(stderr," |");
        if(level>0) fprintf(stderr, "_");
        fprintf(stderr, _nodeTypeName(node->type));
        fprintf(stderr, " ");
        if(node->symbol != NULL) fprintf(stderr, "%s", node->symbol->name);
        fprintf(stderr, "\n");
        for(int i=0; i<4; i++){
            SyntaxTreeNode* child = node->children[i];
            printAST(child, level+1);
        }
    }
}

void _decompileExpression(SyntaxTreeNode* node, FILE* file) {
    switch(node->type){
        case SymbolNode:
        case VariableNode:
            _decompileSymbol(node, file);
            break;
        case ArrayNode:
            _decompileArray(node, file);
            break;
        case NegationNode:
            _decompileNegation(node, file);
            break;
        case AddNode:
            _decompileAdd(node, file);
            break;
        case SubNode:
            _decompileSub(node, file);
            break;
        case ProdNode:
            _decompileProd(node, file);
            break;
        case DivNode:
            _decompileDiv(node, file);
            break;
        case LessNode:
            _decompileLess(node, file);
            break;
        case GreaterNode:
            _decompileGreater(node, file);
            break;
        case AndNode:
            _decompileAnd(node, file);
            break;
        case OrNode:
            _decompileOr(node, file);
            break;
        case LessEqualNode:
            _decompileLessEqual(node, file);
            break;
        case GreaterEqualNode:
            _decompileGreaterEqual(node, file);
            break;
        case EqualNode:
            _decompileEqual(node, file);
            break;
        case DifferentNode:
            _decompileDifferent(node, file);
            break;
        case FunctionNode:
            //_decompileFunction(node, file);
            break;
    }
}

void _decompileArray(SyntaxTreeNode* node, FILE* file) {
    _decompileSymbol(node, file);
    fprintf(file, "[");
    _decompileExpression(node->children[0], file);
    fprintf(file, "]");
}

void _decompileSymbol(SyntaxTreeNode* node, FILE* file) {
    fprintf(file, node->symbol->name);
}

void _decompileAdd(SyntaxTreeNode* node, FILE* file) {
    _decompileExpression(node->children[0], file);
    fprintf(file, " + ");
    _decompileExpression(node->children[1], file);
}

void _decompileSub(SyntaxTreeNode* node, FILE* file) {
    _decompileExpression(node->children[0], file);
    fprintf(file, " - ");
    _decompileExpression(node->children[1], file);
}

void _decompileProd(SyntaxTreeNode* node, FILE* file) {
    _decompileExpression(node->children[0], file);
    fprintf(file, " . ");
    _decompileExpression(node->children[1], file);
}

void _decompileDiv(SyntaxTreeNode* node, FILE* file) {
    _decompileExpression(node->children[0], file);
    fprintf(file, " - ");
    _decompileExpression(node->children[1], file);
}

void _decompileLess(SyntaxTreeNode* node, FILE* file) {
    _decompileExpression(node->children[0], file);
    fprintf(file, " < ");
    _decompileExpression(node->children[1], file);
}

void _decompileGreater(SyntaxTreeNode* node, FILE* file) {
    _decompileExpression(node->children[0], file);
    fprintf(file, " > ");
    _decompileExpression(node->children[1], file);
}

void _decompileAnd(SyntaxTreeNode* node, FILE* file) {
    _decompileExpression(node->children[0], file);
    fprintf(file, " & ");
    _decompileExpression(node->children[1], file);
}

void _decompileOr(SyntaxTreeNode* node, FILE* file) {
    _decompileExpression(node->children[0], file);
    fprintf(file, " | ");
    _decompileExpression(node->children[1], file);
}

void _decompileNegation(SyntaxTreeNode* node, FILE* file) {
    fprintf(file, "~");
    _decompileExpression(node->children[0], file);
}

void _decompileLessEqual(SyntaxTreeNode* node, FILE* file) {
    _decompileExpression(node->children[0], file);
    fprintf(file, " <= ");
    _decompileExpression(node->children[1], file);
}

void _decompileGreaterEqual(SyntaxTreeNode* node, FILE* file) {
    _decompileExpression(node->children[0], file);
    fprintf(file, " >= ");
    _decompileExpression(node->children[1], file);
}

void _decompileEqual(SyntaxTreeNode* node, FILE* file) {
    _decompileExpression(node->children[0], file);
    fprintf(file, " == ");
    _decompileExpression(node->children[1], file);
}

void _decompileDifferent(SyntaxTreeNode* node, FILE* file) {
    _decompileExpression(node->children[0], file);
    fprintf(file, " != ");
    _decompileExpression(node->children[1], file);
}

void _decompileExpressionList(SyntaxTreeNode* node, FILE* file){
    if(node != NULL) {
        _decompileExpression(node->children[0], file);
        if(node->children[1] != NULL){
            fprintf(file, " ");
            _decompileExpressionList(node->children[1], file);
        }
    }
}

void _decompileFunction(SyntaxTreeNode* node, FILE* file) {
    fprintf(file, node->symbol->name);
    fprintf(file, "(");
    _decompileExpressionList(node->children[1], file);
    fprintf(file, ")");
}


void _decompileProgram(SyntaxTreeNode* node, FILE* file) {
    _decompileExpression(node, file);
}

void decompileAST(SyntaxTreeNode* node, char* filename) {
    FILE* file = fopen(filename, "w"); 
    _decompileProgram(node, file);
    fclose(file); 
}
