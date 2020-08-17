#include "values.h"

#include <math.h>

////////////////////////////////////
// VALUE BASE CLASS
////////////////////////////////////
Value::Value(ValueType t) : type(t) {}

void Value::set_position(Position s, Position e) 
{
	start = s;
	end = e;
}

ValueType Value::get_type() const
{
	return type;
}

Position Value::get_start() const
{
	return start;
}

Position Value::get_end() const
{
	return end;
}

long long Value::get_int_value() const
{
	return 0;
}

long double Value::get_double_value() const
{
	return 0;
}

std::vector<Value*> Value::get_list_values() const
{
	std::vector<Value*> res = std::vector<Value*>();
	return res;
}

std::string Value::get_string_value() const
{
	return "";
}

std::string Value::get_func_name() const
{
	return "";
}

BuiltInName Value::get_bifunc_name() const
{
	return BuiltInName::UNDEFINED;
}

Node* Value::get_func_body() const 
{
	return NULL;
}

std::vector<std::string> Value::get_func_argnames() const
{
	return std::vector<std::string>();
}

Value* Value::add(const Value* other) const
{
	return new Null();
}

Value* Value::subtract(const Value* other) const
{
	return new Null();
}

Value* Value::multiply(const Value* other) const
{
	return new Null();
}

Value* Value::divide(const Value* other) const
{
	return new Null();
}

Value* Value::modulo(const Value* other) const
{
	return new Null();
}

Value* Value::power(const Value* other) const
{
	return new Null();
}

Value* Value::notted() const 
{
	return new Integer((int)!is_true());
}

Value* Value::and_with(const Value* other) const
{
	return new Integer((int)(is_true() && other->is_true()));
}

Value* Value::or_with(const Value* other) const
{
	return new Integer((int)(is_true() || other->is_true()));
}

Value* Value::xor_with(const Value* other) const
{
	return new Integer((int)(is_true() ^ other->is_true()));
}

Value* Value::not_equals(const Value* other) const
{
	if (equals(other)->get_type() == ValueType::NULLTYPE)
		return new Null();
	return equals(other)->notted();
}

Value* Value::less_equals(const Value* other) const
{
	if (greater_than(other)->get_type() == ValueType::NULLTYPE)
		return new Null();
	return greater_than(other)->notted();
}

Value* Value::greater_equals(const Value* other) const
{
	if (less_than(other)->get_type() == ValueType::NULLTYPE)
		return new Null();
	return less_than(other)->notted();
}

std::ostream& operator<<(std::ostream& os, const Value &val)
{
	val.print(os);
	return os;
}
