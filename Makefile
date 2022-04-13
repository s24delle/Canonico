CC         = g++ 
CFLAGS     = --std=c++11 -g -Wall
CFLAGSROOT = `root-config --cflags`
LIBSROOT   = `root-config --glibs`

all: argon

libreria.o: libreria.cpp
	$(CC) $(CFLAGS) -c libreria.cpp $(CFLAGSROOT) $(LIBSROOT)
argon: argon.cpp libreria.o
	$(CC) $(CFLAGS) -o cia argon.cpp libreria.o $(CFLAGSROOT) $(LIBSROOT)

clean:
	rm *.o
