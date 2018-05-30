 #!/bin/bash          
STR=0
j=0
while [ $j -lt 100 ]; do
	./Game "Cambio924834" "Cambio924835" "Cambio924834" "Cambio924835" "-s" $j "-i" "default.cnf" |& grep "info: player(s) Cambio924834 got top score"  > /dev/null && let STR+=1
	let j+=1
done



echo $STR
