audioclock:	audioclock.c
	gcc -o audioclock -g -Wall audioclock.c -lm

test:	audioclock
	./audioclock | pacat --rate=48000 --channels=1 --raw
