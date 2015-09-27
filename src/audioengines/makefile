
all: jackengine.so dummyengine.so paengine.so

%.so: %.cpp
	g++ -shared $< -o $@ -I../../include -fPIC -std=c++11
	
	
jackengine.so: jackengine.cpp
	g++ -shared $< -o $@ -I../../include -fPIC -std=c++11 -ljack
	
	
paengine.so: paengine.cpp
	g++ -shared $< -o $@ -I../../include -fPIC -std=c++11 -lportaudio
	
run:
	make -C ../ run

clean:
	rm -f *.so
