 #!/bin/bash          
STR=0
j=0
while [ $j -lt 100 ]; do

	
	./Game "CatStevens" "Cambio924834" "CatStevens" "Cambio924835" "-s" $j "-i" "default.cnf" |& grep "info: player(s) CatStevens got top score"  > /dev/null && let STR+=1
	let j+=1
done

j=0
while [ $j -lt 100 ]; do

	
	./Game "Cambio924834" "CatStevens" "CatStevens" "Cambio924835" "-s" $j "-i" "default.cnf" |& grep "info: player(s) CatStevens got top score"  > /dev/null && let STR+=1
	let j+=1
done
j=0
while [ $j -lt 100 ]; do	

	
	./Game "CatStevens" "Cambio924835" "Cambio924834" "CatStevens" "-s" $j "-i" "default.cnf" |& grep "info: player(s) CatStevens got top score"  > /dev/null && let STR+=1
	let j+=1
done
j=0
while [ $j -lt 100 ]; do

	
	./Game "Cambio924835" "CatStevens" "Cambio924834" "CatStevens" "-s" $j "-i" "default.cnf" |& grep "info: player(s) CatStevens got top score"  > /dev/null && let STR+=1
	let j+=1
done


echo $STR
