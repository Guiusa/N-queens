┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
┃                Guiusepe Oneda Dal Pai    - GRR20210572                       ┃
┃                Pedro Willian Aguiar      - GRR20211766                       ┃
┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃            _   _                 _       _                                   ┃
┃           | \ | |      _ __ __ _(_)_ __ | |__   __ _ ___                     ┃
┃           |  \| |_____| '__/ _` | | '_ \| '_ \ / _` / __|                    ┃
┃           | |\  |_____| | | (_| | | | | | | | | (_| \__ \                    ┃
┃           |_| \_|     |_|  \__,_|_|_| |_|_| |_|\__,_|___/                    ┃
┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃                            BACKTRACKING                                      ┃
┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃ 1. FUNÇÃO WRAPPER                                                            ┃
┃                                                                              ┃
┃       A função rainhas_bt assinada em rainhas.h é um wrapper para a função   ┃
┃ que resolve o problema das n rainhas com casas proibidas utilizando          ┃
┃ backtracking.                                                                ┃
┃       O wrapper aloca um vetor resposta auxiliar para tratar os casos em que ┃
┃ é impossível uma solução completa dado o vetor de casas proibidas. Um exemplo┃
┃ é um tabuleiro de tamanho n em que uma das linhas está inteira proibida, essa┃
┃ instância permite uma solução máxima de n-1 rainhas posicionadas para n>3.   ┃
┃       O backtracking em si retorna o vetor resposta se a solução for completa┃
┃ , ou NULL caso contrário. Assim, se o retorno for NULL, a função wrapper     ┃
┃ copia o vetor auxiliar que contém a melhor resposta incompleta para o vetor  ┃
┃ resposta e o retorna.                                                        ┃
┃       Algumas operações feitas em rainhas_bt são otimizações relacionadas ao ┃
┃ tratamento das casas proibidas e serão discutidas na próxima sessão.         ┃
┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃ 2. FUNÇÃO WRAPPED                                                            ┃
┃                                                                              ┃
┃       A função que realiza o backtracking segue um algoritmo muito semelhante┃
┃ ao descrito na especificação do trabalho, com algumas alterações considerando┃
┃ a possibilidade de soluções parciais.                                        ┃
┃       Tudo que está diferente para essa implementação específica está        ┃
┃ explicado em comentários no próprio código.                                  ┃
┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃ 3. OTIMIZAÇÕES                                                               ┃
┃                                                                              ┃
┃       3.1 CASAS PROIBIDAS                                                    ┃
┃       O modo com que as casas proibidas são recebidas pela função rainhas_bt ┃
┃ obriga que cada passo do backtracking realize k iterações para que se decida ┃
┃ se a casa é proibida ou não, ao invés disso, é possível tomar essa decisão em┃
┃ O(1) se houver uma matriz booleana correspondente ao tabuleiro.              ┃
┃       A função rainhas_bt chama uma subrotina map, que aloca uma matriz n*n e┃
┃ mapeia o vetor de casas proibidas nela, colocando o valor 1 nas casas        ┃
┃ proibidas e 0 nas outras.                                                    ┃
┃       Após isso, o backtracking pode chamar outra subrotina chamada          ┃
┃ check_forbidden para cada casa que precisa ser checada, essa rotina acessa   ┃
┃ a posição correspondente na matriz e diz se uma rainha pode ou não ser       ┃
┃ posicionada ali.                                                             ┃
┃                                                                              ┃
┃       3.2 BACKTRACKING COM PODA                                              ┃
┃       Um galho do backtracking pode ser cortado se a solução até aquele      ┃
┃ momento não pode superar a melhor solução já atingida. A seguinte linha do   ┃
┃ código implementa essa ideia:                                                ┃
┃ ```C                                                                         ┃
┃   if((int) (n)-(ll-1) + q <= *best) return NULL ;                            ┃
┃ ```                                                                          ┃
┃       Se a quantidade de linhas que ainda faltam na recursão (n-ll) somada   ┃
┃ à quantidade de rainhas já posicionadas for menor que o tamanho da melhor    ┃
┃ solução, então aquele galho não pode ser o melhor e pode ser "podado"        ┃
┃       Para testar se a poda funciona, um script bash foi criado para rodar o ┃
┃ algoritmo 100 vezes e testar o tempo de resposta. O arquivo de teste solicita┃
┃ a operação para um tabuleiro 16x16 e com a primeira linha inteira proibida,  ┃
┃ o que gera um caso complexo porque a solução máxima tem n-1 rainhas          ┃
┃ posicionadas.                                                                ┃
┃       O script e os resultados estão descritos abaixo:                       ┃
┃                                                                              ┃
┃       3.2.1 SCRIPT                                                           ┃
┃       ```bash                                                                ┃
┃           summ=0                                                             ┃
┃           for i in `seq 1 100`                                               ┃
┃           do                                                                 ┃
┃               out=`./teste`                                                  ┃
┃               echo $out                                                      ┃
┃               num=$(echo $out | grep backtracking | cut -d " " -f2)          ┃
┃               summ=$((summ+num))                                             ┃
┃               sleep 1                                                        ┃
┃           done                                                               ┃
┃           printf "%d\n" $((summ/100))                                        ┃
┃       ```                                                                    ┃
┃                                                                              ┃
┃       3.2.2 RESULTADOS                                                       ┃
┃       ┏━━━━━━━┳━━━━━━━━━━━┳━━━━━━━━━━┓                                       ┃
┃       ┃       ┃ SEM PODA  ┃ COM PODA ┃                                       ┃
┃       ┃CLOCKS ┃ 2748892   ┃ 6567     ┃                                       ┃
┃       ┗━━━━━━━┻━━━━━━━━━━━┻━━━━━━━━━━┛                                       ┃
┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃                           CONJUNTO INDEPENDENTE                              ┃
┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃ 1. FUNÇÃO WRAPPER                                                            ┃
┃       A função rainhas_ci assinada em rainhas.h é um wrapper para a recursão ┃
┃ com conjuntos independentes.                                                 ┃
┃       Semelhante ao feito com a função com backtracking, um vetor auxiliar   ┃
┃ também é alocado para tratar os casos de soluções incompletas, de forma que  ┃
┃ se o algoritmo devolver "NÃO", o vetor auxiliar é copiado para o vetor       ┃
┃ resposta.                                                                    ┃
┃       A rotina também realiza operações com o vetor de casas proibidas a fim ┃
┃ de otimizar checagens, isso será discutido na sessão de otimizações.         ┃
┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃ 2. ESTRUTURAS DE DADO                                                        ┃
┃       Para implementar as listas de adjacência, foram criadas duas estruturas┃
┃ de dados que juntas representam o estado de um grafo, são elas:              ┃
┃                                                                              ┃
┃       ```C                                                                   ┃
┃           /*                                                                 ┃
┃           * Struct da fila                                                   ┃ 
┃           */                                                                 ┃  
┃           typedef struct queue_t {                                           ┃
┃               struct queue_t *next ;                                         ┃
┃               struct nodo_t *points ;                                        ┃
┃           } queue_t ;                                                        ┃ 
┃                                                                              ┃     
┃           /*                                                                 ┃
┃           * Struct do nodo                                                   ┃
┃           */                                                                 ┃
┃           typedef struct nodo_t {                                            ┃
┃               struct queue_t *adj ;                                          ┃
┃               struct nodo_t *proibiu ;                                       ┃
┃               uint l, c ;                                                    ┃
┃               short valid, percorrido ;                                      ┃
┃               int vizinhos ;                                                 ┃
┃           } nodo_t ;                                                         ┃ 
┃       ```                                                                    ┃
┃                                                                              ┃
┃       As structs do tipo nodo estão no vetor principal que representa as     ┃
┃ casas do tabuleiro, enquanto as structs do tipo queue são posições na fila de┃
┃ vértices adjacentes em cada nodo.                                            ┃
┃       A explicação exata de cada componente está comentada no código fonte.  ┃
┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃ 3. REPRESENTAÇÃO                                                             ┃
┃       A função rainhas_ci cria um grafo usando a função init_graph.          ┃
┃       O correspondete a um tabuleiro 3x3 criado usando essa funçaõ seria:    ┃
┃                                                                              ┃
┃  ┏━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓┃
┃  ┃ NODO_T ┃                        QUEUE_T                                  ┃┃
┃  ┣━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫┃
┃  ┃ [1][1] ┃->[1][2]->[1][3]->[2][1]->[2][2]->[3][1]->[3][3]                 ┃┃
┃  ┃ [1][2] ┃->[1][1]->[1][3]->[2][1]->[2][2]->[2][3]->[3][2]                 ┃┃
┃  ┃ [1][3] ┃->[1][1]->[1][2]->[2][2]->[2][3]->[3][1]->[3][3]                 ┃┃
┃  ┃ [2][1] ┃->[1][1]->[1][2]->[2][2]->[2][3]->[3][1]->[3][2]                 ┃┃
┃  ┃ [2][2] ┃->[1][1]->[1][2]->[1][3]->[2][1]->[2][3]->[3][1]->[3][2]->[3][3] ┃┃
┃  ┃ [2][3] ┃->[1][2]->[1][3]->[2][1]->[2][2]->[3][2]->[3][3]                 ┃┃ 
┃  ┃ [3][1] ┃->[1][1]->[3][3]->[2][1]->[2][2]->[3][2]->[3][3]                 ┃┃
┃  ┃ [3][2] ┃->[1][2]->[2][1]->[2][2]->[2][3]->[3][1]->[3][3]                 ┃┃
┃  ┃ [3][3] ┃->[1][1]->[1][3]->[2][2]->[2][3]->[3][1]->[3][2]                 ┃┃
┃  ┗━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛┃
┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃ 4. OTIMIZAÇÕES                                                               ┃
┃       4.1 CASAS PROIBIDAS                                                    ┃
┃       Da mesma forma que feito no backtracking, a função rainhas_ci chama uma┃
┃ função que mapeia as casas proibidas para a instância usada, no caso, como a ┃
┃ struct nodo_t tem um campo que diz se a casa é valida, basta percorrer o     ┃
┃ vetor de casas proibidas setando os nodos correspondentes para inválido.     ┃      
┃                                                                              ┃
┃       4.2 PODA                                                               ┃
┃       A função mantém o tamanho da melhor solução encontrada, caso um galho  ┃
┃ da árvore de solução tenha uma instância que não consigo chegar à melhor, ele┃
┃ é podado:                                                                    ┃
┃       ```C                                                                   ┃
┃           if(q + tam_c <= *best) return NULL ;                               ┃
┃       ```                                                                    ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
