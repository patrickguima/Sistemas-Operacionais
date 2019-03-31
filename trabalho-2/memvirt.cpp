#include <iostream>
#include <stdio.h>
#include <queue>
#include <map>
#include <vector>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "memvirt.h"
using namespace std;
class Processo{
	private:
		int pdi;
		uint32_t refs; 
		uint32_t pfs;
		uint32_t max_frames;
		vector<pair<int, int> > paginas;
	public:
		Processo();
		Processo(int pdi,int max_frames){
			this->pdi = pdi;
			this->max_frames = max_frames;
			this->refs = 0;
			this->pfs = 0;
		}
		int getPdi(){
			return this->pdi;
		}
		uint32_t getRefs(){
			return this->refs;
		}
		uint32_t getPfs(){
			return this->pfs;
		}
    
    uint32_t getMaxframes(){
      return this->max_frames;
    }
    int getPagSize(){
    	return this->paginas.size();
    }
   
    bool checaPag(int pag){
    ++this->refs;
	for (vector<pair<int,int> >::iterator it = this->paginas.begin(); it!=this->paginas.end();it++ ){

	if(it->first == pag){
		it->second = 1;
		return true;
		}
	}
	return false;
	}

	void inserePag(int pag){
		++this->pfs;
		this->paginas.push_back(make_pair(pag,1));

	}
	 void trocaPag(int pag){
	 	++this->pfs;
		 vector<pair<int,int> >::iterator i= this->paginas.begin();

		 
		 while(1){
		 	i=this->paginas.begin();
		 	if(i->second == 1){
				this->paginas.push_back(make_pair(i->first,0));
				this->paginas.erase(this->paginas.begin());	
			
			
		
		}else{
			this->paginas.push_back(make_pair(pag,1));
			this->paginas.erase(this->paginas.begin())	;
			return;
		}
		
		}

	}




};
struct result * init_struct(int num_proc);
queue<pair<int,int> > ler_file(char *filename);
struct result * memvirt(int num_procs, uint32_t num_frames, char * filename){
	
	if(filename == NULL || num_frames ==0 || num_procs ==0 || num_frames < (uint32_t)num_procs ){
		return NULL;
	}

	struct result * results = init_struct(num_procs);
	string nome = filename;
    char* chr = strdup(nome.c_str());
	queue<pair<int,int> > fila_arq = ler_file(chr);
	uint32_t totalRef = fila_arq.size();

	map<int,Processo> processos_map;
	int i;
	
	for(i=0;i<num_procs;i++){
		Processo processo(i,floor(num_frames/num_procs));
		processos_map.insert(pair<int,Processo>(i,processo));
	}

	int a,b;

	while(!fila_arq.empty()){
		 a = fila_arq.front().first;
		 b = fila_arq.front().second;
   		Processo *pro =&(processos_map.at(a));
   		if(pro->checaPag(b)==false){
   			if((uint32_t)pro->getPagSize() != pro->getMaxframes()){
   				pro->inserePag(b);
   			}
   			else{
   				pro->trocaPag(b);
   			}
   		}
		fila_arq.pop();
	}

	uint32_t pfsTotal = 0;
	for (i=0;i<num_procs;i++){
		Processo *pro =&(processos_map.at(i));
		results->refs[i] = pro->getRefs();
		results->pfs[i] =  pro->getPfs();
		results->pf_rate[i] = (float)pro->getPfs()/pro->getRefs();
		pfsTotal = pfsTotal + pro->getPfs();
	}
	
	results->total_pf_rate = (float)pfsTotal/totalRef;
	return results;


}


struct result * init_struct(int num_proc){
    struct result * results = (struct result *) malloc(sizeof(struct result));
    results->refs = (uint32_t *) calloc( num_proc, sizeof(uint32_t));
    results->pfs = (uint32_t *) calloc( num_proc, sizeof(uint32_t));
    results->pf_rate = (float *) calloc( num_proc, sizeof(float));
    return results;
}
queue<pair<int,int> > ler_file(char *filename){
	queue<pair<int,int> > fila_arq;
	FILE *file = fopen(filename,"r");
	int pid,frame;
	while(fscanf(file, "%d %d",&pid,&frame)!=EOF){
		fila_arq.push(make_pair(pid,frame));
	}
	fclose(file);
	return fila_arq;
}
