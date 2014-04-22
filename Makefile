all: threadsort

threadsort: threadsort.c
	gcc threadsort.c -std=c99 -o threadsort -pthread

clean:
	rm threadsort