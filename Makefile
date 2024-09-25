audioclock:	audioclock.c
	gcc -o audioclock -g -Wall audioclock.c

test:	audioclock
	./audioclock | pacat --rate=48000 --channels=1 --raw
