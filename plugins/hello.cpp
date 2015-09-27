#include "iplugin.h"

#include <iostream>
using namespace std;

class HelloPlugin: public IPlugin {
	public:
	
	HelloPlugin()  {
		cout << "created HelloPlugin" << endl;
	}
	
	virtual ~HelloPlugin(){
		cout << "goodbye world" << endl;
	}
	
	void run() override {
		cout <<  "hello world" << endl;
	}
};


IPlugin *createPlugin() {
	return new HelloPlugin;
}


