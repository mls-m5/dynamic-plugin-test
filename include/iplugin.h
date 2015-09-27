
#pragma once

class IPlugin {
	public:
	virtual ~IPlugin() {};
	
	virtual void run() = 0;
};


extern "C" IPlugin *createPlugin();
