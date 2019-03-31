# Trabalho 3 - Sistemas Operacionais - 2018/1
Prof. Maurício Lima Pilla - http://lups.inf.ufpel.edu.br/~pilla

## Dados do(a)s aluno(a)s

Declaro que o presente trabalho contém código desenvolvido exclusivamente por mim e que não foi compartilhado de nenhuma forma com terceiros a não ser o professor da disciplina. Compreendo que qualquer violação destes princípios será punida rigorosamente de acordo com o Regimento da UFPEL.

(Preencha com seus dados)

- Nome completo: Patrick Guimarães de Vargas
- Username do Bitbucket: patrickguima
- Email @inf: pgdvargas@inf.ufpel.edu.br

- Nome completo: 
- Username do Bitbucket: 
- Email @inf: 


## Descrição

Este trabalho pode ser feito em grupos de dois alunos. No entanto, os grupos devem ser organizados de forma que apenas alunos que entregaram pelo menos um dos trabalhos podem formar grupos junto. Os alunos que não entregaram trabalhos podem formar grupos entre si.

A ferramenta a ser desenvolvida deve dividir um arquivo em três, usando blocos de 512 bytes e aplicando um processo similar ao RAID5.

- Para os primeiros dois blocos, a paridade fica no terceiro arquivo;
- Para os próximos dois blocos, a paridade fica no segundo arquivo; 
- Para os próximos dois blocos, a paridade fica no primeiro arquivo; e
- Repita o mesmo processo para os próximos blocos.

Após, deve ser capaz de realizar a leitura e gravação de dados usando a interface de programação disponibilizada.

## Entrada e saída


**Cada caso de teste deverá ser documentado com uma explicação do que está sendo testado e o resultado esperado.**

A entrada é um arquivo binário com terminação _.bin_ .

A saída são três arquivos binários _.bin1, .bin2, .bin3_. com o mesmo nome do arquivo original. Caso os arquivos já existam, devem ser sobreescritos.


## Produtos

Os seguintes produtos devem ser entregues:

* Implementação (*raidfs.c*) -- 8 pontos 
* Casos de teses do(a) aluno(a) (*aluno.c*) -- 2 pontos (arquivos de entrada dos testes devem ser adicionados a um diretório *testes*) 
* Makefile e configuração do pipeline -- 0 pontos (mas o trabalho não vai funcionar se não estiverem corretos...)

O pipeline do Bitbucket deve estar executando ambos os testes (primeiro o teste do aluno, depois o do professor) em duas regras:

        make aluno
        make grade

**Grupos com _commits_ predominantemente de um componente apenas terão descontos na nota.**

## Cronograma

* Primeira discussão em aula: _28/06/2018_
* Segunda discussão em aula: _05/07/2018_
* Terceira discussão em aula: _12/07/2018_
* Envio da versão final: _22/07/2018_ 

## Observações

O trabalho pode ser feito em C ou C++. 

Uma estrutura interna pode ser usada para armazenar eventuais descritores de arquivos abertos.

O cálculo do bloco de paridade é livre, mas sugere-se o uso de XOR bit-a-bit.



