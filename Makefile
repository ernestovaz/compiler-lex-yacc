.PHONY: run

etapa2: y.tab.c lex.yy.c
		gcc -o etapa2 lex.yy.c SymbolTable.c

y.tab.c: parser.y
	yacc -d parser.y

lex.yy.c: scanner.l
		lex scanner.l

.PHONY: run test
clean:
		rm -f lex.yy.c lex.yy.h y.tab.c etapa2

run: etapa2
		./etapa2

test: etapa2
		@echo 
		./etapa2 programs/correct.code || true
		@echo 
		./etapa2 programs/incorrect.code || true
