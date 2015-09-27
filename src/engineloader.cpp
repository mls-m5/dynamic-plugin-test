#include "engineloader.h"


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

EngineLoader::EngineLoader(std::string filename) {

	cout << "laddar motor " << filename << endl;
	
	
	library = dlopen(filename.c_str(), RTLD_NOW);
	
	checkError();
	
	create = (decltype(&createEngine)) dlsym(library, "createEngine");
	
	checkError();
}

EngineLoader::~EngineLoader() {
	dlclose(library);
	checkError();
}

