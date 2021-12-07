all: myshell
myshell: myshell.c islem tekrar
	gcc -o myshell myshell.c
tekrar: tekrar.c 
	gcc -o tekrar tekrar.c
islem : islem.c topla cikar
	gcc -o islem  islem.c
cikar : cikar.c 
	gcc -o cikar cikar.c
topla : topla.c 
	gcc -o topla topla.c