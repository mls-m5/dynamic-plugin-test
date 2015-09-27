
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

BaseContainer::BaseContainer(std::string filename, std::string createFunctionName) {
	cout << "laddar plugin " << filename << endl;
	
	library = dlopen(filename.c_str(), RTLD_NOW);
	
	checkError();
	
	createPointer = (decltype(createPointer)) dlsym(library, createFunctionName.c_str());
}


BaseContainer::~BaseContainer() {
	cout << "stänger plugin" << endl;
	
	dlclose(library);
}
