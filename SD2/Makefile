.PHONY: build clean

build: hll freq hash

freq: freq.c
	gcc -std=c11 -o $@ -Wall -Wextra $<

hll: hll.c
	gcc -std=c11 -o $@ -Wall -Wextra $<

hash: hash.c
	gcc -std=c11 -o $@ -Wall -Wextra $<

clean:
	rm freq
	rm hash
	rm hll
