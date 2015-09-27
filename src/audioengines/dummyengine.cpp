

#include "iengine.h"

class DummyEngine: public IEngine {
	public:
	
	DummyEngine() {}

	virtual ~DummyEngine() {}
	
	bool init(std::string name = "") override {}
	
	bool activate() override {}
	
	bool close() override {}
	
	void addElement(class IElement *e) override {}
	
	void removeElement(class IElement *e) override {}
	
	void setVolume(double v) override {}
	
	double getVolume() override {}
};


IEngine *createEngine() {
	return new DummyEngine();
}

