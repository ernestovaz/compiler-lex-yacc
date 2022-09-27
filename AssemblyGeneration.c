#include "AssemblyGeneration.h"

void generateAssembly(ThreeAddressCode* first){
    FILE* file;
    file = fopen("out.s", "w");
    
    
    //Start
    fprintf(file,
        "number_string:         \n"
        "	.string	\"%%d\\n\"  \n"
        "text_string:           \n"
        "	.string \"%%s\\n\"  \n"
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
        }
    }
    
    //Hash Table
    
    fclose(file);
}