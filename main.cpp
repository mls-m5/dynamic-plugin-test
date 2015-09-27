

#include <iostream>

#include "plugincontainer.h"
#include <chrono>
#include <thread>
#include <memory>


using namespace std;

int main(int argc, char **argv) {
	//auto container = PluginContainer("plugins/hello.so");
	//auto plugin = container.create();
	//plugin->run();
	//delete plugin;
	
	
	auto elementContainer = ElementContainer("plugins/testelement.so");
	
	
	unique_ptr<EngineContainer> engineContainer;
	if (0) {
		engineContainer.reset(new EngineContainer("src/audioengines/jackengine.so"));
	}
	else {
		engineContainer.reset(new EngineContainer("src/audioengines/paengine.so"));
	}
	
	auto engine = engineContainer->create();
	
	
	auto element = elementContainer.create();
	engine->addElement(element);
	
	
	engine->init("sound out");
	engine->activate();
	
	
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	engine->close();
	
	delete element;
}
