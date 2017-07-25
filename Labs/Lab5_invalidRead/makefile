all: le writebin hexdump dumphex newstring

le: letterfreq.c
	gcc -o le letterfreq.c
writebin: writebin.c
	gcc -o writebin writebin.c -Wall
hexdump: hexdump.c
	gcc -o hexdump hexdump.c -Wall
dumphex: dumphex.c
	gcc -o dumphex dumphex.c -Wall
newstring: newstring.c newstring.h
	gcc -o newstring newstring.c -Wall

clean:
	rm -f le writebin hexdump dumphex newstring
