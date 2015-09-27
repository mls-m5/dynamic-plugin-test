

#include <string>
#include "iplugin.h"

class PluginContainer {
	public:
	PluginContainer(std::string filename);
	~PluginContainer();

	
	IPlugin *(*create)();
};

