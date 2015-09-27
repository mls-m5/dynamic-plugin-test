
#pragma once

#include <string>
#include <memory>

class IEngine {
public:
	virtual ~IEngine() {}
	
	virtual bool Init(std::string name = "") = 0;
	virtual bool Activate() = 0;
	virtual bool Close() = 0;
	virtual void AddElement(class IElement *e) = 0;
	virtual void RemoveElement(class IElement *e) = 0;
	virtual void SetVolume(double v) = 0;
	virtual double GetVolume() = 0;
};


extern "C" IEngine *createEngine();
