.PHONY: run

etapa1: lex.yy.c
		gcc -o etapa1 lex.yy.c HashTable.c

lex.yy.c: scanner.l
		lex scanner.l

clean:
		rm lex.yy.c etapa1

run: etapa1
		./etapa1
