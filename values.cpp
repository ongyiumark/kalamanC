#include "values.h"
#include "position.h"
#include <iostream>

////////////////////////////////////
// VALUE BASE CLASS
////////////////////////////////////
Value::Value() {}
Value::Value(Position s, Position e) : start(s), end(e) {}

std::ostream& operator<<(std::ostream& os, const Value &val)
{
	val.print(os);
	return os;
}

////////////////////////////////////
// NUMBER CLASS
////////////////////////////////////
Integer::Integer(long long x) : Value(), value(x) {}

void Integer::print(std::ostream& os) const 
{
	os << value;
}

long long Integer::get_value()
{
	return value;
}