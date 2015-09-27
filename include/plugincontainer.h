

#include <string>
#include "iplugin.h"
#include "ielement.h"
#include "iengine.h"

class BaseContainer {
	public:
	BaseContainer(std::string filename, std::string createFunctionName);
	virtual ~BaseContainer();

protected:
	
	void *(*createPointer)();
	
	void *library;
};


template <class T>
class Container: public BaseContainer{
public:
	virtual ~Container() {}
	Container(std::string filename, std::string createFunctionName):
		BaseContainer(filename, createFunctionName) {
	}
	
	T *create() {
		typedef T *(*createFunctionType)();
		return ((createFunctionType)(createPointer))();
	}
};


//Specific case classes
class PluginContainer: public Container<IPlugin> {
public:
	virtual ~PluginContainer() {}
	PluginContainer(std::string filename):
		Container(filename, "createPlugin") {}
};


class ElementContainer: public Container<IElement> {
public:
	virtual ~ElementContainer() {}
	ElementContainer(std::string filename):
		Container(filename, "createElement") {}
};


class EngineContainer: public Container<IEngine> {
public:
	virtual ~EngineContainer() {}
	EngineContainer(std::string filename):
		Container(filename, "createEngine") {}
};
