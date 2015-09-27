

#include <iostream>

#include "plugincontainer.h"

using namespace std;

int main(int argc, char **argv) {
	auto container = PluginContainer("plugins/hello.so");
	
	auto plugin = container.create();
	
	plugin->run();
	
	delete plugin;
}
