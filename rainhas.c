#include "rainhas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nodo {
    unsigned int vertice;
    struct nodo* next;
} nodo_t;

typedef struct grafo{
    int numVertices;
    struct nodo** listaAdj;
} grafo_t;

static nodo_t* criaNodo (int v){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    nodo->vertice = v;
    nodo->next = NULL;
    return nodo;
}

static grafo_t* criaGrafo(int vertices){
    grafo_t* grafo = malloc(sizeof(grafo_t));
    grafo->numVertices = vertices;
    grafo->listaAdj = malloc(vertices * sizeof(nodo_t*));

    for(int i = 0; i < vertices; i++)
        grafo->listaAdj[i] = NULL;

    return grafo;
}

static grafo_t* adicionarAresta(grafo_t* grafo, int origem, int destino){
    nodo_t* nodo = criaNodo(destino);
    nodo->next = grafo->listaAdj[origem];
    grafo->listaAdj[origem] =  nodo;

    nodo = criaNodo(origem);
    nodo->next = grafo->listaAdj[destino];
    grafo->listaAdj[destino] = nodo;
}

static void printGrafo(grafo_t* grafo){
    for(int i = 0; i < grafo->numVertices; i++){
        nodo_t* aux = grafo->listaAdj[i];
        printf("\n Vertice %d\n: ", i);
        while(aux){
            printf("%d -> ", aux->vertice);
            aux = aux->next;
  
        }
        printf("\n");
    }
}


/*
 * Checa se a posição está proibida
 */
static int check_forbidden(int i, int j, int n, int *fbd){
    return fbd[(i-1)*n + (j-1)] ;
}
//##############################################################################



/*
 *  Mapeia o vetor de casas proibidas para uma matriz, acesso O(1)
 */
static int *map(casa *c, int k, unsigned int n){
    int *fbd = (int *) calloc((n*n), sizeof(int)) ;
    int i, j ;
    for(int l = 0; l<k; l++){
        i = (int) c[l].linha ;
        j = (int) c[l].coluna ;
        fbd[(i-1)*((int) n) + (j-1)] = 1 ;
    }
    return fbd ;
}
//##############################################################################


/*
 *      n       tamanho do tabuleiro
 *      r       vetor resposta
 *      ll      última linha da recursão
 *      fbd     vetor n*n de casas proibidas
 *      q       quantidade de rainhas já posicionadas
 *      r2      vetor resposta auxiliar
 *      best    tamanho da melhor solução encontrada anteriormente
 */
static unsigned int *rainhas_bt_wrapped(unsigned int n, unsigned int *r, int ll, int *fbd, int q, unsigned int *r2, int *best) {
    if((int) (n)-(ll-1) + q <= *best) return NULL ;

    // Chegou ao fim da recursão
    if(ll > (int) (n)){
        // Se posicionou n rainhas, a solução é completa e deve ser retornada
        if (q == (int) n) return r ;

        // Não é completa, mas é a melhor encontrada até esse ponto
        // copia para vetor auxiliar e retorna NULL
        if(q > *best){
            memcpy(r2, r, n * sizeof(unsigned int)) ;
            *best = q ;
        }
        return NULL ;
    }

    int valid ;

    // Itera sobre todas as casas da linha
    for(int j = 1; j<= (int) n; j++){
        // Se for casa proibida, pula pra próxima
        if(check_forbidden(ll, j, (int) n, fbd)) continue ;

        valid = 1 ;
        int aux ;
        int gap ;
        // Checa se a casa não é atacada por outra rainha
        for(aux = ll-1, gap = 1; aux > 0; aux--, gap++){
            if((int) r[aux-1]==j || (int) r[aux-1]==j-gap || (int) r[aux-1]==j+gap){
                valid = 0 ;
                break ;
            }
        }

        // Se a casa é válida, posiciona a rainha e chama a recursão
        if(valid){
            r[ll-1] = (unsigned int) j ;
            unsigned int *r1 = rainhas_bt_wrapped(n, r, ll+1, fbd, q + 1, r2, best) ;
            if(r1){
                return r1 ;
            } else continue ;
        }
    }
    // Não posicionou rainha na linha, coloca 0
    r[ll-1] = 0 ;
    // chama a recursão retornando NULL (solução incompleta)
    rainhas_bt_wrapped(n, r, ll+1, fbd, q, r2, best) ;
    return NULL ;
}
//##############################################################################



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
    int ll = 1 ;
    int *fbd = map(c, (int) k, n) ;
    int best = 0 ;
    unsigned int *r2 = (unsigned int *) calloc(n, sizeof(unsigned int)) ;
    unsigned int *r1 = rainhas_bt_wrapped(n, r, ll, fbd, 0, r2, &best) ;
    if(!r1){
        memcpy(r, r2, n * sizeof(unsigned int)) ;
    }
    free(r2) ;
    free(fbd) ;
    return r ;
}
//##############################################################################



//------------------------------------------------------------------------------
// computa uma resposta para a instância (n,c) do problema das n
// rainhas com casas proibidas usando a modelagem do problema como
// conjunto independente de um grafo
//
// n, c e r são como em rainhas_bt()
unsigned int *rainhas_ci(unsigned int n, unsigned int k, casa *c, unsigned int *r) {
    grafo_t* g = criaGrafo(4);
    adicionarAresta(g, 0, 1);
    adicionarAresta(g, 0, 2);
    adicionarAresta(g, 0, 3);
    adicionarAresta(g, 1, 2);

    printGrafo(g);

    n = n;
    k = k;
    c = c;

  return r;
}
