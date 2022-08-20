etapa4: y.tab.c lex.yy.c
	gcc -o etapa4 lex.yy.c main.c SymbolTable.c AbstractSyntaxTree.c Decompilation.c

y.tab.c: parser.y
	yacc -d parser.y

lex.yy.c: scanner.l
	lex --header-file=lex.yy.h scanner.l

.PHONY: run test
clean:
	rm -f lex.yy.c lex.yy.h y.tab.c y.tab.h etapa4

run: etapa4
	./etapa4

test: etapa4
	./etapa4 programs/correct.code
