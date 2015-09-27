
#pragma once

#include <string>
#include <memory>

class IEngine {
public:
	virtual ~IEngine() {}
	
	virtual bool init(std::string name = "") = 0;
	virtual bool activate() = 0;
	virtual bool close() = 0;
	virtual void addElement(class IElement *e) = 0;
	virtual void removeElement(class IElement *e) = 0;
	virtual void setVolume(double v) = 0;
	virtual double getVolume() = 0;
};


extern "C" IEngine *createEngine();
