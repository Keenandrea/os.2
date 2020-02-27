all: oss prime
oss: oss.c clock.c clock.h prime.h
	gcc -o oss clock.c clock.h oss.c
prime: prime.c prime.h
	gcc -o prime prime.c 
clean:
	-rm oss prime
