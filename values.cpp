#include "values.h"

////////////////////////////////////
// VALUE BASE CLASS
////////////////////////////////////
Value::Value() {}
Value::Value(std::string t, Position s, Position e) : type(t), start(s), end(e) {}

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