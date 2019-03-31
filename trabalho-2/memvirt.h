#ifndef _MEMVIRT_H_
#define _MEMVIRT_H_
#include <stdint.h>


struct result{
	uint32_t * refs; /* referências a páginas por processo (um vetor!) */
	uint32_t * pfs;  /* page faults por processo */
	float * pf_rate; /* taxa de page fault por processo em % */
	float total_pf_rate; /* taxa de page fault para toda a simulação */
};


/** Função que deve ser implementada
  * @param num_procs número de processos
  * @param num_frames número total de quadros no sistema
  * @param filename nome do arquivo de traço (deve ser um path relativo)
  * @return ponteiro para struct result com as estatísticas de simulação ou NULL
  */
struct result * memvirt(int num_procs, uint32_t num_frames, char * filename);


#endif // _MEMVIRT_H_
