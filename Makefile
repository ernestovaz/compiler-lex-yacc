etapa4: y.tab.c lex.yy.c
	gcc -o etapa4 lex.yy.c main.c SymbolTable.c AbstractSyntaxTree.c Decompilation.c SemanticAnalysis.c

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
	@echo
	for test_file in programs/*.code;\
	do\
		echo;\
		echo "TEST: $$test_file";\
		./etapa4 $$test_file || true;\
	done

