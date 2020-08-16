#include "values.h"

////////////////////////////////////
// NULL CLASS
////////////////////////////////////
Null::Null() : Value(ValueType::NULLTYPE) {}

void Null::print(std::ostream& os) const 
{
	os << "<null>";
}