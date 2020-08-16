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


std::ostream& operator<<(std::ostream& os, const Value &val)
{
	val.print(os);
	return os;
}
