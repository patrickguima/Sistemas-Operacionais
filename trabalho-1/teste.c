#include "roteador.h"
#include "simplegrade.h"
#include <assert.h>
#include <math.h>

uint32_t iptoint(int a, int b, int c, int d){
	return (uint32_t) (a<<24)+(b<<16)+(c<<8)+d;
}

void test1(){

	entrada rotas[2] = { {iptoint(5,4,0,0), 16, 1},
						 {iptoint(123,0,0,0), 16, 2} };


	uint32_t pacotes[4] = { iptoint(5,4,0,1),iptoint(5,4,0,0),iptoint(123,0,10,1), iptoint(123,0,1,1) };

	entrada filtros[2] = {{iptoint(5,4,0,0),24,0},{iptoint(123,0,0,0),16,0}};

	uint32_t *result;
	WHEN("Tenho duas rotas diferentes e dois filtros");
	IF("Envio quatro pacotes");

	result = roteamento(rotas,2,pacotes,4,filtros,2,2);

	assert(result);
	THEN("Devo ter descartado todos pacotes	");
	isEqual(result[0], 4, 1); 

	THEN("Não devo ter nenhum pacote em cada enlace");
	for(int i=1; i<=2; i++){
		isEqual(result[i], 0, 1);
	}
	free(result);

}


void test2(){


	entrada rotas[4] = { {iptoint(10,0,0,1), 8, 1},
						 {iptoint(100,4,0,1), 16, 2},
						 {iptoint(19,20,21,22), 24, 3},
						 {iptoint(5,4,0,1), 24, 4} };

	uint32_t pacotes[4] = { iptoint(10,0,0,1), iptoint(100,4,0,1),iptoint(19,20,21,22),iptoint(5,4,0,1) };
	uint32_t * result;

	entrada filtros[1] = {{iptoint(0,0,0,0),24,0}};

	WHEN("Tenho quatro rotas diferentes");
	IF("Envio um pacote para cada rota");


	result = roteamento(rotas, 4, pacotes, 4,filtros,0, 4);
	assert(result);
	THEN("Não devo ter descartado nenhum pacote");
	isEqual(result[0], 0, 1); 

	THEN("Devo ter um pacote em cada enlace");
	for(int i=1; i<=4; i++){
		isEqual(result[i], 1, 1);
	}
	free(result);

}
void test3(){
	entrada rotas[2] = { {iptoint(10,9,0,0), 24, 1},
						 {iptoint(10,9,1,0), 16, 2} };

	uint32_t pacotes[2] = { iptoint(10,9,1,0), iptoint(10,9,0,0) };
	uint32_t * result;
	entrada filtros[1] = {{iptoint(0,0,0,0),24,0}};
	WHEN("Tenho duas rotas iguais até certo ponto ");
	IF("Envio um pacote para cada rota");
	result = roteamento(rotas, 2, pacotes, 2,filtros,0,2);
	assert(result);
	THEN("Não devo ter descartado nenhum pacote");
	isEqual(result[0], 0, 1);
	THEN("Devo ter um pacote em cada enlace");
	for(int i=1; i<=2; i++){
		isEqual(result[i], 1, 1);
	}
	free(result);

}

void test4(){
	entrada rotas[2] = { {iptoint(10,9,0,0), 24, 1},
						 {iptoint(5,4,0,0), 16, 2 }};

	uint32_t pacotes[4] = { iptoint(10,9,0,0),iptoint(10,0,1,0), iptoint(5,4,1,1),iptoint(5,0,1,0)};
	uint32_t * result;
	entrada filtros[1] = {{iptoint(10,0,0,0),16,0}};
	WHEN("Tenho duas rotas e um filtro");
	IF("Envio quatro pacotes");
	result = roteamento(rotas, 2, pacotes, 4,filtros,1,2);
	assert(result);
	THEN("Devo ter descartado dois pacotes");
	isEqual(result[0], 2, 1);
	THEN("Devo ter um pacote em cada enlace");
	for(int i=1; i<=2; i++){
		isEqual(result[i], 1, 1);
	}
	free(result);

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