PROJECT := simulator
CC = gcc
CFLAGS = -Wall -pedantic
DEBUG = -ggdb3 -D DEBUG
LDFLAGS = -pthread


all: $(PROJECT) cleanobj

$(PROJECT): parser.o core.o scheduler.o main.o
	$(CC) $(CFLAGS) $(LDFLAGS) *.o -o $(PROJECT)

core.o: parser.c core.c core.h
	$(CC) -c $(CFLAGS) core.c

parser.o:  parser.c strutture_dati.h
	$(CC) -c $(CFLAGS) parser.c

scheduler.o: parser.c scheduler.c scheduler.h
	$(CC) -c $(CFLAGS) scheduler.c


main.o: parser.c scheduler.c main.c
	$(CC) -c $(CFLAGS) main.c


clean: cleanobj
	$(RM) $(PROJECT)
cleanobj: 
	$(RM) *.o *.h.gch






