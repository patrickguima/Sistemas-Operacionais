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




void teste_basico(){
	DESCRIBE("Teste simples");

	IF("Abro um arquivo no modo leitura e tento escrever nele");
	THEN("Deve falhar");
	CLEANUP("testes/entrada");
	indice_arq_t f = f_open("testes/entrada",LEITURA);
	char buffer[1];
	buffer[0] = 'D';
	isEqual(f_write(f,1,buffer),FALHA,1);
	f_close(f);

	IF("Agora um arquivo no modo escrita e tento ler ele");
	THEN("Deve falhar");
	f = f_open("testes/entrada",ESCRITA);
	isEqual(f_read(f,1,buffer),FALHA,1);
	f_close(f);
}


void teste_normal(){
	int i;

	DESCRIBE("Testes de operação sem falhas");

	IF("Inicializo um arquivo");
	THEN("Deve ler valor original");
	CLEANUP("testes/512A");
	if (initfs("testes/512A")==SUCESSO){
		indice_arq_t f = f_open("testes/512A",LEITURA);
		char buffer[512];
		if (f != FALHA){
			if (f_read(f, 512, buffer)==512){
				for(i=0; (i<512) && (buffer[i]=='A'); i++);
				isEqual(i,512,1); // para antes se não for B e falha
			} else isEqual(0,1,1);
			f_close(f);
		} else isEqual(0,1,1);
	} else isEqual(0,1,1);

	IF("Mando escrever um bloco no final do arquivo");
	THEN("Deve ler dado original e o dado novo");
	CLEANUP("testes/512A");
	if (initfs("testes/512A")==SUCESSO){
		indice_arq_t f = f_open("testes/512A",LEITURAESCRITA);
		char buffer[1024];
		char buffer2[512];
		for(i=0;i<512;i++){
			buffer2[i] = 'D';
		}
		f_seek(f,512);
		f_write(f,512,buffer2);
		f_seek(f,0);
		if (f != FALHA){
			if (f_read(f, 1024, buffer)==1024){
				for(i=0; (i<512) && (buffer[i]=='A'); i++);
				isEqual(i,512,1);
				for(i=512; (i<1024) && (buffer[i]=='D'); i++);
				isEqual(i,1024,1);
			} else isEqual(0,1,1);
			f_close(f);
		} else isEqual(0,1,1);
	} else isEqual(0,1,1);




	IF("Mando escrever bloco novo no meio");
	THEN("Dever ler A, ler B e ler D");
	CLEANUP("testes/512A");
	if (initfs("testes/512A")==SUCESSO){
		indice_arq_t f = f_open("testes/512A",LEITURAESCRITA);
		char buffer[1536];
		char buffer3[512];
		char buffer2[512];
		for(i=0;i<512;i++){
			buffer2[i] = 'D';
			buffer3[i] = 'B';
		}
		f_seek(f,512);
		f_write(f,512,buffer2);
		f_seek(f,512);
		f_write(f,512,buffer3);
		f_seek(f,0);
		if (f != FALHA){
			if (f_read(f, 1536, buffer)==1536){
				for(i=0; (i<512) && (buffer[i]=='A'); i++);
				isEqual(i,512,1);
				for(i=512; (i<1024) && (buffer[i]=='B'); i++);
				isEqual(i,1024,1);
				for(i=1024; (i<1536) && (buffer[i]=='D'); i++);
				isEqual(i,1536,1);
			} else isEqual(0,1,1);
			f_close(f);
		} else isEqual(0,1,1);
	} else isEqual(0,1,1);




}



void teste_recover(){
	int i;
	DESCRIBE("Testes de operação com falhas");

	IF("Inicializo um arquivo e apaga um bloco");
	THEN("Deve ler valor original");
	CLEANUP("testes/entrada");
	if (initfs("testes/entrada")==SUCESSO){
		remove((const char *)"testes/entrada.bin3");		
		recover("testes/entrada");
		indice_arq_t f = f_open("testes/entrada",LEITURA);
		char buffer[8];
		f_read(f, 8, buffer);
			for(i=0; (i<4) && (buffer[i]=='A'); i++);
			isEqual(i,4,1); 
			for(i=4; (i<8) && (buffer[i]=='B'); i++);
			isEqual(i,8,1); 
		
	}
	IF("Inicializo um arquivo e apago dois blocos");
	THEN("Como não tem como recuperar, deve falhar");
	CLEANUP("testes/entrada");
	if (initfs("testes/entrada")==SUCESSO){
		remove((const char *)"testes/entrada.bin1");
		remove((const char *)"testes/entrada.bin2");		
		if(recover("testes/entrada")==FALHA){
			isEqual(1,1,1);
		}else{
			isEqual(0,1,1);
		}
		
	}

}
void teste_blocos_incompletos (){

	int i;
	IF("Inicializo um arquivo e insiro 100 bytes no meio do bloco");
	THEN("Deve ler valor original, os 100 bytes e o resto do valor original");
	CLEANUP("testes/512A");
	if (initfs("testes/512A")==SUCESSO){
		indice_arq_t f = f_open("testes/512A",LEITURAESCRITA);
		char buffer[1024];
		char buffer2[100];
		char buffer3[100];
		for(i=0;i<100;i++){
			buffer2[i]='D';
			buffer3[i]='E';
		}
		f_seek(f,256);
		f_write(f,100,buffer2);
		f_seek(f,0);
			if (f_read(f,612, buffer)==612){
				for(i=0; (i<256) && (buffer[i]=='A'); i++);
				isEqual(i,256,1);
				for(i=256; (i<356) && (buffer[i]=='D'); i++);
				isEqual(i,356,1); 
				for(i=356; (i<612) && (buffer[i]=='A'); i++);
				isEqual(i,612,1);  // para antes se não for B e falha
			} else isEqual(0,1,1);

			IF("Insiro outros 100 bytes no meio do bloco e excluiu o .bin2");
			THEN("recover deve funcionar, recuperar o bin2 e ler o valor total");
			f_seek(f,306);
			f_write(f,100,buffer3);
			f_seek(f,0);
			remove((const char *)"testes/512A.bin2");		
			recover("testes/512A");
			if (f_read(f,712, buffer)==712){
				for(i=0; (i<256) && (buffer[i]=='A'); i++);
				isEqual(i,256,1);
				for(i=256; (i<306) && (buffer[i]=='D'); i++);
				isEqual(i,306,1);
				for(i=306; (i<406) && (buffer[i]=='E'); i++);
				isEqual(i,406,1);
				for(i=406; (i<456) && (buffer[i]=='D'); i++);
				isEqual(i,456,1); 
				for(i=456; (i<712) && (buffer[i]=='A'); i++);
				isEqual(i,712,1);  
			} else isEqual(0,1,1);
			f_close(f);
	} else isEqual(0,1,1);


}


int main(){

	teste_basico();
	teste_normal();
	teste_blocos_incompletos();
	teste_recover();

	GRADEME();

	if (grade==maxgrade)
		return 0;
	else return grade;
}
