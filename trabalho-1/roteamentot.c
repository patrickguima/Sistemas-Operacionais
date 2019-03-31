#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "roteador.h"
#include <pthread.h>

pthread_mutex_t trava;

typedef struct FilaP filaP;
struct FilaP{
	uint32_t endereco;
	filaP *next;
};

typedef struct BUFFERR bufferr;
struct BUFFERR{
	uint32_t *vet;
	uint32_t *pacotes_insercao;
	entrada *rotas;
	filaP *pacotes;
	int num_rotas;
	int num_filtros;
	int num_pacotes;
	int fim;

};

void selection_sort(entrada rotasLista[], entrada rotas[],entrada filtros[],int num_filtros,int num_rotas) { 
  int i, j, min;
  int tam= num_filtros+num_rotas; 
  entrada aux;
  for (i =0;i<num_filtros;i++){
  	rotasLista[i]=filtros[i];
  }
   for (i =num_filtros;i<num_rotas+num_filtros;i++){
  	rotasLista[i]=rotas[i-num_filtros];
  }

  for (i = num_filtros; i < (tam-1); i++) 
  {
     min = i;
     for (j = (i+1); j < tam; j++) {
       if(rotasLista[j].mascara > rotasLista[min].mascara) 
         min = j;
     }
     if (rotasLista[i].mascara != rotasLista[min].mascara) {
       aux = rotasLista[i];
       rotasLista[i] = rotasLista[min];
       rotasLista[min] = aux;
     }
  }
}

filaP *inserePacotes(filaP *L,uint32_t pacote){
	if(L==NULL){
		L = malloc(sizeof(filaP));
		L->next = NULL;
		L->endereco = pacote;
		return L;
	}
	filaP *aux2 = L;
	filaP *aux = malloc(sizeof(filaP));
	aux->next=NULL;
	aux->endereco = pacote;
	while(L->next!=NULL){
		L=L->next;
	}
	L->next = aux;
	return aux2;

}

filaP *retiraPacotes(filaP *L,uint32_t *val){
	
	if(L ==NULL){
		*val = -1;
		return L;
	}
	*val = L->endereco;
	filaP *aux = L->next;
	free(L);
	return aux;

}


void *insereEnlace(void *buf){
	int descarta;
	uint32_t auxPacote;
	uint32_t auxRota;
	int valEnlace;
	uint32_t compare = -1;
	uint32_t *val = calloc(1,sizeof(uint32_t));
	entrada *rotas;
	bufferr *aux = (bufferr *)buf;
	rotas = aux->rotas;
	while(1){
		bufferr *aux = (bufferr *)buf;
		rotas =  aux->rotas;
		descarta=1;
		pthread_mutex_lock(&trava);
		aux->pacotes= retiraPacotes (aux->pacotes,val);
		pthread_mutex_unlock(&trava);
		if(*val == compare	 && aux->fim == 1)	{
			break;
		}
		if(*val!= compare){
			for(int j=0;j<aux->num_rotas + aux->num_filtros;j++){
				auxPacote = (*val) >>(32- rotas[j].mascara);
				auxRota = rotas[j].endereco >>(32 -rotas[j].mascara);
				if((auxPacote^auxRota)==0){
					pthread_mutex_lock(&trava);
					valEnlace=rotas[j].enlace;
					aux->vet[valEnlace]+=1;
					pthread_mutex_unlock(&trava);
					descarta=0;
					break;
				}
			
			}
			if(descarta==1){
				pthread_mutex_lock(&trava);
				aux->vet[0]+=1;
				pthread_mutex_unlock(&trava);
			}
		}

	}
	free(val);
	return NULL;
}
void *insercao_fila(void *buf){
	int i;
	bufferr *aux = (bufferr *)buf;
	for (i =0 ;i<aux->num_pacotes;i++){
	pthread_mutex_lock(&trava);
	aux->pacotes=inserePacotes(aux->pacotes,aux->pacotes_insercao[i]);
	pthread_mutex_unlock(&trava);
	}
	aux->fim = 1;
	return NULL;



}


uint32_t * roteamento(entrada * rotas, int num_rotas, uint32_t * pacotes, 
	int num_pacotes, entrada * filtros, int num_filtros, int num_enlaces){

	int i;
	
	entrada *rotasLista = calloc(num_filtros+num_rotas,sizeof(entrada));
	selection_sort(rotasLista,rotas,filtros,num_filtros,num_rotas);

	bufferr buf;
	buf.vet = (uint32_t *)calloc(num_enlaces,sizeof(uint32_t));
	buf.num_rotas = num_rotas;
	buf.num_filtros = num_filtros;
	buf.num_pacotes = num_pacotes;
	buf.pacotes= NULL;
	buf.rotas = rotasLista;
	buf.pacotes_insercao =pacotes;
	buf.fim = 0;

	pthread_mutex_init(&trava, NULL);
	pthread_t threadInsertion;
	pthread_t thread[4];
	pthread_attr_t attr;
    pthread_attr_init(&attr);

	pthread_create( &threadInsertion, &attr, insercao_fila,&buf);
	
	for(i=0;i<4;i++){
 	pthread_create( &thread[i], &attr, insereEnlace,&buf);
 	}
 
 	for(i=0;i<4;i++){
 	pthread_join(thread[i], NULL);
 	}
 
 	pthread_join(threadInsertion, NULL);
 	free(rotasLista);
	return buf.vet;

}