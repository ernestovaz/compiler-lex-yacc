#include "AbstractSyntaxTree.h"

#include <stdlib.h>
#include <string.h>

char* _nodeTypeName(SyntaxNodeType type) {
    switch(type) {
        case ParenthesesNode:
            return "PARENTHESES";
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
        case VariableDefNode:
            return "VARIABLE DEFINITION";
        case ArrayDefNode:
            return "ARRAY DEFINITION";
        case ArrayNode:
            return "ARRAY";
        case FunctionNode:
            return "FUNCTION";
        case ExpressionListNode:
            return "EXPRESSION LIST ELEMENT";
        case AssignmentNode:
            return "ASSIGNMENT";
        case ReadNode:
            return "READ";
        case ReturnNode:
            return "RETURN";
        case PrintNode:
            return "PRINT";
        case PrintListNode:
            return "PRINT LIST ELEMENT";
        case IfNode:
            return "IF";
        case IfElseNode:
            return "IF ELSE";
        case WhileNode:
            return "WHILE";
        case CommandBlockNode:
            return "COMMAND BLOCK";
        case CommandListNode:
            return "COMMAND LIST";
        case LiteralListNode:
            return "LITERAL LIST ELEMENT";
        case IntTypeNode:
            return "INT TYPE";
        case CharTypeNode:
            return "CHAR TYPE";
        case FloatTypeNode:
            return "FLOAT TYPE";
        case DefinitionListNode:
            return "DEFINITION LIST ELEMENT";
        case FunctionDefNode:
            return "FUNCTION DEFINITION";
        case ParameterListNode:
            return "PARAMETER LIST ELEMENT";
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
        case ParenthesesNode:
            _decompileParentheses(node, file);
            break;
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
            _decompileFunction(node, file);
            break;
    }
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

void _decompileArray(SyntaxTreeNode* node, FILE* file) {
    _decompileSymbol(node, file);
    fprintf(file, "[");
    _decompileExpression(node->children[0], file);
    fprintf(file, "]");
}

void _decompileSymbol(SyntaxTreeNode* node, FILE* file) {
    fprintf(file, node->symbol->name);
}

void _decompileParentheses(SyntaxTreeNode* node, FILE* file) {
    fprintf(file, "(");
    if(node->children[0] != NULL) _decompileExpression(node->children[0], file);
    fprintf(file, ")");
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

void _decompileFunction(SyntaxTreeNode* node, FILE* file) {
    fprintf(file, node->symbol->name);
    fprintf(file, "(");
    _decompileExpressionList(node->children[0], file);
    fprintf(file, ")");
}

void _decompileType(SyntaxTreeNode* node, FILE* file) {
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

void _decompileVariableDef(SyntaxTreeNode* node, FILE* file) {
    _decompileType(node->children[0], file);
    fprintf(file, " ");
    _decompileSymbol(node, file);
    fprintf(file, "(");
    _decompileSymbol(node->children[1], file);
    fprintf(file, ");");
}

void _decompileArrayDef(SyntaxTreeNode* node, FILE* file) {
    _decompileType(node->children[0], file);
    fprintf(file, " ");
    _decompileSymbol(node, file);
    fprintf(file, "[");
    _decompileSymbol(node->children[1], file);
    fprintf(file, "] ");
    _decompileLiteralList(node->children[2], file);
    fprintf(file, ";");
}

void _decompileLiteralList(SyntaxTreeNode* node, FILE* file) {
    if(node != NULL) {
        _decompileSymbol(node->children[0], file);
        if(node->children[1] != NULL){
            fprintf(file, " ");
            _decompileLiteralList(node->children[1], file);
        }
    }
}

void _decompileAssignment(SyntaxTreeNode* node, FILE* file) {
    _decompileSymbol(node->children[0], file);
    fprintf(file, " <- ");
    _decompileExpression(node->children[1], file);
}

void _decompileRead(SyntaxTreeNode* node, FILE* file) {
    fprintf(file, "read ");
    switch(node->children[0]->type) {
        case VariableNode:
            _decompileSymbol(node->children[0], file);
            break;
        case ArrayNode:
            _decompileArray(node->children[0], file);
            break;
    }
}

void _decompileReturn(SyntaxTreeNode* node, FILE* file) {
    fprintf(file, "return ");
    _decompileExpression(node->children[0], file);
}

void _decompileIf(SyntaxTreeNode* node, FILE* file) {
    fprintf(file, "if(");
    _decompileExpression(node->children[0], file);
    fprintf(file, ")");
    _decompileCommand(node->children[1], file);
}

void _decompileIfElse(SyntaxTreeNode* node, FILE* file) {
    fprintf(file, "if(");
    _decompileExpression(node->children[0], file);
    fprintf(file, ")");
    _decompileCommand(node->children[1], file);
    fprintf(file, " else ");
    _decompileCommand(node->children[2], file);
}

void _decompileWhile(SyntaxTreeNode* node, FILE* file) {
    fprintf(file, "while(");
    _decompileExpression(node->children[0], file);
    fprintf(file, ")");
    _decompileCommand(node->children[1], file);
}

void _decompilePrint(SyntaxTreeNode* node, FILE* file) {
    fprintf(file, "print ");
    _decompilePrintList(node->children[0], file);
}

void _decompilePrintList(SyntaxTreeNode* node, FILE* file) {
    if(node != NULL) {
        if(node->symbol) _decompileSymbol(node, file);
        else _decompileExpression(node->children[0], file);
        if(node->children[1] != NULL){
            fprintf(file, " ");
            _decompilePrintList(node->children[1], file);
        }
    }
}

void _decompileCommand(SyntaxTreeNode* node, FILE* file) {
    if(node != NULL) {
        switch(node->type) {
            case CommandBlockNode:
                _decompileCommandBlock(node, file);
                break;
            case AssignmentNode:
                _decompileAssignment(node, file);
                break;
            case ReadNode:
                _decompileRead(node, file);
                break;
            case ReturnNode:
                _decompileReturn(node, file);
                break;
            case PrintNode:
                _decompilePrint(node, file);
                break;
            case IfNode:
                _decompileIf(node, file);
                break;
            case IfElseNode:
                _decompileIfElse(node, file);
                break;
            case WhileNode:
                _decompileWhile(node, file);
                break;
        }
    }
}

void _decompileCommandList(SyntaxTreeNode* node, FILE* file) {
    if(node != NULL) {
        _decompileCommand(node->children[0], file);
        if(node->children[1] != NULL){
            fprintf(file, ";\n");
            _decompileCommandList(node->children[1], file);
        }
    }
}

void _decompileCommandBlock(SyntaxTreeNode* node, FILE* file) {
    fprintf(file, "{\n");
    _decompileCommandList(node->children[0], file);
    fprintf(file, "\n}");
}

void _decompileParameterList(SyntaxTreeNode* node, FILE* file){
    if(node != NULL) {
        _decompileType(node->children[0], file);
        fprintf(file, " ");
        _decompileSymbol(node, file);
        if(node->children[1] != NULL){
            fprintf(file, " ");
            _decompileParameterList(node->children[1], file);
        }
    }
}


void _decompileFunctionDef(SyntaxTreeNode* node, FILE* file) {
    _decompileType(node->children[0], file);
    fprintf(file, " ");
    _decompileSymbol(node, file);
    fprintf(file, "(");
    _decompileParameterList(node->children[1], file);
    fprintf(file, ")");
    _decompileCommandBlock(node->children[2], file);
}

void _decompileDefinition(SyntaxTreeNode* node, FILE* file) {
    switch(node->type){
        case FunctionDefNode:
            _decompileFunctionDef(node, file);
            break;
        case VariableDefNode:
            _decompileVariableDef(node, file);
            break;
        case ArrayDefNode:
            _decompileArrayDef(node, file);
            break;
    }
}

void _decompileDefinitionList(SyntaxTreeNode* node, FILE* file) {
    if(node != NULL) {
        _decompileDefinition(node->children[0], file);
        if(node->children[1] != NULL){
            fprintf(file, "\n");
            _decompileDefinitionList(node->children[1], file);
        }
    }
}

void _decompileProgram(SyntaxTreeNode* node, FILE* file) {
    _decompileDefinitionList(node, file);
}

void decompileAST(SyntaxTreeNode* node, char* filename) {
    FILE* file = fopen(filename, "w"); 
    _decompileProgram(node, file);
    fclose(file); 
}
