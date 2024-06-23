#! /bin/bash
summ=0

for i in `seq 1 100`
do
    out=`./teste`
    echo $out
    num=$(echo $out | grep backtracking | cut -d " " -f2)
    summ=$((summ+num))
    # Sem o sleep, parece haver interferência de cache, as execuções ficam muito
    # rápidas
    sleep 1
done
printf "%f\n" $((summ/100))
