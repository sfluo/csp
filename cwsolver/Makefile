
CC=g++
CFLAGS=-I ./
LIBS=

#ODIR=obj
OBJ=main.o cword.o

%.o:%.cc
	$(CC) -c -o $@ $< $(CFLAGS)

cword: $(OBJ) 
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -rf *.o *~ cword
