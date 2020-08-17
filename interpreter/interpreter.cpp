#include "interpreter.h"

////////////////////////////////////
// RUNTIME RESULT CLASS
////////////////////////////////////
RTResult::RTResult() 
	: value(NULL), return_value(NULL), error(NULL), to_continue(false), to_break(false) {}

// Propagate values
Value* RTResult::register_value(const RTResult& result)
{
	error = result.get_error();
	value = result.get_value();
	return_value = result.get_return_value();
	to_continue = result.get_continue();
	to_break = result.get_break();
	return value;
}

// Reset values
void RTResult::reset()
{	
	error = NULL;
	value = NULL;
	return_value = NULL;
	to_continue = false;
	to_break = false;
}

// Stops propagating
bool RTResult::should_return() const 
{
	return error || return_value || to_continue || to_break;
}

// Propagates value
void RTResult::success(Value* val)
{
	reset();
	value = val;
}

// Propagates error
void RTResult::failure(Error* err)
{
	reset();
	error = err;
}

// Propagates return value
void RTResult::success_return(Value* val)
{
	reset();
	return_value = val;
}

// Propagates break flag
void RTResult::success_break()
{
	reset();
	to_break = true;
}

// Propagate continue flag
void RTResult::success_continue()
{
	reset();
	to_continue = true;
}

// Getters
Value* RTResult::get_value() const
{
	return value;
}

Value* RTResult::get_return_value() const
{
	return return_value;
}

Error* RTResult::get_error() const
{
	return error;
}

bool RTResult::get_continue() const
{
	return to_continue;
}

bool RTResult::get_break() const
{
	return to_break;
}

// Print value
void RTResult::print(std::ostream& os) const 
{
	if (error) os << *error;
	else if (value) 
	{
		if (value->get_list_values().size() == 1) os << *value->get_list_values()[0];
		else os << *value;
	}
	else os << "No value";
}

std::ostream& operator<<(std::ostream& os, const RTResult& res)
{
	res.print(os);
	return os;
}