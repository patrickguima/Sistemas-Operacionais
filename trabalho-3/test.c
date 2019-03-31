#include "raidfs.h"
#include "simplegrade.h"
#include <stdio.h>


inline void CLEANUP(const char * arquivo){
	int i;
	for(i=1;i<=3;i++){
		char nome[1024];
		sprintf(nome,"%s.bin%d",arquivo,i);
		remove((const char *)nome);		
	}
	
}




void test_init(){
	DESCRIBE("Teste de inicialização de um arquivo");

	IF("Crio um arquivo raidfs a partir de um arquivo inexistente");
	THEN("Deve falhar");

	isEqual(initfs("EUNAOEXISTO"),FALHA,1);

	IF("Crio um arquivo raidfs a partir de um arquivo existente de 1 byte");
	THEN("Deve retornar sucesso");
	CLEANUP("onebyte");
	if(initfs("onebyte")==SUCESSO){
		indice_arq_t f = f_open("onebyte",LEITURA);
		char buffer[1];
		if (f != FALHA){
			if (f_read(f, 1, buffer)==1){
				if (buffer[0]=='A'){
					isEqual(1,1,1);
				}
				else isEqual(0,1,1);
			} else isEqual(0,1,1);
			f_close(f);
		} else isEqual(0,1,1);

	} else isEqual(0,1,1);

}



void test_operacao_normal(){
	int i;

	DESCRIBE("Testes de operação sem falhas");

	IF("Crio um arquivo com dois blocos e leio");
	THEN("Deve ler valor original");
	CLEANUP("1024B");
	if (initfs("1024B")==SUCESSO){
		indice_arq_t f = f_open("1024B",LEITURA);
		char buffer[1024];
		if (f != FALHA){
			if (f_read(f, 1024, buffer)==1024){
				for(i=0; (i<1024) && (buffer[i]=='B'); i++);
				isEqual(i,1024,1); // para antes se não for B e falha
			} else isEqual(0,1,1);
			f_close(f);
		} else isEqual(0,1,1);
	} else isEqual(0,1,1);


	IF("Crio um arquivo e sobreescrevo dados");
	THEN("Dados devem ser lidos sem problemas");
	CLEANUP("1024B");
	if (initfs("1024B")==SUCESSO){
		indice_arq_t f = f_open("1024B",LEITURAESCRITA);
		char buffer[1024];
		for(i=0; i<1024; i++) 
			buffer[i] = 'C';
		if (f != FALHA){
			f_seek(f, 0);
			f_write(f, 1024, buffer);
			f_close(f);
			f = f_open("1024B",LEITURA);
			if (f_read(f, 1024, buffer)==1024){
				for(i=0; (i<1024) && (buffer[i]=='C'); i++);
				isEqual(i,1024,1); // para antes se não for C e falha
			} else isEqual(0,1,1);
			f_close(f);
		} else isEqual(0,1,1);
	} else isEqual(0,1,1);

	IF("Crio um arquivo e adiciono dados");
		THEN("Dados devem ser lidos sem problemas");
		CLEANUP("1024B");
		if (initfs("1024B")==SUCESSO){
			indice_arq_t f = f_open("1024B",LEITURAESCRITA);
			char buffer[2048];
			for(i=0; i<2048; i++) 
				buffer[i] = 'C';
			if (f != FALHA){
				f_seek(f, 1024);
				f_write(f, 1024, buffer);
				f_close(f);
				f = f_open("1024B",LEITURA);
				if (f_read(f, 1024, buffer)==1024){
					for(i=0; (i<1024) && (buffer[i]=='B'); i++);
					isEqual(i,1024,1); // para antes se não for B e falha
				} else isEqual(0,1,1);
				if (f_read(f, 1024, buffer)==1024){
					for(i=0; (i<1024) && (buffer[i]=='C'); i++);
					isEqual(i,1024,1); // para antes se não for B e falha
				} else isEqual(0,1,1);
				f_close(f);
			} else isEqual(0,1,2);
		} else isEqual(0,1,2);

		IF("Crio um arquivo e adiciono dados byte a byte");
		THEN("Dados devem ser lidos sem problemas");
		CLEANUP("1024B");
		if (initfs("1024B")==SUCESSO){
			indice_arq_t f = f_open("1024B",LEITURAESCRITA);
			char buffer[2048];
			buffer[0]='D';
			if (f != FALHA){
				f_seek(f, 1024);
				for(i=0; i<1024; i++){
					f_write(f,1,buffer);
				}
				f_close(f);
				f = f_open("1024B",LEITURA);
				if (f_read(f, 1024, buffer)==1024){
					for(i=0; (i<1024) && (buffer[i]=='B'); i++);
					isEqual(i,1024,1); // para antes se não for B e falha
				} else isEqual(0,1,1);
				if (f_read(f, 1024, buffer)==1024){
					for(i=0; (i<1024) && (buffer[i]=='D'); i++);
					isEqual(i,1024,1); // para antes se não for B e falha
				} else isEqual(0,1,1);
				f_close(f);
			} else isEqual(0,1,2);
		} else isEqual(0,1,2);

}



void test_operacao_com_falha(){
	int i;
	DESCRIBE("Testes de operação com falhas");

	IF("Crio um arquivo com dois blocos, apago um e leio");
	THEN("Deve ler valor original");

	CLEANUP("1024B");
	if (initfs("1024B")==SUCESSO){
		remove((const char *)"1024B.bin1");		
		recover("1024B");
		indice_arq_t f = f_open("1024B",LEITURA);
		char buffer[1024];
		if (f != FALHA){
			if (f_read(f, 1024, buffer)==1024){
				for(i=0; (i<1024) && (buffer[i]=='B'); i++);
				isEqual(i,1024,1); // para antes se não for B e falha
			} else isEqual(0,1,1);
			f_close(f);
		} else isEqual(0,1,1);
	} else isEqual(0,1,1);

	CLEANUP("1024B");
	if (initfs("1024B")==SUCESSO){
		remove((const char *)"1024B.bin2");		
		recover("1024B");
		indice_arq_t f = f_open("1024B",LEITURA);
		char buffer[1024];
		if (f != FALHA){
			if (f_read(f, 1024, buffer)==1024){
				for(i=0; (i<1024) && (buffer[i]=='B'); i++);
				isEqual(i,1024,1); // para antes se não for B e falha
			} else isEqual(0,1,1);
			f_close(f);
		} else isEqual(0,1,1);
	} else isEqual(0,1,1);

	CLEANUP("1024B");
	if (initfs("1024B")==SUCESSO){
		remove((const char *)"1024B.bin3");		
		recover("1024B");
		indice_arq_t f = f_open("1024B",LEITURA);
		char buffer[1024];
		if (f != FALHA){
			if (f_read(f, 1024, buffer)==1024){
				for(i=0; (i<1024) && (buffer[i]=='B'); i++);
				isEqual(i,1024,1); // para antes se não for B e falha
			} else isEqual(0,1,1);
			f_close(f);
		} else isEqual(0,1,1);
	} else isEqual(0,1,1);

}


int main(){

	test_init();
	test_operacao_normal();
	test_operacao_com_falha();

	GRADEME();

	if (grade==maxgrade)
		return 0;
	else return grade;
}