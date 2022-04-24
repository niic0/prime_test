run: compile
	./prime_test -k 70 -n 1114111 -t

compile: utils.o main.o
	gcc utils.o main.o -Wall -lgmp -o prime_test

clean:
	rm prime_test 
	rm *.o

%.o: %.c
	gcc -c $< -Wall -g

profile: compile
	valgrind --track-origins=yes --leak-check=full ./prime_test