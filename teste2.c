#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "rainhas.h"

static clock_t inicio, fim;

//------------------------------------------------------------------------------
#define CRONOMETRA(call,t) inicio = clock(); (call); fim = clock(); t=fim-inicio

//------------------------------------------------------------------------------
static void mostra_resposta(unsigned int n, unsigned int *r) {

  for (unsigned int i=0; i<n; i++) {
    printf("%u ", r[i]);
  }

  printf("\n");
}

static casa *proibe_linha(unsigned int n, casa *proibido){
    for(unsigned int i = 0; i<n; i++){
        proibido[i].linha = 1 ;
        proibido[i].coluna = i+1 ;
    }
    return proibido ;
}

static casa *proibe_coluna(unsigned int n, casa *proibido){
    for(unsigned int i = 0; i<n; i++){
        proibido[i].linha = i+1 ;
        proibido[i].coluna = 1 ;
    }
    return proibido ;
}

static casa *proibe_diagonais(unsigned int n, casa *proibido) {

  // proíbe todas as casas nas diagonais
  for(unsigned int i = 0, p = 1; i < 2 * n; i+=2, p++) {

    // diagonal "principal"
    proibido[i].linha = proibido[i].coluna = p;

    // "outra" diagonal
    proibido[i+1].linha = p;
    proibido[i+1].coluna = n - p + 1;
  }

  return proibido + 2*n;
}

//------------------------------------------------------------------------------
int main (void) {

  unsigned int n;
  scanf("%u", &n) ;
  unsigned int *resposta = malloc(n*sizeof(unsigned int));

  unsigned int k = n;
  casa *proibido = malloc(k*sizeof(casa));

  proibe_linha(n, proibido) ;
  //proibe_diagonais(n, proibido) ;

  /*
  printf("backtracking: ");
  long int tempo_bt;
  CRONOMETRA(rainhas_bt(n, k, proibido, resposta), tempo_bt);
  printf("%ld\n", tempo_bt);
  mostra_resposta(n, resposta);

  printf("grafo: ");
  long int tempo_ci;
  CRONOMETRA(rainhas_ci(n, k, proibido, resposta), tempo_ci);
  printf("%ld\n", tempo_ci);
  mostra_resposta(n, resposta);
  // printf("%.2f\n", (double)tempo_ci/(double)tempo_bt);
    */
  rainhas_ci(n, k, proibido, resposta) ;

  free(proibido);
  free(resposta);

  return 0;
}
