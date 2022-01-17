testlib : main.o list.o hash.o bitmap.o hex_dump.o debug.o
	gcc -o testlib main.o list.o hash.o bitmap.o hex_dump.o debug.o
main.o : list.h hash.h bitmap.h main.c
	gcc -c -o main.o main.c
list.o : list.h list.c
	gcc -c -o list.o list.c
hash.o : hash.h hash.c
	gcc -c -o hash.o hash.c
bitmap.o : bitmap.h bitmap.c
	gcc -c -o bitmap.o bitmap.c
hex_dump.o : hex_dump.h hex_dump.c
	gcc -c -o hex_dump.o hex_dump.c
debug.o : debug.h debug.c
	gcc -c -o debug.o debug.c
clean :
	rm *.o testlib
