#include "AssemblyGeneration.h"
#include <stdlib.h>
#include <string.h>

//Replace comma decimal separator with a dot
void replaceDecimalSeparator(char* number, char separator){
    char* comma = strchr(number, ','); 

    //strchr returns pointer to first occurence
    *comma = separator;
}

void replaceQuotationMarks(char* string){
    for(int i=0; i<strlen(string); i++){
        if(string[i] == '"' || string[i] == '\\') string[i] = '_';
    }
}

char* getLabelName(char* name, DataType type){
    char* copy = (char*) malloc(strlen(name)+2);
    strcpy(copy, name);

    if(type == DataTypeFloat) replaceDecimalSeparator(copy, '_');
    else if(type == DataTypeString) replaceQuotationMarks(copy);

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

void generateAssembly(ThreeAddressCode* first, SymbolTable* table){
    FILE* file;
    file = fopen("out.s", "w");
    
    
    //Start
    fprintf(file,
        "int_string:            \n"
        "	.string	\"%%d\"  \n"
        "float_string:          \n"
        "	.string \"%%f\"  \n"
        "char_string:           \n"
        "	.string \"%%c\"  \n"
        "text_string:           \n"
        "	.string \"%%s\"  \n"
    );

    //Each Three Address Code
    ThreeAddressCode* ptr;
    for(ptr = first; ptr; ptr = ptr->next){
        switch(ptr->type){
            case TACBeginFun:
                fprintf(file,
                    ".globl	%s            \n"
                    "%s:                  \n"
                    "pushq	%%rbp         \n"
                    "movq	%%rsp, %%rbp  \n",
                    ptr->result->name,
                    ptr->result->name
                );
                break;
            case TACEndFun:
                fprintf(file, 
                    "popq	%%rbp   \n"
                    "ret            \n"
                );
                break;
            case TACPrint:
                DataType type = ptr->result->dataType;
                char* label = getLabelName(ptr->result->name, type);

                const char* data;
                if(type == DataTypeString) data = label;
                else data = stringForData(type);
                fprintf(file, 
                    "leaq	%s(%%rip), %%rax    \n"
                    "movl	%s(%%rip), %%esi   \n"
                    "movq	%%rax, %%rdi        \n"
                    "call	printf@PLT          \n",
                    data,
                    label
                );
                free(label);
                break;
        }
    }
    
    //Hash Table
    generateSymbolTableAssembly(table, file);
    
    fclose(file);
}

void generateSymbolTableAssembly(SymbolTable* table, FILE* file){
    SymbolTableNode* node;
    for(int i=0; i<table->size; i++){
        for(node=table->table[i]; node; node = node->next){
            SymbolType symbolType = node->symbol->type;
            DataType dataType = node->symbol->dataType;

            //only variables and literals for now
            if(symbolType != SymbolFunction && symbolType != SymbolArray){
                char *name, *value, *type; 
                name    = (char*) malloc(strlen(node->symbol->name)+1);
                value   = (char*) malloc(strlen(node->symbol->name)+1);
                type    = (char*) malloc(7);

                //name
                strcpy(name, node->symbol->name);
                
                //value
                if(symbolType == SymbolVariable) value = strcpy(value, "0");
                else strcpy(value, name); //else is literal
                if(dataType == DataTypeFloat){
                    replaceDecimalSeparator(name, '_');
                    replaceDecimalSeparator(value, '.');
                    strcpy(type, "float");
                }
                else if(dataType == DataTypeString){
                    replaceQuotationMarks(name);
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