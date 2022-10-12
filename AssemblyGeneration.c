#include "AssemblyGeneration.h"
#include <stdlib.h>
#include <string.h>

//Replace comma decimal separator with a dot
void replaceDecimalSeparator(char* number, char separator){
    char* comma = strchr(number, ','); 

    //strchr returns pointer to first occurence
    if(comma) *comma = separator;
}

void reformatString(char* string){
    for(int i=0; i<strlen(string); i++){
        if(string[i] == '"' || string[i] == '\\' || string[i] == ' ') string[i] = '_';
    }
}

char* getLabelName(char* name, DataType type){
    char* copy = (char*) malloc(strlen(name)+2);
    strcpy(copy, name);

    if(type == DataTypeFloat) replaceDecimalSeparator(copy, '_');
    else if(type == DataTypeString) reformatString(copy);

    memmove(copy + 1, copy, strlen(copy)+1); 
    copy[0] = '_';

    return copy;
}

const char* stringForData(DataType type){
    switch(type){
        case DataTypeInt:
            return "int_string";
        case DataTypeFloat:
            return "float_string";
        case DataTypeChar:
            return "char_string";
        case DataTypeString:
            return "text_string";
    }
    return "";
}

void generateMoveAssembly(ThreeAddressCode* code, FILE* file){
    char *var1, *var2;
    var1 = getLabelName(code->operator1->name, code->operator1->dataType);
    var2 = getLabelName(code->result->name, code->result->dataType);
    fprintf(file, 
        "#assignment                \n"
        "movl	%s(%%rip), %%edx    \n"
        "movl	%%edx, %s(%%rip)    \n"
        "                           \n",
        var1,
        var2
    );
    free(var1);
    free(var2);
}

void generateAddAssembly(ThreeAddressCode* code, FILE* file){
    char *var1, *var2, *var3;
    var1 = getLabelName(code->operator1->name, code->operator1->dataType);
    var2 = getLabelName(code->operator2->name, code->operator2->dataType);
    var3 = getLabelName(code->result->name, code->result->dataType);
    fprintf(file, 
        "#add                       \n"
        "movl	%s(%%rip), %%edx    \n"
        "movl 	%s(%%rip), %%eax    \n"
        "addl 	%%edx, %%eax        \n"
        "movl	%%eax, %s(%%rip)    \n"
        "                           \n",
        var1,
        var2,
        var3
    );
    free(var1);
    free(var2);
    free(var3);
}

void generateSubAssembly(ThreeAddressCode* code, FILE* file){
    char *var1, *var2, *var3;
    var1 = getLabelName(code->operator1->name, code->operator1->dataType);
    var2 = getLabelName(code->operator2->name, code->operator2->dataType);
    var3 = getLabelName(code->result->name, code->result->dataType);
    fprintf(file, 
        "#subtract                  \n"
        "movl	%s(%%rip), %%eax    \n"
        "movl 	%s(%%rip), %%edx    \n"
        "subl 	%%edx, %%eax        \n"
        "movl	%%eax, %s(%%rip)    \n"
        "                           \n",
        var1,
        var2,
        var3
    );
    free(var1);
    free(var2);
    free(var3);
}

void generateProdAssembly(ThreeAddressCode* code, FILE* file){
    char *var1, *var2, *var3;
    var1 = getLabelName(code->operator1->name, code->operator1->dataType);
    var2 = getLabelName(code->operator2->name, code->operator2->dataType);
    var3 = getLabelName(code->result->name, code->result->dataType);
    fprintf(file, 
        "#multiplication            \n"
        "movl	%s(%%rip), %%eax    \n"
        "movl 	%s(%%rip), %%edx    \n"
        "imull 	%%edx, %%eax        \n"
        "movl	%%eax, %s(%%rip)    \n"
        "                           \n",
        var1,
        var2,
        var3
    );
    free(var1);
    free(var2);
    free(var3);
}

void generateDivAssembly(ThreeAddressCode* code, FILE* file){
    char *var1, *var2, *var3;
    var1 = getLabelName(code->operator1->name, code->operator1->dataType);
    var2 = getLabelName(code->operator2->name, code->operator2->dataType);
    var3 = getLabelName(code->result->name, code->result->dataType);
    fprintf(file, 
        "#division                  \n"
        "movl	%s(%%rip), %%eax    \n"
        "movl 	%s(%%rip), %%ecx    \n"
        "cltd                       \n"
        "idivl 	%%ecx               \n"
        "movl	%%eax, %s(%%rip)    \n"
        "                           \n",
        var1,
        var2,
        var3
    );
    free(var1);
    free(var2);
    free(var3);
}

void generatePrintAssembly(char* label, DataType type, FILE* file){
    const char* data;
    if(type == DataTypeString) data = label;
    else data = stringForData(type);

    if(type != DataTypeFloat){
        fprintf(file, 
            "#print                     \n"
            "leaq	%s(%%rip), %%rax    \n"
            "movl	%s(%%rip), %%esi    \n"
            "movq	%%rax, %%rdi        \n"
            "call	printf@PLT          \n"
            "                           \n",
            data,
            label
        );
    } else {
        fprintf(file, 
            "#load floating point               \n"
            "movss	%s(%%rip), %%xmm0           \n"
            "                                   \n"
            "#print floating point              \n"
            "cvtss2sd %%xmm0, %%xmm1            \n"
            "movq	%%xmm1, %%rax               \n"
            "movq	%%rax, %%xmm0               \n"
            "leaq	%s(%%rip), %%rax            \n"
            "movq	%%rax, %%rdi                \n"
            "call	printf@PLT                  \n"
            "                                   \n",
            label,
            data
        );
    }
}

void generateAssembly(ThreeAddressCode* first, SymbolTable* table){
    FILE* file;
    file = fopen("out.s", "w");
    
    
    //Start
    fprintf(file,
        "#template strings      \n"
        "int_string:            \n"
        "	.string	\"%%d\"     \n"
        "float_string:          \n"
        "	.string \"%%f\"     \n"
        "char_string:           \n"
        "	.string \"%%c\"     \n"
        "text_string:           \n"
        "	.string \"%%s\"     \n"
        "                       \n"
    );

    //Each Three Address Code
    ThreeAddressCode* ptr;
    for(ptr = first; ptr; ptr = ptr->next){
        switch(ptr->type){
            case TACBeginFun:
                fprintf(file,
                    "#function start      \n"
                    ".globl	%s            \n"
                    "%s:                  \n"
                    "pushq	%%rbp         \n"
                    "movq	%%rsp, %%rbp  \n"
                    "                     \n",
                    ptr->result->name,
                    ptr->result->name
                );
                break;
            case TACEndFun:
                fprintf(file, 
                    "#function end  \n"
                    "popq	%%rbp   \n"
                    "ret            \n"
                    "               \n"
                );
                break;
            case TACPrint:
                DataType type = ptr->result->dataType;
                char* label = getLabelName(ptr->result->name, type);
                generatePrintAssembly(label, type, file);
                free(label);
                break;
            case TACAdd:
                generateAddAssembly(ptr, file);
                break;
            case TACMove:
                generateMoveAssembly(ptr, file);
                break;
            case TACSub:
                generateSubAssembly(ptr, file);
                break;
            case TACMul:
                generateProdAssembly(ptr, file);
                break;
            case TACDiv:
                generateDivAssembly(ptr,file);
                break;
        }
    }
    
    //Hash Table
    generateSymbolTableAssembly(table, file);
    
    fclose(file);
}

void generateSymbolTableAssembly(SymbolTable* table, FILE* file){
    SymbolTableNode* node;
    fprintf(file, "#symbol table labels \n.data\n");
    for(int i=0; i<table->size; i++){
        for(node=table->table[i]; node; node = node->next){
            SymbolType symbolType = node->symbol->type;
            DataType dataType = node->symbol->dataType;

            //only variables and literals for now
            if(symbolType != SymbolFunction && symbolType != SymbolArray){
                char *name, *value, *type; 
                name    = (char*) malloc(strlen(node->symbol->name)+1);
                type    = (char*) malloc(7);

                //name
                strcpy(name, node->symbol->name);
                
                //value
                if(symbolType == SymbolVariable){
                    if(node->symbol->initialValue){
                        char* initialValue = node->symbol->initialValue->name;
                        value   = (char*) malloc(strlen(initialValue)+1);
                        value = strcpy(value, initialValue);  
                    } else {
                        value   = (char*) malloc(2);
                        value = strcpy(value, "0");
                    }
                } else { //else is literal
                    value   = (char*) malloc(strlen(name)+1);
                    strcpy(value, name); 
                } 
                if(dataType == DataTypeFloat){
                    replaceDecimalSeparator(name, '_');
                    replaceDecimalSeparator(value, '.');
                    strcpy(type, "float");
                }
                else if(dataType == DataTypeString){
                    reformatString(name);
                    strcpy(type, "string");
                } else{
                    strcpy(type, "long");
                }

                fprintf(file,
                    "_%s:   .%s   %s\n",
                    name,
                    type,
                    value
                );

                free(name);
                free(value);
                free(type);
            }
        }
    }
}