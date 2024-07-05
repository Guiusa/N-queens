#include "rainhas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define uint unsigned int



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
//      r[i] = 0     indica que nã0x35o há rainha nenhuma na linha i+1
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



/*
 * Struct da fila
 */
typedef struct queue_t {
    struct queue_t *next ;
    struct nodo_t *points ;
} queue_t ;
/*
* Struct do nodo
*/
typedef struct nodo_t {
    struct queue_t *adj ;
    uint l, c ;
    short valid, percorrido ;
} nodo_t ;



/*
 * Printa os elementos enfileirados nas filas de adjacência
 */
static void print_fila(nodo_t* n){
    queue_t *aux = n->adj ;
    if(!aux) return ;
    while(aux){
        printf("->[%d][%d]", aux->points->l, aux->points->c) ;
        aux = aux->next ;
    }
}
//##############################################################################



/*
 * Printa o grafo
 */
static void print_graph(nodo_t *g, int n){
    for(int i = 0; i<n*n; i++){
        printf("[%d][%d]", g[i].l, g[i].c) ;
        print_fila(&g[i]) ;
        printf("\n") ;
    }
}
//##############################################################################



/*
 * Inicializa um nodo no vetor principal do grafo
 */
static nodo_t *init_nodo_t(nodo_t* nodo, uint l, uint c) {
    nodo->l = l ;
    nodo->c = c ;
    nodo->valid = 1 ;
    nodo->percorrido = 0 ;
    nodo->adj = NULL ;
    return nodo ;
}
//##############################################################################



/*
 * Inicializa um elemento a ser adicionado nas listas de adjacência
 */
static queue_t *init_queue_t(nodo_t* points){
    queue_t* queue_node = (queue_t *) malloc(sizeof(queue_t)) ;
    queue_node->points = points ;
    queue_node->next = NULL ;
    return queue_node ;
}
//##############################################################################



/*
 * Adiciona um elemento à lista (adiciona no inicio para não percorrer)
 */
static void queue_append(nodo_t* head, queue_t* elem){
    elem->next = head->adj ;
    head->adj = elem ;
}
//##############################################################################



/*
 * Adiciona casas atacadas de acordo com a distância
 * Retorna número de arestas criadas
 * Se d=1, adiciona todas as casas que atacam a casa [i][j] e estão a 1 casa de
 * distância
 */
static int add_by_distance(int i, int j, nodo_t* g, int d, int n){
    int edges = 0 ;
    nodo_t* nodo_aux1 = &g[(i*n) + j] ;
    
    for(int iaux = -d; iaux<= +d; iaux+=d){
        // Se a linha ultrapassa os limites do tabuleiro, continua
        if(i+iaux < 0 || i+iaux >= n) continue ;

        for(int jaux = -d; jaux <= d; jaux+=d){
            // Se a coluna ultrapassa so limites do tabuleiro, continua
            if(j+jaux < 0 || j+jaux >= n) continue ;
            // Se a casa comparada for a própria casa recebida, não adiciona
            if(jaux == 0 && iaux == 0) continue ;

            nodo_t* nodo_aux2 = &g[((i+iaux)*n) + (j+jaux)] ;
            // Se a casa a ser comparada já foi percorrida, não insere de novo
            if(nodo_aux2->percorrido) continue ;

            queue_t* queue_aux1 = init_queue_t(nodo_aux1) ;
            queue_t* queue_aux2 = init_queue_t(nodo_aux2) ;

            // X é vizinho de Y, então X é adicionado à lista de Y e vice-versa
            queue_append(nodo_aux1, queue_aux2) ;
            queue_append(nodo_aux2, queue_aux1) ;
            edges++ ;
        }
    }
    nodo_aux1->percorrido = 1 ;
    return edges ;
}
//##############################################################################


/*
 * Cria uma representação do tabuleiro com uma lista de adjacência
 */
static nodo_t* init_graph(int n){
    nodo_t *graph = (nodo_t *) malloc(n*n * sizeof(nodo_t)) ;
    // Inicia todos os nodos do vetor principal
    for(int i = 0; i<n*n; i++){
        nodo_t* aux = &graph[i] ;
        init_nodo_t(aux, i/n, i%n) ;
    }

    for(int i = 0; i<n; i++){
        for(int j = 0; j<n; j++){
            for(int d = 1; d < n; d++){
                add_by_distance(i, j, graph, d, n) ;
            }
        }
    }

    return graph ;
}
//##############################################################################



/*
 * Libera o grafo e inibe leaks
 */
static void destroy_graph(nodo_t *g, int n){
    for(int i = 0; i<n*n; i++)
        while(g[i].adj){
            queue_t *aux = g[i].adj ;
            g[i].adj = g[i].adj->next ;
            free(aux) ;
        }
    free(g) ;
}
//##############################################################################

//------------------------------------------------------------------------------
// computa uma resposta para a instância (n,c) do problema das n
// rainhas com casas proibidas usando a modelagem do problema como
// conjunto independente de um grafo
//
// n, c e r são como em rainhas_bt()
unsigned int *rainhas_ci(unsigned int n, unsigned int k, casa *c, unsigned int *r) {
    nodo_t* graph = init_graph((int) n) ;
    print_graph(graph, (int) n) ;
    destroy_graph(graph, (int) n) ;
    return r;
}
