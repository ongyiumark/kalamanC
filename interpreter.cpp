#include "interpreter.h"

////////////////////////////////////
// RUNTIME RESULT CLASS
////////////////////////////////////
RTResult::RTResult() 
	: value(NULL), return_value(NULL), error(NULL), to_continue(false), to_break(false) {}

Value* RTResult::register_value(RTResult result)
{
	error = result.get_error();
	value = result.get_value();
	return_value = result.get_return_value();
	to_continue = result.get_continue();
	to_break = result.get_break();
	return value;
}

void RTResult::reset()
{	
	error = NULL;
	value = NULL;
	return_value = NULL;
	to_continue = false;
	to_break = false;
}

bool RTResult::should_return()
{
	return error || return_value || to_continue || to_break;
}

void RTResult::success(Value* val)
{
	reset();
	value = val;
}

void RTResult::failure(Error* err)
{
	reset();
	error = err;
}

void RTResult::success_return(Value* val)
{
	reset();
	return_value = val;
}

void RTResult::success_break()
{
	reset();
	to_break = true;
}

void RTResult::success_continue()
{
	reset();
	to_continue = true;
}

Value* RTResult::get_value()
{
	return value;
}

Value* RTResult::get_return_value()
{
	return return_value;
}

Error* RTResult::get_error()
{
	return error;
}

bool RTResult::get_continue()
{
	return to_continue;
}

bool RTResult::get_break()
{
	return to_break;
}

void RTResult::print(std::ostream& os) const 
{
	if (error) os << *error;
	else if (value) os << *value;
	else os << "No value";
}

std::ostream& operator<<(std::ostream& os, const RTResult& res)
{
	res.print(os);
	return os;
}