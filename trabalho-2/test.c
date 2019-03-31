#include "simplegrade.h"
#include "memvirt.h"


#pragma GCC diagnostic ignored "-Wwrite-strings"

void test1(){
	struct result * res;

	DESCRIBE("Testes de parametros invalidos");
	IF("Nome do arquivo eh NULL");
	THEN("Deve retornar NULL");
	res = memvirt(1, 1, NULL);
	isNull(res,1);
	if (res)
		free(res);

	IF("Numero de frames ou processos eh zero");
	THEN("Deve retornar NULL");
	res = memvirt(0, 1, "teste.txt");
	isNull(res,1);
	if (res)
		free(res);	
	res = memvirt(1, 0, "teste.txt");
	isNull(res,1);
	if (res)
		free(res);	


	IF("Numero de frames eh menor que processos");
	THEN("Deve retornar NULL");
	res = memvirt(11, 10, "teste.txt");
	isNull(res,1);
	if (res)
		free(res);	
}


void test2(){
	struct result * res;

	DESCRIBE("Testes simples com 3 processos");
	IF("Um frame por processo");
	res = memvirt(3, 3, "t2.txt");
	isNotNull(res,1);
	if (res){
		THEN("Deve ter 100% de page fault para P0");
		isEqual(res->refs[0],4,1);
		isEqual(res->pfs[0],4,1);
		THEN("Demais processos devem ter 50% de page fault");
		isEqual(res->pfs[1],1,1);
		isEqual(res->pfs[2],1,1);
		
		free(res);
	}
	else isNotNull(res,4);

}

void test3(){
	struct result * res;

	DESCRIBE("Teste de um programa simples");

	IF("1 frame por processo");
	res = memvirt(1, 1, "t3b.txt");
	isNotNull(res,1);
	if (res){
		THEN("Deve ter 99,7% de page fault para P0");
		isEqual(res->refs[0],1000,1);
		isEqual(res->pfs[0],997,1);

		free(res);
	}
	else isNotNull(res,2);


	IF("2 frames por processo");
	res = memvirt(1, 2, "t3b.txt");
	isNotNull(res,1);
	if (res){
		THEN("Deve ter 99,7% de page fault para P0");
		isEqual(res->refs[0],1000,1);
		isEqual(res->pfs[0],997,1);

		free(res);
	}
	else isNotNull(res,2);

    IF("5 frames por processo");
    res = memvirt(1, 5, "t3b.txt");
    isNotNull(res,1);
    if (res){
            THEN("Deve ter 1,5% de page fault para P0");
            isEqual(res->refs[0],1000,2);
            isEqual(res->pfs[0],15,2);

            free(res);
    }
    else isNotNull(res,4);

    IF("6 frames por processo");
    res = memvirt(1, 6, "t3b.txt");
    isNotNull(res,1);
    if (res){
            THEN("Deve ter 0,6% de page fault para P0");
            isEqual(res->refs[0],1000,2);
            isEqual(res->pfs[0],6,2);

            free(res);
    }
    else isNotNull(res,4);




	IF("100 frames por processo");
	res = memvirt(1, 10, "t3b.txt");
	isNotNull(res,1);
	if (res){
		THEN("Deve ter 0,6% de page fault para P0");
		isEqual(res->refs[0],1000,2);
		isEqual(res->pfs[0],6,2);
		
		free(res);
	}
	else isNotNull(res,4);
}


void test4(){
	struct result * res;

	DESCRIBE("Multiply and Add");

	IF("1 frame por processo");
	res = memvirt(1, 1, "m2k.txt");
	isNotNull(res,1);
	if (res){
		THEN("Deve ter 99,7% de page fault para P0");
		isEqual(res->refs[0],2000,1);
		isEqual(res->pfs[0],1989,1);

		free(res);
	}
	else isNotNull(res,2);


	IF("2 frames por processo");
	res = memvirt(1, 2, "m2k.txt");
	isNotNull(res,1);
	if (res){
		THEN("Deve ter 99,7% de page fault para P0");
		isEqual(res->pfs[0],1989,2);

		free(res);
	}
	else isNotNull(res,2);

	IF("3 frames por processo");
	res = memvirt(1, 3, "m2k.txt");
	isNotNull(res,1);
	if (res){
		THEN("Deve ter 16,7% de page fault para P0");
		isEqual(res->pfs[0],334,2);

		free(res);
	}
	else isNotNull(res,2);

	IF("4 frames por processo");
	res = memvirt(1, 5, "m2k.txt");
	isNotNull(res,1);
	if (res){
		THEN("Deve ter 0,85% de page fault para P0");
		isEqual(res->pfs[0],170,2);

		free(res);
	}
	else isNotNull(res,2);

	IF("1000 frames por processo");
	res = memvirt(1, 1000, "m2k.txt");
	isNotNull(res,1);
	if (res){
		THEN("Deve ter só os compulsórios");
		isEqual(res->pfs[0],18,2);

		free(res);
	}
	else isNotNull(res,2);


	IF("10000 frames por processo");
	res = memvirt(1, 10000, "m2k.txt");
	isNotNull(res,1);
	if (res){
		THEN("Não deveria ser um problema...");
		isEqual(res->pfs[0],18,2);

		free(res);
	}
	else isNotNull(res,2);
}



int main(){
	test1();
	test2();
	test3();
	test4();

	GRADEME();

	if (grade==maxgrade)
		return 0;
	else return grade;

}
