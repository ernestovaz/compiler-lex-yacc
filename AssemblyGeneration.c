#include "AssemblyGeneration.h"
#include <stdlib.h>
#include <string.h>

int g_functionNumber = 1;
int g_labelCount = 1;

//Replace comma decimal separator with a dot
void replaceDecimalSeparator(char* number, char separator){
    char* comma = strchr(number, ','); 

    //strchr returns pointer to first occurence
    if(comma) *comma = separator;
}

void reformatString(char* string){
    for(int i=0; i<strlen(string); i++){
        if(string[i] == '"' || string[i] == '\\' || string[i] == ' ' || string[i] == '!' || string[i] == ':' || string[i] == '=') 
            string[i] = '_';
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
        case DataTypeBool:
        case DataTypeInt:
            return "int_string";
        case DataTypeFloat:
            return "float_string";
        case DataTypeChar:
            return "char_string";
        case DataTypeString:
            return "text_string";
        default:
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
    DataType type = code->result->dataType;
    fprintf(file, "#add\n");
    if(type == DataTypeFloat){
        fprintf(file,        
            "movss	%s(%%rip), %%xmm1    \n"
            "movss 	%s(%%rip), %%xmm0    \n"
            "addss	%%xmm1, %%xmm0       \n"
            "movss 	%%xmm0, %s(%%rip)    \n",
            var1,
            var2,
            var3
        );
    }
    else{
        fprintf(file, 
            "movl	%s(%%rip), %%edx    \n"
            "movl 	%s(%%rip), %%eax    \n"
            "addl 	%%edx, %%eax        \n"
            "movl	%%eax, %s(%%rip)    \n",
            var1,
            var2,
            var3
        );
    }
    fprintf(file, "\n");
    free(var1);
    free(var2);
    free(var3);
}

void generateSubAssembly(ThreeAddressCode* code, FILE* file){
    char *var1, *var2, *var3;
    var1 = getLabelName(code->operator1->name, code->operator1->dataType);
    var2 = getLabelName(code->operator2->name, code->operator2->dataType);
    var3 = getLabelName(code->result->name, code->result->dataType);
    DataType type = code->operator1->dataType;
    fprintf(file, "#subtract\n");
    if(type == DataTypeFloat){
        fprintf(file,
            "movss	%s(%%rip), %%xmm0   \n"
            "movss 	%s(%%rip), %%xmm1   \n"
            "subss 	%%xmm1, %%xmm0      \n"
            "movss 	%%xmm0, %s(%%rip)   \n",
            var1,
            var2,
            var3
        );
    }
    else {
        fprintf(file, 
            "movl	%s(%%rip), %%eax    \n"
            "movl 	%s(%%rip), %%edx    \n"
            "subl 	%%edx, %%eax        \n"
            "movl	%%eax, %s(%%rip)    \n",
            var1,
            var2,
            var3
        );
    }
    fprintf(file, "\n");
    free(var1);
    free(var2);
    free(var3);
}

void generateProdAssembly(ThreeAddressCode* code, FILE* file){
    char *var1, *var2, *var3;
    var1 = getLabelName(code->operator1->name, code->operator1->dataType);
    var2 = getLabelName(code->operator2->name, code->operator2->dataType);
    var3 = getLabelName(code->result->name, code->result->dataType);
    DataType type = code->operator1->dataType;
    fprintf(file, "#multiplication\n");
    if(type == DataTypeFloat){
        fprintf(file,
            "movss	%s(%%rip), %%xmm0   \n"
            "movss 	%s(%%rip), %%xmm1   \n"
            "mulss 	%%xmm1, %%xmm0      \n"
            "movss 	%%xmm0, %s(%%rip)   \n",
            var1,
            var2,
            var3
        );
    }
    else {
        fprintf(file, 
            "movl	%s(%%rip), %%eax    \n"
            "movl 	%s(%%rip), %%edx    \n"
            "imull 	%%edx, %%eax        \n"
            "movl	%%eax, %s(%%rip)    \n",
            var1,
            var2,
            var3
        );
    }
    fprintf(file, "\n");
    free(var1);
    free(var2);
    free(var3);
}

void generateDivAssembly(ThreeAddressCode* code, FILE* file){
    char *var1, *var2, *var3;
    var1 = getLabelName(code->operator1->name, code->operator1->dataType);
    var2 = getLabelName(code->operator2->name, code->operator2->dataType);
    var3 = getLabelName(code->result->name, code->result->dataType);
    DataType type = code->operator1->dataType;
    fprintf(file, "#division\n");
    if(type == DataTypeFloat){
        fprintf(file,
            "movss	%s(%%rip), %%xmm0   \n"
            "movss 	%s(%%rip), %%xmm1   \n"
            "divss 	%%xmm1, %%xmm0      \n"
            "movss 	%%xmm0, %s(%%rip)   \n",
            var1,
            var2,
            var3
        );
    }
    else {
        fprintf(file, 
            "movl	%s(%%rip), %%eax    \n"
            "movl 	%s(%%rip), %%ecx    \n"
            "cltd                       \n"
            "idivl 	%%ecx               \n"
            "movl	%%eax, %s(%%rip)    \n",
            var1,
            var2,
            var3
        );
    }
    fprintf(file, "\n");
    free(var1);
    free(var2);
    free(var3);
}

void generatePrintAssembly(char* label, DataType type, FILE* file){
    const char* data;
    if(type == DataTypeString) data = label;
    else data = stringForData(type);

    if(type == DataTypeFloat){
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
    } else {
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
    }
}

void generateReadAssembly(char* label, DataType type, FILE* file){
    const char* data;
    if(type == DataTypeString) data = label;
    else data = stringForData(type);

    fprintf(file, 
    "#read                      \n"
    "leaq	%s(%%rip), %%rax    \n"
    "movq	%%rax, %%rsi        \n"
    "leaq	%s(%%rip), %%rax    \n"
    "movq	%%rax, %%rdi        \n"
    "call	__isoc99_scanf@PLT  \n"
    "                           \n",
    label,
    data
    );
}

void generateCallAssembly(ThreeAddressCode* code, FILE* file){
    fprintf(file,
        "#function call       \n"
        "movl $0, %%eax       \n"
        "call %s              \n",
        code->operator1->name
    );
    if(code->result){
        char* label = getLabelName(code->result->name, code->result->dataType);
        fprintf(file,
            "movl %%eax, %s(%%rip)\n",
            label
        );
        free(label);
    }
    fprintf(file,
        "movl $0, %%eax       \n"
        "                     \n"
    );
}


void generateReturnAssembly(ThreeAddressCode* code, FILE* file) {
    char* label = getLabelName(code->result->name, code->result->dataType);
    fprintf(file,
        "#return                \n"
        "movl %s(%%rip), %%eax  \n"
        "jmp .END%d             \n"
        "                       \n",
        label,
        g_functionNumber
    );
    free(label);
}

void generateGreaterAssembly(ThreeAddressCode* code, FILE* file) {
    char *var1, *var2, *var3;
    var1 = getLabelName(code->operator1->name, code->operator1->dataType);
    var2 = getLabelName(code->operator2->name, code->operator2->dataType);
    var3 = getLabelName(code->result->name, code->result->dataType);
    DataType type = code->operator1->dataType;
    fprintf(file,
        "#greater than       \n"
        "movl $0, %s(%%rip)  \n", 
        var3
    );
    if(type == DataTypeFloat){
        fprintf(file,
            "movss %s(%%rip), %%xmm0\n"
            "movss %s(%%rip), %%xmm1\n"
            "comiss %%xmm0, %%xmm1  \n"
            "jbe .L%d             \n",
            var2,
            var1,
            g_labelCount
        );
    } else {
        fprintf(file,
            "movl %s(%%rip), %%eax\n"
            "movl %s(%%rip), %%edx\n"
            "cmpl %%eax, %%edx    \n"
            "jle .L%d             \n",
            var2,
            var1,
            g_labelCount
        );
    }
    fprintf(file,
        "movl $1, %s(%%rip)  \n"
        ".L%d:               \n"  
        "                    \n",
        var3,
        g_labelCount
    );
    g_labelCount++;
    free(var1);
    free(var2);
    free(var3);
}

void genereateIfAssembly(ThreeAddressCode* code, FILE* file){
    char *var1, *var2;
    var1 = getLabelName(code->operator1->name, code->operator1->dataType);
    var2 = getLabelName(code->result->name, code->result->dataType);
    fprintf(file,
        "#if                    \n"
        "movl %s(%%rip), %%eax  \n"
        "testl %%eax, %%eax     \n"
        "je %s                  \n"
        "                       \n",
        var1,
        var2
    );
    free(var1);
    free(var2);
}

void generateLabelAssembly(ThreeAddressCode* code, FILE* file){
    char *var1;
    var1 = getLabelName(code->result->name, code->result->dataType);
    fprintf(file,
        "#if label         \n"
        "%s:               \n"
        "                  \n",
        var1
    );
    free(var1);
}

void generateArrayAccessAssembly(ThreeAddressCode* code, FILE* file){
    char *var1, *var2, *var3;
    var1 = getLabelName(code->operator1->name, code->operator1->dataType);
    var2 = getLabelName(code->operator2->name, code->operator2->dataType);
    var3 = getLabelName(code->result->name, code->result->dataType);
    DataType type = code->operator1->dataType;
    fprintf(file,
        "#array access                 \n"
        "movl	%s(%%rip), %%eax       \n"
        "cltq                          \n"
        "leaq	0(,%%rax,4), %%rdx     \n"
        "leaq	%s(%%rip), %%rax       \n",
        var2, 
        var1
    );
    if(type == DataTypeFloat) {
        fprintf(file,
            "movss	(%%rdx,%%rax), %%xmm0   \n"
            "movss	%%xmm0, %s(%%rip)       \n",
            var3
        );
    } else {
        fprintf(file,
            "movl	(%%rdx,%%rax), %%eax   \n"
            "movl	%%eax, %s(%%rip)       \n",
            var3
        );
    }
    fprintf(file, "\n");
    free(var1);
    free(var2);
    free(var3);
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
                    "#function end      \n"
                    ".END%d:  \n"
                    "popq	%%rbp       \n"
                    "ret                \n"
                    "                   \n",
                    g_functionNumber
                );
                g_functionNumber++;
                break;
            case TACPrint:{
                DataType type = ptr->result->dataType;
                char* label = getLabelName(ptr->result->name, type);
                generatePrintAssembly(label, type, file);
                free(label);
            }
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
            case TACRead:{
                DataType type = ptr->result->dataType;
                char* label = getLabelName(ptr->result->name, type);
                generateReadAssembly(label, type, file);
                free(label);
            }
                break;
            case TACCall:
                generateCallAssembly(ptr, file);
                break;
            case TACRet:
                generateReturnAssembly(ptr, file);
                break;
            case TACGreater:
                generateGreaterAssembly(ptr, file);
                break;
            case TACJumpF:
                genereateIfAssembly(ptr, file);
                break;
            case TACLabel:
                generateLabelAssembly(ptr, file);
                break;
            case TACAccess:
                generateArrayAccessAssembly(ptr, file);
                break;
            default:
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

            if(symbolType != SymbolLabel && symbolType != SymbolFunction){
                if(symbolType == SymbolArray){
                    char *name, *type; 
                    name    = (char*) malloc(strlen(node->symbol->name)+1);
                    type    = (char*) malloc(7);

                    //name
                    strcpy(name, node->symbol->name);
                    //type
                    if(dataType == DataTypeFloat) strcpy(type, "float");
                    else strcpy(type, "long");

                    fprintf(file, "_%s:\n", name);
                    for(int i=0; i<node->symbol->arraySize; i++){
                        Symbol* literal = node->symbol->arrayValues[i];
                        char* value;
                        value = (char*) malloc(strlen(literal->name)+1);
                        strcpy(value, literal->name); 
                        if(dataType == DataTypeFloat) 
                            replaceDecimalSeparator(value, '.');
                            
                        fprintf(file, "    .%s %s\n", type, value);
                    }
                } 
                else {
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
}