#include "values.h"

#include <math.h>

////////////////////////////////////
// DOUBLE CLASS
////////////////////////////////////
String::String(std::string val) : Value(ValueType::STRINGTYPE), value(val) {}

void String::print(std::ostream& os) const 
{
	os << value;
}

std::string String::get_string_value() const
{
	return value;
}

Value* String::add(const Value* other) const
{
	switch (other->get_type())
	{
		case ValueType::STRINGTYPE:
			return new String(value + other->get_string_value());
		default:
			return new Null();
	}
}

Value* String::multiply(const Value* other) const
{
	switch (other->get_type())
	{
		case ValueType::INTEGERTYPE:
		{
			std::string val = value;
			std::string res;
			long long num = other->get_int_value();
			if (num < 0) return new Null();
			if (num == 0) return new String("");

			while(num > 0)
			{
				if (num&1) res += val;
				val += val;
				num >>= 1;
			}

			return new String(res);
		}
		default:
			return new Null();
	}
}

bool String::is_true() const
{
	return value.size();
}

Value* String::less_than(const Value* other) const
{
	switch (other->get_type())
	{
		case ValueType::STRINGTYPE:
			return new Integer((int)(value < other->get_string_value()));
		default:
			return new Null();
	}
}

Value* String::greater_than(const Value* other) const
{
	switch (other->get_type())
	{
		case ValueType::STRINGTYPE:
			return new Integer((int)(value > other->get_string_value()));
		default:
			return new Null();
	}
}

Value* String::equals(const Value* other) const
{
	switch (other->get_type())
	{
		case ValueType::STRINGTYPE:
			return new Integer((int)(value == other->get_string_value()));
		default:
			return new Null();
	}
}

Value* String::copy() const
{
	return new String(value);
}