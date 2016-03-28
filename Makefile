project: mallocation.c
	gcc -o mallocation mallocation.c main.c
	gcc -o test mallocation.c test.c -lcunit
