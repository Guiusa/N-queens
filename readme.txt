# Guiusepe Oneda Dal Pai    - GRR20210572
# Pedro Willian Aguiar      - GRR20211766
  _   _
 | \ | |
 |  \| |______ __ _ _   _  ___  ___ _ __  ___
 | . ` |______/ _` | | | |/ _ \/ _ \ '_ \/ __|
 | |\  |     | (_| | |_| |  __/  __/ | | \__ \
 |_| \_|      \__, |\__,_|\___|\___|_| |_|___/
                 | |
                 |_|

################################################################################
# BACKTRACKING
    1.
        A função disponível em rainhas.h é um wrapper do backtracking
    implementado, que adiciona algumas variáveis para controle da recursão e
    cálculo de casas proibidas.
        Para a implementação do backtracking no problema das N-rainhas, foi
    seguido o modelo disponível na página de especificação do trabalho, com
    algumas tomadas de decisão baseadas no melhor desempenho que pudesse ser
    alcançado. A próxima sessão visa explicar os trechos que tem alguma
    otimização envolvida.

|------------------------------------------------------------------------------|

    2.

    ____________________________________________________________________________
    2.1 - Controle de nível do backtracking
        Para controlar em qual nível o backtracking está, foi usada uma variável
    chamada ll ((l)ast (l)ine). A variável indica qual é a próxima linha
    em que deve ser posicionada uma rainha, ela é incrementada em 1 para cada
    nível de chamada recursiva.
    ```
        if(valid){
            r[ll-2] = (unsigned int) j ;
            unsigned int *r1 = rainhas_bt_wrapped(n, k, c, r, ll+1, fbd) ;
            if(r1) return r1 ;                                ^^^^
        }
    ```

    ____________________________________________________________________________
    2.2 - Checar fim do backtracking
    ```
        Se t é solução
            Devolva t
    ```

        Checar se t é uma solução válida olhando para todas as posições do vetor
    resposta custaria muito por iteração, ao invés disso, a função checa se a
    variável ll chegou ao mesmo valor de n. Nesse caso, ll indica uma linha
    inválida para acesso e que a recursão chegou ao fim.

    ```
        if(ll > (int) (n)) return r ;
    ```

    ____________________________________________________________________________
    2.3 - Rainha a se posicionar não está atacada
        Sabe-se que uma rainha "a" é atacada por uma "b" se a rainha "a" também
    ataca "b".
        Além disso, a recurão preenche o tabuleiro de cima pra baixo, portanto,
    para checar se a rainha pode ser posicionada, basta checar todas as posições
    de linhas mais altas que ela atacaria.

    ```
        valid = 1 ;
        int aux ;
        int gap ;
        for(aux = ll-1, gap = 1; aux > 0; aux--, gap++){
            if((int) r[aux-1]==j || (int) r[aux-1]==j-gap || (int) r[aux-1]==j+gap){
                valid = 0 ;
                break ;
            }
        }
    ```

    ____________________________________________________________________________
    2.4 - Checar se a casa a se posicionar uma rainha é proibida
        Da forma que a lista de casas proibidas é passada à função rainhas_bt,
    todo nível da recursão custaria até O(n²) para ter certeza que ela está
    livre, o que torna o algoritmo extremamente lento.
        Uma opção considerada pra resolver isso é ordenar o vetor de casas
    proibidas, de forma que seria possível realizar uma busca binária de custo
    O(nlg(n)) para cada chamada recursiva.
        É possível fazer isso de forma ainda melhor usando uma lookup table
    fazendo a checagem em O(1), com um "trade" de mais memória ocupada pelo
    programa.
        A segunda opção foi escolhida visando performance. Foi criado um vetor
    chamado fbd que é passado para a função rainhas_bt_wrapped.
        A inicialização do vetor é feita com a função calloc, pois é conveniente
    que as posições tenham valor 0, o tamanho é n². Após a alocação, um loop de
    custo k preenche as casas proibidas com o valor 1, sendo k o número de casas
    proibidas.
        Quando a função rainhas_bt_wrapped termina, o vetor é liberado, inibindo
    leaks de memória.

|------------------------------------------------------------------------------|
    3. Desempenho
        De maneira informal, foi criado um script em bash que roda o programa
    100 vezes e calcula a média de clocks entre as execuções, o script e os
    resultados encontrados estão abaixo:

    ```
        #! /bin/bash
        summ=0

        for i in `seq 1 100`
        do
            out=`./teste`
            echo $out
            num=$(echo $out | grep backtracking | cut -d " " -f2)
            summ=$((summ+num))
            # Sem o sleep, parece haver interferência de cache, as execuções
            # ficam muito rápidas
            sleep 1
        done
        printf "%f\n" $((summ/100))
    ```

        | N-RAINHAS | N_RAINHAS COM CASAS PROIBIDAS |
        |-----------|-------------------------------|
  Clocks| 5597      | 5125                          |

        O desempenho da variante com casas proibidas é ligeiramente mais rápido
    provavelmente porque se trata de um backtracking com galhos, já que inúmeros
    casos deixam de ser testados por serem filhos de uma casa proibida.
################################################################################
