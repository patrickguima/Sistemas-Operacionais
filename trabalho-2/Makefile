CC=gcc
CXX=g++

CFLAGS=-g -O1 -std=c99 -pthread -lm -Wall -Wextra -Werror
CXXFLAGS=-g -O1 -lm -Wall -Wextra -Werror -std=c++11

all: run_aluno grade 

memvirt.o: memvirt.cpp

aluno.o: aluno.c

test.o: test.c

aluno: memvirt.o aluno.o 
	$(CXX) $(CXXFLAGS) memvirt.cpp aluno.c -o aluno

test: memvirt.o test.o
	$(CXX) $(CXXFLAGS) memvirt.cpp test.c -o test

run_aluno: aluno
	./aluno

grade: test
	./test

clean:
	rm -rf *.o test aluno