
#pragma once

#include "common.h"

class IElement {
public:
	virtual ~IElement() {}
	
	virtual void controlSignal() = 0;
	
	virtual void process(sample_t *in, sample_t *out, int size) = 0;
};

extern "C" IElement *createElement();
