#ifndef BUILTINFUNCS_H
#define BUILTINFUNCS_H

#include "interpreter.h"

class BFunctions
{
public:
	static RTResult PRINT(const Context* context);
	static RTResult INPUTSTR(const Context* context);
};	

#endif