#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raidfs.h"
#include <string.h>
typedef struct SA sa;
struct SA{
	char *nome;
	char *nomeBin1;
	char *nomeBin2;
	char *nomeBin3;
	int *rw;
	int *pos;

};

sa my_sa[10];
int total_arq=1;
int block_size=512;
void set_paridade(FILE *bin1,FILE *bin2,FILE *bin3,char *entradaBin1,char *entradaBin2,char *entradaBin3,int numToPut){
	int i;
	for(i=0;i<numToPut;i++){
		if(entradaBin1[i]!=0){
			fputc(entradaBin1[i],bin1);
		}
		if(entradaBin2[i]!=0){
			fputc(entradaBin2[i],bin2);
		}
		if(entradaBin3[i]==0){
			fputc('0',bin3);
		}
		else{
			fputc(entradaBin3[i],bin3);
			}
		}
	return;
}
int set_fs(FILE *file,FILE *bin1,FILE *bin2, FILE *bin3){
	int i=0;
	int j=0;
	int k;
	int b=3;
	char c;
	char *entradaBin1;
	char *entradaBin2;
	char *entradaBin3;

	while (!feof(file)){
		entradaBin1 = calloc(512,sizeof(char));
		entradaBin2 = calloc(512,sizeof(char));
		entradaBin3 = calloc(512,sizeof(char));
		
		if(b==0){
			b=3;
		}

		for (i=0;i<block_size && ((c = fgetc(file)) != EOF);i++){
				entradaBin1[i]= c;
		}
		for(j=0;j<block_size && ((c = fgetc(file))!=EOF);j++){
			entradaBin2[j]=c;	
		}
		for(k=0;k<block_size;k++){
			entradaBin3[k] = entradaBin1[k]^entradaBin2[k];
		}		

		
		if(b==3){
			set_paridade(bin1,bin2,bin3,entradaBin1,entradaBin2,entradaBin3,i);
		}
		if(b==2){
			set_paridade(bin1,bin3,bin2,entradaBin1,entradaBin2,entradaBin3,i);
		}
		if(b==1){
			set_paridade(bin2,bin3,bin1,entradaBin1,entradaBin2,entradaBin3,i);
		}
		

		
		free(entradaBin1);
		free(entradaBin2);
		free(entradaBin3);
	
		b--;
	}

		return SUCESSO;

}

int initfs(char * arquivo){
	char entrada[30];
	FILE *bin1;
	FILE *bin2;
	FILE *bin3;
	strcpy(entrada,arquivo);
	strcat(entrada,".bin");
	FILE *file = fopen(entrada,"rb");
	

	if(file == NULL){
		return FALHA;
	}
	else{
		strcpy(entrada,arquivo);
		strcat(entrada,".bin1");
		bin1 = fopen(entrada,"wb");

		strcpy(entrada,arquivo);
		strcat(entrada,".bin2");
		bin2 = fopen(entrada,"wb");
		
		strcpy(entrada,arquivo);
		strcat(entrada,".bin3");
		bin3 = fopen(entrada,"wb");

		strcpy(entrada,arquivo);
		strcat(entrada,".bin");
			
		int out = set_fs(file,bin1,bin2,bin3);




		fclose(file);
		fclose(bin1);
		fclose(bin2);
		fclose(bin3);

	return out;
	}

}
 indice_arq_t f_open(char * nome,  int acesso){
 	int i;
 	char entrada[30];
 	for(i=1;i<total_arq;i++){
 		if(strcmp(my_sa[i].nome,nome)==0 && *my_sa[i].rw == acesso){
 			return i;
 		}
 	}
 	int n=strlen(nome);
 	my_sa[total_arq].nome = calloc(n+1,sizeof(char));
 	my_sa[total_arq].nomeBin1 = calloc(n+5,sizeof(char));
 	my_sa[total_arq].nomeBin2 = calloc(n+5,sizeof(char));
 	my_sa[total_arq].nomeBin3 = calloc(n+5,sizeof(char));
 	my_sa[total_arq].rw = calloc(1,sizeof(int));
 	my_sa[total_arq].pos = calloc(1,sizeof(int));
 	strcpy(my_sa[total_arq].nome,nome);
 	strcpy(entrada,nome);
	strcat(entrada,".bin1");
	strcpy(my_sa[total_arq].nomeBin1,entrada);
	strcpy(entrada,nome);
	strcat(entrada,".bin2");
	strcpy(my_sa[total_arq].nomeBin2,entrada);
	strcpy(entrada,nome);
	strcat(entrada,".bin3");
	strcpy(my_sa[total_arq].nomeBin3,entrada);

 	*my_sa[total_arq].pos = 0;
 	*my_sa[total_arq].rw = acesso;
 	total_arq = total_arq+1;
 	return (total_arq -1);
 }
int read_correct(FILE *bin1,FILE *bin2, FILE *bin3,int b,char *buffer, uint32_t tamanho,int seekPos){
	char c;
	int i;
	int seekControl=0;
	uint32_t j=0;
	while(1){
		if(feof(bin1) &&feof(bin2) && feof(bin3) )
			break;
		if(b==0){
			b=3;
		}
		if(b==3){
			for(i=0;i<block_size && ((c = fgetc(bin1)) != EOF);i++){
				if(seekControl >=seekPos){
				buffer[j] = c;
				j++;
				if(j==tamanho){
					return j;
				}
			}
			seekControl++;
			}
			for(i=0;i<block_size && ((c = fgetc(bin2)) != EOF);i++){
			if(seekControl >=seekPos){
				buffer[j] = c;
				j++;
				if(j==tamanho){
					return j;
				}
			}
			seekControl++;
		}
			for(i=0;i<block_size && ((c = fgetc(bin3)) != EOF);i++);
		}
		if(b==2){
			for(i=0;i<block_size && ((c = fgetc(bin1)) != EOF);i++){
			if(seekControl >=seekPos){
				buffer[j] = c;
				j++;
				if(j==tamanho){
					return j;
				}
			}
			seekControl++;
		}
			for(i=0;i<block_size && ((c = fgetc(bin3)) != EOF);i++){
			if(seekControl >=seekPos){
				buffer[j] = c;
				j++;
				if(j==tamanho){
					return j;
				}
			}
			seekControl++;
		}
			for(i=0;i<block_size && ((c = fgetc(bin2)) != EOF);i++);
		}
		
		if(b==1){
			for(i=0;i<block_size && ((c = fgetc(bin2)) != EOF);i++){
		if(seekControl >=seekPos){
				buffer[j] = c;
				j++;
				if(j==tamanho){
					return j;
				}
			}
			seekControl++;
		}
			for(i=0;i<block_size && ((c = fgetc(bin3)) != EOF);i++){
			if(seekControl >=seekPos){
				buffer[j] = c;
				j++;
				if(j==tamanho){
					return j;
				}
			}
			seekControl++;
			}
			for(i=0;i<block_size && ((c = fgetc(bin1)) != EOF);i++);
		}
	b--;


	
	}		
	return j;



}
uint32_t f_read(indice_arq_t arquivo, uint32_t tamanho, char *buffer){
	int num_read;
	int seekPos = *my_sa[arquivo].pos;
	int b=3;
	if(*my_sa[arquivo].rw ==ESCRITA){
		return FALHA;
	}
	FILE *bin1;
	FILE *bin2;
	FILE *bin3;

	bin1 = fopen(my_sa[arquivo].nomeBin1,"rb");
	bin2 = fopen(my_sa[arquivo].nomeBin2,"rb");
	bin3 = fopen(my_sa[arquivo].nomeBin3,"rb");

	num_read = read_correct(bin1,bin2,bin3,b,buffer,tamanho,seekPos);

	fclose(bin1);
	fclose(bin2);
	fclose(bin3);
	*my_sa[arquivo].pos=num_read;
	return num_read;
}

int f_seek(indice_arq_t arquivo, uint32_t seek){

	if(my_sa[arquivo].nome==NULL){
		return FALHA;
	}
	*my_sa[arquivo].pos = seek;
	return SUCESSO;

}
int set_newfs(char *entrada,FILE *bin1,FILE *bin2, FILE *bin3){
	int z=0;
	int i=0;
	int j=0;
	int k;
	int b=3;
	
	char *entradaBin1;
	char *entradaBin2;
	char *entradaBin3;
	int n = strlen(entrada);
	while (z<n){
		entradaBin1 = calloc(512,sizeof(char));
		entradaBin2 = calloc(512,sizeof(char));
		entradaBin3 = calloc(512,sizeof(char));
		
		if(b==0){
			b=3;
		}

		for (i=0;i<block_size && z<n;i++,z++){
				entradaBin1[i]= entrada[z];
		}
		for(j=0;j<block_size && z<n;j++,z++){
			entradaBin2[j]=entrada[z];	
		}
		for(k=0;k<block_size;k++){
			entradaBin3[k] = entradaBin1[k]^entradaBin2[k];
		}		

		
		if(b==3){
			set_paridade(bin1,bin2,bin3,entradaBin1,entradaBin2,entradaBin3,i);
		}
		if(b==2){
			set_paridade(bin1,bin3,bin2,entradaBin1,entradaBin2,entradaBin3,i);
		}
		if(b==1){
			set_paridade(bin2,bin3,bin1,entradaBin1,entradaBin2,entradaBin3,i);
		}
		free(entradaBin1);
		free(entradaBin2);
		free(entradaBin3);
	
		b--;
	}

		return SUCESSO;

}
int f_write(indice_arq_t arquivo, uint32_t tamanho, char *buffer){

	int i=0;
	uint32_t j=0;
	if(*my_sa[arquivo].rw == LEITURA){
		return FALHA;
	}
	char *buffer3 = (char *)malloc(block_size);
	int posArq=*my_sa[arquivo].pos;

	FILE *bin1;
	FILE *bin2;
	FILE *bin3;
	int z =0;
	int time=1;
	bin1 = fopen(my_sa[arquivo].nomeBin1,"rb");
	bin2 = fopen(my_sa[arquivo].nomeBin2,"rb");	
	bin3 = fopen(my_sa[arquivo].nomeBin3,"rb");

	while(1){
		char *buffer2=calloc(1,sizeof(char));
		fseek(bin1,0,0);
		fseek(bin2,0,0);
		fseek(bin3,0,0);

		if(j==(uint32_t)posArq){
		 for(uint32_t k=0;k<tamanho;k++){
		 	buffer3[z] = buffer[k];
		 	z++;
			if(z==(block_size*time)-1){
				time++;
				buffer3 = (char *)realloc(buffer3,time*block_size);
			}
		 	
		 	}
		}
		read_correct(bin1,bin2,bin3,3,buffer2,1,i);
		buffer3[z] = buffer2[0];
		if(z==(block_size*time)-1){
			time++;
			buffer3 = (char *)realloc(buffer3,time*block_size);
		}
		z++;
		i++;
		if(buffer2[0]==0){
			free(buffer2);
			break;
		}
		free(buffer2);
		j++;
	}
	fclose(bin1);
	fclose(bin2);
	fclose(bin3);

	bin1 = fopen(my_sa[arquivo].nomeBin1,"wb");
	bin2 = fopen(my_sa[arquivo].nomeBin2,"wb");
	bin3 = fopen(my_sa[arquivo].nomeBin3,"wb");
	set_newfs(buffer3,bin1,bin2,bin3);
	fclose(bin1);
	fclose(bin2);
	fclose(bin3);
	free(buffer3);
		
	return SUCESSO;
}

int f_delete(indice_arq_t arquivo){

	if(my_sa[arquivo].nome==NULL){
		return FALHA;
	}
	remove((const char*)my_sa[arquivo].nome);
	remove((const char*)my_sa[arquivo].nomeBin1);
	remove((const char*)my_sa[arquivo].nomeBin2);
	remove((const char*)my_sa[arquivo].nomeBin3);
	f_close(arquivo);
	return SUCESSO;




}

void recupera(char *nome,char *entrada,char *entrada2){

	FILE *bin1;
	FILE *bin2;
	FILE *bin3;
	char c =0;
	char d =0;	
	char e;
	bin1 = fopen(nome,"wb");
	bin2 = fopen(entrada,"rb");
	bin3 = fopen(entrada2,"rb");
	while( (c = fgetc(bin2)) != EOF ||  (d = fgetc(bin3)) != EOF ){
		if(c=='0'){
			c= 0;
		}
		if(d=='0'){
			d=0;
		}
		e = c^d;
		fputc(e,bin1);
	}
	fclose(bin1);
	fclose(bin2);
	fclose(bin3);
	return;

}

int recover(char * nome){
	char *entrada;
	char *entrada2;
	char *entrada3;
	entrada =calloc(20,sizeof(char));
	entrada2 =calloc(20,sizeof(char));
	entrada3 =calloc(20,sizeof(char));
	FILE *bin1;
	FILE *bin2;
	FILE *bin3;
	strcpy(entrada,nome);
	strcat(entrada,".bin1");
	strcpy(entrada2,nome);
	strcat(entrada2,".bin2");
	strcpy(entrada3,nome);
	strcat(entrada3,".bin3");
	bin1 = fopen(entrada,"rb");
	bin2 = fopen(entrada2,"rb");
	bin3 = fopen(entrada3,"rb");

	if(bin1==NULL && bin2 ==NULL && bin3!=NULL){
		fclose(bin3);
		return FALHA;

	}
	if(bin1==NULL && bin2 !=NULL && bin3==NULL){
		fclose(bin2);
		return FALHA;

	}
	if(bin1!=NULL && bin2 ==NULL && bin3==NULL){
		fclose(bin1);
		return FALHA;
	}
	
	if(bin1==NULL){		
		fclose(bin2);
		fclose(bin3);
		recupera(entrada,entrada2,entrada3);
	 }else{

		if(bin2==NULL){
			fclose(bin1);
			fclose(bin3);
			recupera(entrada2,entrada,entrada3);
		}
		else{

		if(bin3==NULL){
			fclose(bin1);
			fclose(bin2);
			recupera(entrada3,entrada,entrada2);
		}}}

		
		free(entrada);
		free(entrada2);
		free(entrada3);


	return SUCESSO;

}


int f_close(indice_arq_t arquivo){
	if(my_sa[arquivo].nome ==NULL){
		return FALHA;
	}
	free(my_sa[arquivo].nome);
	free(my_sa[arquivo].nomeBin1);
	free(my_sa[arquivo].nomeBin2);
	free(my_sa[arquivo].nomeBin3);
	free(my_sa[arquivo].pos);
	free(my_sa[arquivo].rw);
	total_arq=total_arq-1;


	return SUCESSO;
}	
