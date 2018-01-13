all:
	gcc main.c -lseabreeze -lusb -g -o main
	gcc color.c -lseabreeze -lusb -g -o color

