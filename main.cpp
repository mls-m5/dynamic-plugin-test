

#include <iostream>

#include "plugincontainer.h"
#include "engineloader.h"

using namespace std;

int main(int argc, char **argv) {
	auto container = PluginContainer("plugins/hello.so");
	//auto engineLoader = EngineLoader("src/audioengines/jackengine.so");
	auto engineLoader = EngineLoader("src/audioengines/paengine.so");
	
	auto plugin = container.create();
	auto engine = engineLoader.create();
	
	engine->Init("sound out");
	engine->Activate();
	engine->Close();
	
	plugin->run();
	
	delete plugin;
}
