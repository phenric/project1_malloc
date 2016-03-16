project: mallocation.c
	gcc -o mallocation mallocation.c main.c

test: test.c
	gcc -o test mallocation.c test.c -lcunit
