#include "ielement.h"

#include <iostream>

using namespace std;


class TestElement: public IElement {
	
	virtual void controlSignal() override {	
		cout << "TestElement controlSignal" << endl;
	}
	
	virtual void process(sample_t *in, sample_t *out, int size) override {
		cout << "TestElement process" << endl;
		
		for (int i = 0; i < size; ++i) {
			out[i] = (float)(i % 100) / 100.;
		}
	}
};



IElement *createElement() {
	return new TestElement;
}
