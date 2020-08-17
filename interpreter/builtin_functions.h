#ifndef BUILTINFUNCS_H
#define BUILTINFUNCS_H

#include "interpreter.h"

class BFunctions
{
public:
	static RTResult PRINT(Context* context);
	static RTResult INPUTSTR(Context* context);
};	

#endif