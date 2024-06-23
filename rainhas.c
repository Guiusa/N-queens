#include "rainhas.h"
#include <stdio.h>
#include <stdlib.h>

int ll = 1;
int ordered = 0 ;
int *fbd = NULL;

static int check_forbidden(int i, int j, int n){
    return fbd[(i-1)*n + (j-1)] ;
}

static void map(casa *c, int k, unsigned int n){
    fbd = (int *) calloc((n*n), sizeof(int)) ;
    int i, j ;
    for(int l = 0; l<k; l++){
        i = (int) c[l].linha ;
        j = (int) c[l].coluna ;
        fbd[(i-1)*((int) n) + (j-1)] = 1 ;
    }
}

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
    if(!fbd) map(c, (int) k, n) ;
    if(ll > (int) (n)){
        free(fbd) ;
        fbd = NULL ;
        return r ;
    }

    int valid ;
    for(int j = 1; j<= (int) n; j++){
        if(check_forbidden(ll, j, (int) n)) continue ;

        valid = 1 ;
        int aux ;
        int gap ;
        for(aux = ll-1, gap = 1; aux > 0; aux--, gap++){
            if((int) r[aux-1]==j || (int) r[aux-1]==j-gap || (int) r[aux-1]==j+gap){
                valid = 0 ;
                break ;
            }
        }

        ll++ ;
        if(valid){
            r[ll-2] = (unsigned int) j ;
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
