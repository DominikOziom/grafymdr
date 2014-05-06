__start__: obj Grafy
	./Grafy

CPPFLAGS=-Wall -pedantic -Iinc -c
LDFLAGS=-Wall



obj:
	mkdir obj

Grafy:  obj/grafy.o 
	g++ ${LDFLAGS} -o Grafy obj/grafy.o

obj/grafy.o: graf.cpp 
	g++ ${CPPFLAGS} -o obj/grafy.o graf.cpp

clean:
	rm -f Grafy obj/* core*
