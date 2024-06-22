#include "rainhas.h"
#include <stdio.h>


int ll = 0;

//------------------------------------------------------------------------------
// computa uma resposta para a instância (n,c) do problema das n rainhas 
// com casas proibidas usando backtracking
//
//    n é o tamanho (número de linhas/colunas) do tabuleiro
//
//    c é um vetor de k 'struct casa' indicando as casas proibidas
//
//    r é um vetor de n posições (já alocado) a ser preenchido com a resposta:
//      r[i] = j > 0 indica que a rainha da linha i+1 fica na coluna j;
//      r[i] = 0     indica que não há rainha nenhuma na linha i+1
//
// devolve r
unsigned int *rainhas_bt(unsigned int n, unsigned int k, casa *c, unsigned int *r) {
    if(ll == (int) (n)) return r ;

    int valid ;
    for(int j = 0; j< (int) n; j++){
        valid = 1 ;
        int aux ;
        int gap ;
        for(aux = ll-1, gap = 1; aux >= 0; aux--, gap++){
            if((int) r[aux]==j || (int) r[aux]==j-gap || (int) r[aux]==j+gap){
                valid = 0 ;
                break ;
            }
        }

        ll++ ;
        if(valid){
            r[ll-1] = (unsigned int) j ;
            unsigned int *r1 = rainhas_bt(n, k, c, r) ;
            if(r1) return r1 ;
        }
        ll-- ;
    }
    return NULL ;
}
//------------------------------------------------------------------------------
// computa uma resposta para a instância (n,c) do problema das n
// rainhas com casas proibidas usando a modelagem do problema como
// conjunto independente de um grafo
//
// n, c e r são como em rainhas_bt()

unsigned int *rainhas_ci(unsigned int n, unsigned int k, casa *c, unsigned int *r) {

  n = n;
  k = k;
  c = c;

  return r;
}
