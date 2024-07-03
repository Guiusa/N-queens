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
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
