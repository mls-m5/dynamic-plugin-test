
#include "plugincontainer.h"
#include "iplugin.h"
#include <iostream>
#include <dlfcn.h> //for dlopen dlsym


using namespace std;

static void checkError() {
    char *error;
    if ((error = dlerror()) != NULL)  {
        cerr << error << endl;
        exit(1);
    }
}

PluginContainer::PluginContainer(std::string filename) {
	cout << "laddar plugin " << filename << endl;
	
	void *lib = dlopen(filename.c_str(), RTLD_NOW);
	
	checkError();
	
	create = (decltype(&createPlugin)) dlsym(lib, "createPlugin");
}


PluginContainer::~PluginContainer() {
	cout << "stänger plugin" << endl;
}
