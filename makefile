
OBJ = main.o src/plugincontainer.o
TARGET= librarytest
CXXFLAGS= -std=c++11 -Iinclude/
LIBS=  -ldl

all: .depend ${TARGET}
	make -C plugins/

${TARGET}: ${OBJ}
	g++ -o librarytest ${OBJ} ${LIBS}

.depend:
	matmake/matdep  ${CXXFLAGS} ${OBJ} > .depend
	
	

#include .depend

run: all
	./librarytest
	
clean:
	rm -f .depend
