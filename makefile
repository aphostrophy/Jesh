run : shell
	./bin/shell

shell : src/shell.c
	gcc -o bin/shell src/shell.c