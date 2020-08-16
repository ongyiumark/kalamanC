#include "values.h"

#include <math.h>

////////////////////////////////////
// INTEGER CLASS
////////////////////////////////////
Integer::Integer(long long x) : Value(ValueType::INTEGERTYPE), value(x) {}

void Integer::print(std::ostream& os) const 
{
	os << value;
}

long long Integer::get_int_value() const
{
	return value;
}

Value* Integer::add(const Value* other) const
{
	switch (other->get_type())
	{
		case ValueType::INTEGERTYPE:
			return new Integer(value + other->get_int_value());
		case ValueType::DOUBLETYPE:
			return new Double(value + other->get_double_value());
		default:
			return new Null();
	}
}

Value* Integer::subtract(const Value* other) const
{
	switch (other->get_type())
	{
		case ValueType::INTEGERTYPE:
			return new Integer(value - other->get_int_value());
		case ValueType::DOUBLETYPE:
			return new Double(value - other->get_double_value());
		default:
			return new Null();
	}
}

Value* Integer::multiply(const Value* other) const
{
	switch (other->get_type())
	{
		case ValueType::INTEGERTYPE:
			return new Integer(value * other->get_int_value());
		case ValueType::DOUBLETYPE:
			return new Double(value * other->get_double_value());
		default:
			return new Null();
	}
}

Value* Integer::divide(const Value* other) const
{
	switch (other->get_type())
	{
		case ValueType::INTEGERTYPE:
		{
			if (other->get_int_value() == 0) return  new Null();
			return new Integer(value / other->get_int_value());
		}
		case ValueType::DOUBLETYPE:
		{
			if (other->get_double_value() == 0) return  new Null();
			return new Double(value / other->get_double_value());
		}
		default:
			return new Null();
	}
}

Value* Integer::modulo(const Value* other) const
{
	switch (other->get_type())
	{
		case ValueType::INTEGERTYPE:
		{
			if (other->get_int_value() == 0) return  new Null();
			return new Integer(value % other->get_int_value());
		}
		default:
			return new Null();
	}
}

Value* Integer::power(const Value* other) const
{
	switch (other->get_type())
	{
		case ValueType::INTEGERTYPE:
		{
			long long ans = 1;
			long long b = value;
			long long e = other->get_int_value();
			while(e > 0)
			{
				if (e&1) ans*=b;
				b*=b;
				e >>= 1;
			}
			return new Integer(ans);
		}
		case ValueType::DOUBLETYPE:
			return new Double(pow(value,other->get_double_value()));
		default:
			return new Null();
	}
}