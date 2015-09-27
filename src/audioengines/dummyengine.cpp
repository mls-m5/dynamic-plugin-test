

#include "iengine.h"

class DummyEngine: public IEngine {
	public:
	
	DummyEngine() {}

	virtual ~DummyEngine() {}
	
	bool Init(std::string name = "") override {}
	
	bool Activate() override {}
	
	bool Close() override {}
	
	void AddElement(class IElement *e) override {}
	
	void RemoveElement(class IElement *e) override {}
	
	void SetVolume(double v) override {}
	
	double GetVolume() override {}
};


IEngine *createEngine() {
	return new DummyEngine();
}

