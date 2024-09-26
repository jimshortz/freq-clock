audioclock:	audioclock.c
	gcc -o audioclock -g -Wall audioclock.c -lpulse-simple -lm

clean:
	-rm audioclock *.o

