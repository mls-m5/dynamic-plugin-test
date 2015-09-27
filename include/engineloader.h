#pragma once

#include <string>
#include "iengine.h"

class EngineLoader {
public:

	EngineLoader(std::string filename);
	
	~EngineLoader();
	
	IEngine * (*create)();
	
protected:
	void *library;
};
