#include "object.h"
#include <math.h>

using namespace Objects;

Integer::Integer(long long value) : _value(value) {}

Type Integer::type() const
{
    return Type::INTEGER;
}

long long Integer::get_value() const
{
    return _value;
}

std::string Integer::to_string() const
{
    std::ostringstream os;
    os << _value;
    return os.str();
}

// The result becomes a double when the other operand is a double.
Object* Integer::added_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
            return new Integer(_value+((Integer*)other)->get_value());
        case Type::DOUBLE:
            return new Double(_value+((Double*)other)->get_value());
        default:
            return none_result;
    }
}

Object* Integer::subtracted_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
            return new Integer(_value-((Integer*)other)->get_value());
        case Type::DOUBLE:
            return new Double(_value-((Double*)other)->get_value());
        default:
            return none_result;
    }
}

// Multiplying an integer to a string duplicates the string to that integer number to times.
// I used binary multiplication (derived from binary exponentiation).
Object* Integer::multiplied_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
            return new Integer(_value*((Integer*)other)->get_value());
        case Type::DOUBLE:
            return new Double(_value*((Double*)other)->get_value());
        case Type::STRING:
        {
            if (_value < 0) return none_result;
            long long e = _value;
            std::string result;
            std::string base = ((String*)other)->get_value();
            while (e > 0)
            {
                if (e&1) result += base;
                base += base;
                e >>= 1;
            }
            return new String(result);
        }
        default:
            return none_result;
    }
}

// Returns none when division by zero occurs.
Object* Integer::divided_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
        {
            Integer* other_int = (Integer*)other;
            if (other_int->get_value() == 0) return none_result;
            return new Integer(_value/other_int->get_value());
        }
        case Type::DOUBLE:
        {
            Double* other_double = (Double*)other;
            if (other_double->get_value() == 0) return none_result;
            return new Double(_value/other_double->get_value());            
        }
        default:
            return none_result;
    }
}

Object* Integer::modded_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
        {
            Integer* other_int = (Integer*)other;
            if (other_int->get_value() == 0) return none_result;
            return new Integer(_value%other_int->get_value());
        }
        default:
            return none_result;
    }
}

// I used binary exponentiaion here if the exponent is a non-negative integer.
Object* Integer::powered_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
        {
            long long b = _value;
            long long e = ((Integer*)other)->get_value();
            if (e < 0) return new Double(powl(b,e));

            long long ans = 1;
            while (e > 0)
            {
                if (e&1) ans *= b;
                b *= b;
                e >>= 1;
            }
            return new Integer(ans);
        }
        case Type::DOUBLE:
            return new Double(powl(_value, ((Double*)other)->get_value()));   
        default:
            return none_result;         
    }
}

// Returns the respective booleans.
Object* Integer::less_than(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
            return new Boolean(_value<((Integer*)other)->get_value());
        case Type::DOUBLE:
            return new Boolean(_value<((Double*)other)->get_value());
        default:
            return none_result;
    }
}

Object* Integer::greater_than(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
            return new Boolean(_value>((Integer*)other)->get_value());
        case Type::DOUBLE:
            return new Boolean(_value>((Double*)other)->get_value());
        default:
            return none_result;
    }
}

Object* Integer::equals(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
            return new Boolean(_value==((Integer*)other)->get_value());
        case Type::DOUBLE:
            return new Boolean(_value==((Double*)other)->get_value());
        default:
            return new Boolean(false);
    }
}