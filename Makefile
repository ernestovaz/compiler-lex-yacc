etapa6: y.tab.c lex.yy.c
	gcc -Wall -o etapa6 lex.yy.c main.c SymbolTable.c AbstractSyntaxTree.c Decompilation.c SemanticAnalysis.c IntermediaryCode.c AssemblyGeneration.c

y.tab.c: parser.y
	yacc -d parser.y

lex.yy.c: scanner.l
	lex --header-file=lex.yy.h scanner.l

.PHONY: run test
clean:
	rm -f lex.yy.c lex.yy.h y.tab.c y.tab.h etapa6

run: etapa6
	./etapa6

test: etapa6
	@echo
	for test_file in test_programs/*.code;\
	do\
		echo;\
		echo "TEST: $$test_file";\
		./etapa6 $$test_file || true;\
	done

