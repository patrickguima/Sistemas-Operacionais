#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "roteador.h"

typedef struct Fila fila;
struct Fila{
	fila *next;
	entrada estado;
	fila *prev;
};
fila *insereFiltros(fila *L,entrada H){
	fila *auxFila= malloc(sizeof(fila));
	auxFila->estado=H;
	auxFila->prev=NULL;
	auxFila->next=L;
	L=auxFila;

	return L;
}

fila *inicializaFila(fila *L, entrada H){
	L->next = NULL;
	L->prev = NULL;
	L->estado = H;
	return L;
}

fila *insereFila(fila *L,entrada H){

	if(L==NULL){
		L = malloc(sizeof(fila));
		L = inicializaFila(L,H);
		return L;
	}

	fila *auxFila2=L;
	fila *auxFila=malloc(sizeof(fila));
	auxFila->estado=H;
	
	while(1){
		if(H.mascara > L->estado.mascara){
			auxFila->estado = L->estado;
			auxFila->prev = L;
			auxFila ->next = L->next;
			L->next = auxFila;
			L->estado = H;
			return auxFila2;
		}else{
			if(L->next==NULL){
				L->next=auxFila;
				auxFila->next=NULL;
				auxFila->prev=L;
				return auxFila2;
			}
		}
		
		L=L->next;
	}
		
	return auxFila2;
}


void limpaFila(fila *L){
	fila *aux;
	while(L!=NULL){
		aux=L->next;
		free(L);
		L=aux;
	}

	return;
}



void insereEnlace(fila *L, uint32_t *pacotes,uint32_t *enlace,int num_pacotes,int num_rotas, int num_filtros){
	uint32_t auxPacote;
	uint32_t auxRota;
	int valEnlace;
	int descarta=1;
	fila *aux=L;
	for(int i=0;i<num_pacotes;i++){
		descarta=1;
		aux=L;

		for(int j=0;j<num_rotas+num_filtros;j++){
			auxPacote = pacotes[i] >>(32- aux->estado.mascara);
			auxRota = aux->estado.endereco >>(32 -aux->estado.mascara);
			
			if((auxPacote^auxRota)==0){
				valEnlace=aux->estado.enlace;
				enlace[valEnlace]+=1;
				descarta=0;
				break;
			}
	
			aux=aux->next;
		}
		if(descarta==1){
			enlace[0]+=1;
		}
	}

	return;





}
uint32_t * roteamento(entrada * rotas, int num_rotas, uint32_t * pacotes, 
	int num_pacotes, entrada * filtros, int num_filtros, int num_enlaces){

	uint32_t *vetEnlace=calloc(num_enlaces,sizeof(int));
	fila *prioriteQueue = NULL;
	for (int i=0;i<num_rotas;i++){
		prioriteQueue = insereFila(prioriteQueue,rotas[i]); 
	}
	for (int i=0;i<num_filtros;i++){
		prioriteQueue = insereFiltros(prioriteQueue,filtros[i]);		
	}
	insereEnlace(prioriteQueue,pacotes,vetEnlace,num_pacotes,num_rotas,num_filtros);
	limpaFila(prioriteQueue);
	return vetEnlace;
}
