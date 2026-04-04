all:
	gcc src/programaTrab.c src/cabecalho.c src/registro_dados.c src/auxiliares.c src/fornecidas.c src/funcionalidades.c -o main

run:
	./main

zip:
	zip trabalho.zip src/* makefile