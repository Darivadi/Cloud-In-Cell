CC = gcc
CFLAGSDEBUG = -g -Wall -c -I/home/$(USER)/local/include/ -I/usr/include/ -DCIC -DBINARYDATA
CFLAGSASCII = -c -O3 -Wall -I/home/$(USER)/local/include/ -I/usr/include/ -DCIC -DASCIIDATA
CFLAGS = -c -O3 -I/home/$(USER)/libs/include/ -DCIC -DBINARYDATA
LFLAGS = -L/home/$(USER)/libs/lib
PROGRAM = CIC


$(PROGRAM):
	$(CC) -c -save-temps $@.c $(CFLAGS)
	$(CC) $@.o -lm -lfftw3 $(LFLAGS) -o $@.x

debug:
	echo Compiling for debug $(PROGRAM).c
	$(CC) $(CFLAGSDEBUG) $(PROGRAM).c -o $(PROGRAM).o
	$(CC) $(PROGRAM).o $(LFLAGS) -lm -o $(PROGRAM).x

asciidata:
	echo Compiling for debug $(PROGRAM).c
	$(CC) $(CFLAGSASCII) $(PROGRAM).c -o $(PROGRAM).o
	$(CC) $(PROGRAM).o $(LFLAGS) -lm -o $(PROGRAM).x	

clean:
	rm -rf *.out
	rm -rf *-
	rm -rf *.out
	rm -rf *#
	rm -rf *.o
	rm -rf *.a
	rm -rf *.so
	rm *~
	rm *.x
