#include "values.h"

#include <math.h>

////////////////////////////////////
// BUILT IN FUNCTION CLASS
////////////////////////////////////
BuiltInFunction::BuiltInFunction(BuiltInName nm, const std::vector<std::string>& args_n)
	: Value(ValueType::BIFUNCTYPE), name(nm), arg_names(args_n) {}

void BuiltInFunction::print(std::ostream& os) const 
{
	os << "<builtin_function:" << BUILTINNAMES[name] << ">";
}

BuiltInName BuiltInFunction::get_bifunc_name() const
{
	return name;
}

std::string BuiltInFunction::get_func_name() const
{
	return BUILTINNAMES[name];
}


std::vector<std::string> BuiltInFunction::get_func_argnames() const
{
	return arg_names;
}

bool BuiltInFunction::is_true() const
{
	return true;
}

Value* BuiltInFunction::less_than(const Value* other) const
{
	return new Null();
}

Value* BuiltInFunction::greater_than(const Value* other) const
{
	return new Null();
}

Value* BuiltInFunction::equals(const Value* other) const
{
	return new Null();
}

Value* BuiltInFunction::copy() const
{
	return new BuiltInFunction(name, arg_names);
}