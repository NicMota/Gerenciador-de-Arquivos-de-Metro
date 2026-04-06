all:
	gcc src/*.c -o executorprojeto -lm

run:
	./executorprojeto

zip:
	zip trabalho.zip src/* makefile build.sh Doxyfile