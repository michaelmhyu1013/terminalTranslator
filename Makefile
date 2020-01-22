CC=gcc -Wall

clean:
	rm -f *.o *.gch core.* translator
translator: translator.o input.o output.o translate.o
	$(CC) -o translator translator.o input.o output.o translate.o $(CLIB)
translator.o:
	$(CC) -c translator.c
input.o: 
	$(CC) -c input.c
output.o:
	$(CC) -c output.c
translate.o:
	$(CC) -c translate.c
	