#include "rainhas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define uint unsigned int

char BUFF[] = "                              " ;


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
static int *map_bt(casa *c, int k, uint n){
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
static uint *rainhas_bt_wrapped(uint n,
                                        uint *r,
                                        int ll,
                                        int *fbd,
                                        int q,
                                        uint *r2,
                                        int *best) {
    if((int) (n)-(ll-1) + q <= *best) return NULL ;

    // Chegou ao fim da recursão
    if(ll > (int) (n)){
        // Se posicionou n rainhas, a solução é completa e deve ser retornada
        if (q == (int) n) return r ;

        // Não é completa, mas é a melhor encontrada até esse ponto
        // copia para vetor auxiliar e retorna NULL
        if(q > *best){
            memcpy(r2, r, n * sizeof(uint)) ;
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
            r[ll-1] = (uint) j ;
            uint *r1 = rainhas_bt_wrapped(n, r, ll+1, fbd, q + 1, r2, best) ;
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
uint *rainhas_bt(uint n, uint k, casa *c, uint *r) {
    int ll = 1 ;
    int *fbd = map_bt(c, (int) k, n) ;
    int best = 0 ;
    uint *r2 = (uint *) calloc(n, sizeof(uint)) ;
    uint *r1 = rainhas_bt_wrapped(n, r, ll, fbd, 0, r2, &best) ;
    if(!r1){
        memcpy(r, r2, n * sizeof(uint)) ;
    }
    free(r2) ;
    free(fbd) ;
    return r ;
}
//##############################################################################



/*
 * Struct da fila
 *  - next      Próximo elemento queue_t da fila que está inserido
 *  - points    Endereço do nodo que esse elemento da fila referencia, ou seja,
 *  qual casa do tabuleiro que é vizinho
 */
typedef struct queue_t {
    struct queue_t *next ;
    struct nodo_t *points ;
} queue_t ;

/*
* Struct do nodo
*  - adj        Fila de casas que são adjacentes ao nodo
*  - proibiu    Qual nodo que o retirou do vetor C (necessário na recursão)
*  - l, c       Coordenadas da casa
*  - valid      Se o nodo pode ser escolhido
*  - percorrido Usado para percorrer os nodos ao criar as listas de adjacência
*  - vizinhos   Quantos vizinhos tem no nodo
*/
typedef struct nodo_t {
    struct queue_t *adj ;
    struct nodo_t *proibiu ;
    uint l, c ;
    short valid, percorrido ;
    int vizinhos ;
} nodo_t ;

nodo_t proibidor ;



/*
 * Inicializa um nodo no vetor principal do grafo
 */
static nodo_t *init_nodo_t(nodo_t* nodo, uint l, uint c) {
    nodo->l = l ;
    nodo->c = c ;
    nodo->valid = 1 ;
    nodo->percorrido = 0 ;
    nodo->adj = NULL ;
    nodo->proibiu = NULL ;
    nodo->vizinhos = 0 ;
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
            nodo_aux2->vizinhos++ ;
        }
    }
    nodo_aux1->vizinhos += edges ;
    nodo_aux1->percorrido = 1 ;
    return edges ;
}
//##############################################################################



/*
 * Coloca os nodos proibidos como inválidos
 */
static void map_ci(casa *c, int k, int n, nodo_t* nodes){
    for(int l = 0; l<k; l++){
        int i = (int) c[l].linha -1;
        int j = (int) c[l].coluna -1;
        nodes[(i*n) + j].valid = 0 ;
        nodes[(i*n) + j].proibiu = &proibidor ;
    }
}
/*
 * Cria uma representação do tabuleiro com uma lista de adjacência
 */
static nodo_t* init_graph(int n){
    long uint n2 = (uint) n* (uint) n ;
    nodo_t *graph = (nodo_t *) malloc(n2 * sizeof(nodo_t)) ;
    // Inicia todos os nodos do vetor principal
    for(uint i = 0; i<n*n; i++){
        nodo_t* aux = &graph[i] ;
        init_nodo_t(aux, i/(uint) n+1, i%(uint) n+1) ;
    }

    for(int i = 0; i<n; i++){
        for(int j = 0; j<(int) n; j++){
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



/*
 * Percorre a fila de vizinhos de um nodo setando todos para inválido
 */
static int sets_queue(nodo_t* sol, short n){
    int c = 0 ;
    queue_t *q = sol->adj ;
    if(n == 0){
        for(; q!= NULL; q=q->next){
            if(q->points->valid == 2) continue ;
            if(q->points->proibiu) continue ;

            q->points->valid = 0 ;
            q->points->proibiu = sol ;
            c++ ;
        }
    } else {
        for(; q!= NULL; q = q->next){
            if(q->points->valid == 2) continue ;
            if(q->points->proibiu != sol) continue ;
            
            q->points->valid = 1 ;
            q->points->proibiu = NULL ;
            c++ ;
        }
    }
    return c ;
}
//##############################################################################



/*
 *  n       tamanho do tabuleiro
 *  r       vetor resposta
 *  r2      vetor resposta auxiliar
 *  q       quantia de rainhas já posicionadas
 *  tam_c   tamanho do conjunto C
 *  best    melhor solução encontrada até agora
 *  g       grafo com listas de adjacência
 */
static uint *rainhas_ci_wrapped(int n,
                                uint *r,
                                uint *r2,
                                int q,
                                int tam_c,
                                int* best,
                                nodo_t* g){
    // Se chegou ao fim, retorna
    if (q == n){
        *best = q ;
        return r ;
    }

    // Não pode ser a melhor solução, retorna NULL
    if(q + tam_c <= *best) return NULL ;
    
    nodo_t *v = NULL;
    // Itera sobre os nodos disponíveis em C
    for(int i = 0; i<n*n; i++){
        if(g[i].valid==1){
            v = &g[i] ;
            break ;
        }
    }
    if(!v) return NULL ; // Se não achou nenhum, retorna null
    
    // Seta todos os vizinhos do nodo escolhido para inválido
    tam_c -= sets_queue(v, 0) ;
    v->valid = 2 ;
    tam_c--;     

    // Coloca a rainha
    r[v->l-1] = v->c ;

    // Checa se se está na maior solução
    if(q > *best){
        *best = q ;
        memcpy(r2, r, (uint) n*sizeof(uint)) ;
    }

    // Chama a recursão com v no CI
    uint *r1 = rainhas_ci_wrapped(n, r, r2, q+1, tam_c, best, g) ;
    if(r1) return r1 ;


    // Se falhou, v não pertence a CI, chama a recursão de novo
    tam_c += sets_queue(v, 1) ;
    r1 = rainhas_ci_wrapped(n, r, r2, q, tam_c, best, g) ;
    if(r1) return r1 ;

    // Se falhou, reseta v e pede outra instância pra nível de cima
    v->valid = 1 ;
    r[v->l-1] = 0 ;
    tam_c ++ ;
    return NULL ;
}

//------------------------------------------------------------------------------
// computa uma resposta para a instância (n,c) do problema das n
// rainhas com casas proibidas usando a modelagem do problema como
// conjunto independente de um grafo
//
// n, c e r são como em rainhas_bt()
uint *rainhas_ci(uint n, uint k, casa *c, uint *r) {
    nodo_t* graph = init_graph((int) n) ;
    map_ci(c, (int) k, (int) n, graph) ;

    int best = 0 ;
    uint *r2 = calloc(n, sizeof(uint)) ;
    uint *r1 = rainhas_ci_wrapped((int) n, r, r2, 0, (int) (n*n-k), &best, graph) ; 
    
    if (!r1) memcpy(r, r2, n*sizeof(uint)) ;

    destroy_graph(graph, (int) n) ;
    free(r2) ;
    return r;
}
