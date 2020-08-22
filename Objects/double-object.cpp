#include "object.h"
#include <math.h>

using namespace Objects;

Double::Double(double value) : _value(value) {}

Type Double::type() const
{
    return Type::DOUBLE;
}

double Double::get_value() const
{
    return _value;
}

std::string Double::to_string() const
{
    std::ostringstream os;
    os << _value;
    return os.str();
}

Object* Double::added_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
            return new Double(_value+((Integer*)other)->get_value());
        case Type::DOUBLE:
            return new Double(_value+((Double*)other)->get_value());
    }
    return new None();
}

Object* Double::subtracted_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
            return new Double(_value-((Integer*)other)->get_value());
        case Type::DOUBLE:
            return new Double(_value-((Double*)other)->get_value());
    }
    return new None();
}

Object* Double::multiplied_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
            return new Double(_value*((Integer*)other)->get_value());
        case Type::DOUBLE:
            return new Double(_value*((Double*)other)->get_value());
    }
    return new None();
}

Object* Double::divided_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
        {
            Integer* other_int = (Integer*)other;
            if (other_int->get_value() == 0) return new None();
            return new Double(_value/other_int->get_value());
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

Object* Double::powered_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
            return new Double(pow(_value, ((Integer*)other)->get_value())); 
        case Type::DOUBLE:
            return new Double(pow(_value, ((Double*)other)->get_value()));            
    }
    return new None();
}

Object* Double::less_than(Object* other) const
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

Object* Double::greater_than(Object* other) const
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

Object* Double::equals(Object* other) const
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