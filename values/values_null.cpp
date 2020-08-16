#include "values.h"

////////////////////////////////////
// NULL CLASS
////////////////////////////////////
Null::Null() : Value(ValueType::NULLTYPE) {}

void Null::print(std::ostream& os) const 
{
	os << "<null>";
}

bool Null::is_true() const
{
	return false;
}

Value* Null::less_than(const Value* other) const
{
	return new Integer(0);
}

Value* Null::greater_than(const Value* other) const
{
	return new Integer(0);
}

Value* Null::equals(const Value* other) const
{
	if (other->get_type() == ValueType::NULLTYPE) return new Integer(1);
	return new Integer(0);
}

Value* Null::copy() const
{
	return new Null();
}