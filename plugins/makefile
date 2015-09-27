

%.so: %.cpp
	g++ -shared $< -o $@ -I../include -fPIC -std=c++11
	
run: hello.so
