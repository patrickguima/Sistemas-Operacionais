#ifndef _RAIDFS_H_
#define _RAIDFS_H_

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SUCESSO 1
#define FALHA 0

#define LEITURA 0
#define ESCRITA 1
#define LEITURAESCRITA 2

typedef int indice_arq_t;

/** Inicializa o arquivo raid a partir de um arquivo binário
 *
 *  Caso o nome de arquivo já tenha sido inicializado, deve sobreescrever.
 *
 *  @param arquivo nome do arquivo (sem terminação .bin)
 *  @return SUCESSO ou FALHA
 */
int initfs(char * arquivo);


/** Abre um arquivo
 *
 *  @param nome do arquivo no sistema de arquivos já inicializado (sem terminação .bin)
 *  @return file handle do descritor de sistema de arquivos ou FALHA
 */
 
indice_arq_t f_open(char * nome,  int acesso);

/** Fecha um arquivo.
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @return SUCESSO OU FALHA
 */
int f_close(indice_arq_t arquivo);

/** Lê bytes de um arquivo aberto.
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @param tamanho número de bytes a serem lidos
 * @param buffer ponteiro para buffer onde serão armazenados os bytes lidos
 * @return número de bytes lidos
 */
uint32_t f_read(indice_arq_t arquivo, uint32_t tamanho, char *buffer);

/** Escreve bytes em um arquivo aberto.
 *
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @param tamanho número de bytes a serem escritos
 * @param buffer ponteiro para conteúdo a ser escrito
 * @return SUCESSO ou FALHA
 */
int f_write(indice_arq_t arquivo, uint32_t tamanho, char *buffer);

/** Apaga um arquivo e o fecha.
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @return SUCESSO ou FALHA
 */

int f_delete(indice_arq_t arquivo);

/** Modifica a posição atual de leitura ou escrita do arquivo
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @param seek deslocamento em relação ao início do arquivo
 * @return SUCESSO ou FALHA
 */
int f_seek(indice_arq_t arquivo, uint32_t seek);


/** Recupera arquivo RAID que perdeu um dos seus arquivos componentes
 * 
 * @param nome nome do arquivo sem terminação .bin
 * @return SUCESSO ou FALHA
 */
int recover(char * nome);


#endif
