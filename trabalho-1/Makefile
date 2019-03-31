CC=gcc

CFLAGS=-Wall -Wextra -Werror -O0 -g -std=c11 -pthread
LDFLAGS=-lm -Wno-missing-braces

all:   teste_aluno grade gradet 

aluno: roteamento.c teste.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o aluno roteamento.c teste.c -lm 

test: roteamento.c test.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o test roteamento.c test.c -lm 

testt: roteamentot.c test.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o testt roteamentot.c test.c -lm 


grade: test
	./test

gradet: testt
	./testt

teste_aluno: aluno
	./aluno

clean:
	rm -rf *.o test testt aluno
