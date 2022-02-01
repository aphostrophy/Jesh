run : shell
	./shell

shell : src/shell.c
	gcc -o shell src/shell.c