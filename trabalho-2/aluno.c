#include "simplegrade.h"
#include "memvirt.h"
#include <assert.h>

#pragma GCC diagnostic ignored "-Wwrite-strings"


void test1(){
	struct result * res;

	DESCRIBE("Teste com 4 processos e uma entrada grande");
	IF("20 frames total");
	res = memvirt(4, 20, "alunos/t02.txt");
	isNotNull(res,1);
	if (res){
		THEN("Deve possuir 36 referências  e 2 page faults no processo 0");
		isEqual(res->refs[0],36,1);
		isEqual(res->pfs[0],2,1);
		THEN("Deve possuir  48 referências e 2 page faults no processo 1");
		isEqual(res->refs[1],48,1);
		isEqual(res->pfs[1],2,1);
		THEN("Deve possuir 84 referências  e 4 page faults no processo 2");
		isEqual(res->refs[2],84,1);
		isEqual(res->pfs[2],4,1);
		THEN("Deve possuir  48 referências e 3 page faults no processo 3");
		isEqual(res->refs[3],48,1);
		isEqual(res->pfs[3],3,1);
		THEN("Deve possuir  page fault total bem baixo (6)");
		isEqual(res->total_pf_rate,0.06,1);

		free(res);
	}
	else isNotNull(res,4);



	DESCRIBE("Teste com 1 frame por processo");
	IF("4 frames total");
	res = memvirt(4, 4, "alunos/t02.txt");
	isNotNull(res,1);
	if (res){
		THEN("Deve possuir 24 page faults no processo 0");
		isEqual(res->pfs[0],24,1);
		THEN("Deve possuir 24 page faults no processo 1");
		isEqual(res->pfs[1],24,1);
		THEN("Deve possuir 84 page faults no processo 2");
		isEqual(res->pfs[2],84,1);
		THEN("Deve possuir 37 page faults no processo 3");
		isEqual(res->pfs[3],37,1);
				free(res);
	}
	else isNotNull(res,4);

	DESCRIBE("Teste com 2 frames por processo");
	IF("8 frames total");
	res = memvirt(4, 8, "alunos/t02.txt");
	isNotNull(res,1);
	if (res){
		THEN("Deve possuir 2 page faults no processo 0");
		isEqual(res->pfs[0],2,1);
		THEN("Deve possuir 2 page faults no processo 1");
		isEqual(res->pfs[1],2,1);
		THEN("Deve possuir 72 page faults no processo 2");
		isEqual(res->pfs[2],72,1);
		THEN("Deve possuir 37 page faults no processo 3");
		isEqual(res->pfs[3],37,1);
				free(res);
	}
	else isNotNull(res,4);

}

void test2(){
	struct result * res;

	DESCRIBE("Teste com 4 processos");

	IF("1 frame por processo");
	res = memvirt(4, 4, "alunos/t01.txt");
	isNotNull(res,1);
	if (res){
		THEN("Deve ter 50% de page fault para P0");
		isEqual(res->pf_rate[0],0.5,1);
		THEN("Deve ter 75% de page fault para P1");
		isEqual(res->pf_rate[1],0.75,1);
		THEN("Deve ter 50% de page fault para P2");
		isEqual(res->pf_rate[2],0.5,1);
		THEN("Deve ter 25% de page fault para P3");
		isEqual(res->pf_rate[3],0.25,1);
		THEN("Deve ter 50% de page fault Total");
		isEqual(res->total_pf_rate,0.5,1);
		free(res);
	}
	else isNotNull(res,2);


	
}


int main(){
	test1();
	test2();
	GRADEME();

	if (grade==maxgrade)
		return 0;
	else return grade;
}
