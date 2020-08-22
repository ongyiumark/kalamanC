#include "object.h"
#include <math.h>

using namespace Objects;

Integer::Integer(int value) : _value(value) {}

Type Integer::type() const
{
    return Type::INTEGER;
}

int Integer::get_value() const
{
    return _value;
}

std::string Integer::to_string() const
{
    std::ostringstream os;
    os << _value;
    return os.str();
}

Object* Integer::added_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
            return new Integer(_value+((Integer*)other)->get_value());
        case Type::DOUBLE:
            return new Double(_value+((Double*)other)->get_value());
    }
    return new None();
}

Object* Integer::subtracted_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
            return new Integer(_value-((Integer*)other)->get_value());
        case Type::DOUBLE:
            return new Double(_value-((Double*)other)->get_value());
    }
    return new None();
}

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
            if (_value < 0) return new None();
            int e = _value;
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
    }
    return new None();
}

Object* Integer::divided_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
        {
            Integer* other_int = (Integer*)other;
            if (other_int->get_value() == 0) return new None();
            return new Integer(_value/other_int->get_value());
        }
        case Type::DOUBLE:
        {
            Double* other_double = (Double*)other;
            if (other_double->get_value() == 0) return new None();
            return new Double(_value/other_double->get_value());            
        }
    }
    return new None();
}

Object* Integer::modded_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
        {
            Integer* other_int = (Integer*)other;
            if (other_int->get_value() == 0) return new None();
            return new Integer(_value%other_int->get_value());
        }
    }
    return new None();
}

Object* Integer::powered_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
        {
            int b = _value;
            int e = ((Integer*)other)->get_value();
            if (e < 0)
                return new Double(pow(b,e));
            int ans = 1;
            while (e > 0)
            {
                if (e&1) ans *= b;
                b *= b;
                e >>= 1;
            }
            return new Integer(ans);
        }
        case Type::DOUBLE:
            return new Double(pow(_value, ((Double*)other)->get_value()));            
    }
    return new None();
}

Object* Integer::less_than(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
            return new Boolean(_value<((Integer*)other)->get_value());
        case Type::DOUBLE:
            return new Boolean(_value<((Double*)other)->get_value());
    }
    return new None();
}

Object* Integer::greater_than(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
            return new Boolean(_value>((Integer*)other)->get_value());
        case Type::DOUBLE:
            return new Boolean(_value>((Double*)other)->get_value());
    }
    return new None();
}

Object* Integer::equals(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
            return new Boolean(_value==((Integer*)other)->get_value());
        case Type::DOUBLE:
            return new Boolean(_value==((Double*)other)->get_value());
    }
    return new Boolean(false);
}