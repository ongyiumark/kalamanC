#include "values.h"

#include <math.h>

////////////////////////////////////
// DOUBLE CLASS
////////////////////////////////////
Double::Double(long double x) : Value(ValueType::DOUBLETYPE), value(x) {}

void Double::print(std::ostream& os) const 
{
	os << std::fixed << value;
}

long double Double::get_double_value() const
{
	return value;
}

Value* Double::add(const Value* other) const
{
	switch (other->get_type())
	{
		case ValueType::INTEGERTYPE:
			return new Double(value + other->get_int_value());
		case ValueType::DOUBLETYPE:
			return new Double(value + other->get_double_value());
		default:
			return new Null();
	}
}

Value* Double::subtract(const Value* other) const
{
	switch (other->get_type())
	{
		case ValueType::INTEGERTYPE:
			return new Double(value - other->get_int_value());
		case ValueType::DOUBLETYPE:
			return new Double(value - other->get_double_value());
		default:
			return new Null();
	}
}

Value* Double::multiply(const Value* other) const
{
	switch (other->get_type())
	{
		case ValueType::INTEGERTYPE:
			return new Double(value * other->get_int_value());
		case ValueType::DOUBLETYPE:
			return new Double(value * other->get_double_value());
		default:
			return new Null();
	}
}

Value* Double::divide(const Value* other) const
{
	switch (other->get_type())
	{
		case ValueType::INTEGERTYPE:
			if (other->get_int_value() == 0) return  new Null();
			return new Double(value / other->get_int_value());
		case ValueType::DOUBLETYPE:
			if (other->get_double_value() == 0) return  new Null();
			return new Double(value / other->get_double_value());
		default:
			return new Null();
	}
}

Value* Double::power(const Value* other) const
{
	switch (other->get_type())
	{
		case ValueType::INTEGERTYPE:
			return new Double(pow(value,other->get_int_value()));
		case ValueType::DOUBLETYPE:
			return new Double(pow(value,other->get_double_value()));
		default:
			return new Null();
	}
}