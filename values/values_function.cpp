#include "values.h"

#include <math.h>

////////////////////////////////////
// FUNCTION CLASS
////////////////////////////////////
Function::Function(std::string nm, Node* n, const std::vector<std::string>& args_n)
	: Value(ValueType::FUNCTIONTYPE), name(nm), body(n), arg_names(args_n) {}

void Function::print(std::ostream& os) const 
{
	os << "<function:" << name << ">";
}

std::string Function::get_func_name() const
{
	return name;
}

Node* Function::get_func_body() const 
{
	return body;
}

std::vector<std::string> Function::get_func_argnames() const
{
	return arg_names;
}

bool Function::is_true() const
{
	return true;
}

Value* Function::less_than(const Value* other) const
{
	return new Null();
}

Value* Function::greater_than(const Value* other) const
{
	return new Null();
}

Value* Function::equals(const Value* other) const
{
	return new Null();
}

Value* Function::copy() const
{
	return new Function(name, body, arg_names);
}