CC = gcc
CFLAGS = -Wall -O3
MODE = -g

cifras: cifras.o cifras_method.o atack_method.o
	$(CC) $(CFLAGS) $(MODE) -o $@ $^

cifras.o: cifras.c
	$(CC) $(CFLAGS) $(MODE) -c $<

cifras_method.o: cifras_method.c cifras_method.h
	$(CC) $(CFLAGS) $(MODE) -c $<

atack_method_1.o: atack_method.c atack_method.h
	$(CC) $(CFLAGS) $(MODE) -c $<

clean:
	rm -f *.o cifras
