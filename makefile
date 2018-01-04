all: main.c
	gcc main.c -lseabreeze -lusb -o main
