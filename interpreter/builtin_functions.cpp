#include "builtin_functions.h"
#include "../values/values.h"

#include <iostream>

RTResult BFunctions::PRINT(Context* context)	
{
	RTResult result = RTResult();
	Value* val = context->get_table()->get_value("value");
	std::cout << *val << std::endl;
	result.success(new Null());
	return result;
}

RTResult BFunctions::INPUTSTR(Context* context)	
{
	RTResult result = RTResult();
	std::string val_str;
	getline(std::cin, val_str);
	result.success(new String(val_str));
	return result;
}