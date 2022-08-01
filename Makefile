.PHONY: run

etapa3: y.tab.c lex.yy.c
		gcc -o etapa3 lex.yy.c SymbolTable.c AbstractSyntaxTree.c

y.tab.c: parser.y
	yacc -d parser.y

lex.yy.c: scanner.l
		lex scanner.l

.PHONY: run test
clean:
		rm -f lex.yy.c lex.yy.h y.tab.c etapa3

run: etapa3
		./etapa3

test: etapa3
		@echo 
		./etapa3 programs/correct.code || true
		@echo 
		./etapa3 programs/incorrect.code || true
