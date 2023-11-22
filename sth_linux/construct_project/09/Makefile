.PHONY: all

static:
	ar rcs lib/static/libhello.a obj/hello.o obj/sum.o

link:
	gcc obj/main.o -Llib/static -lhello -o bin/app

all:
	gcc -c main.c -o obj/main.o -I./inc
	gcc -c src/hello.c -o obj/hello.o -I./inc
	gcc -c src/sum.c -o obj/sum.o -I./inc


clean: 
	rm -f ./bin/* ./obj/* *.exe 