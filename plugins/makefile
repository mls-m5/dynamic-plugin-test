
all: hello.so testelement.so

%.so: %.cpp
	g++ -shared $< -o $@ -I../include -fPIC -std=c++11
	
run:
	make -C ../ run
