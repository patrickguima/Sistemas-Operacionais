CC=gcc

CFLAGS=-Wall -Wextra -Werror -O1 -g -std=c99 
LDFLAGS=-lm -Wno-missing-braces

all:  aluno grade 

teste_aluno: raidfs.c aluno.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o teste_aluno raidfs.c aluno.c -lm 

test: raidfs.c test.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o test raidfs.c test.c -lm 


grade: test
	./test

aluno: teste_aluno
	./teste_aluno

clean:
	rm -rf *.o test  aluno
