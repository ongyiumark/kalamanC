#include "object.h"
#include "../constants.h"

using namespace Objects;

Boolean::Boolean(bool value) : _value(value) {}

Type Boolean::type() const
{
    return Type::BOOLEAN;
}

bool Boolean::get_value() const
{
    return _value;
}

// I decided to use the keyword used for the boolean values. Refer to constants.h.
std::string Boolean::to_string() const
{
    std::ostringstream os;
    os << (_value ? KT_TRUE : KT_FALSE);
    return os.str();
}

// Boolean operations. These are self explanatory.
Object* Boolean::notted() const
{
    return new Boolean(!_value);
}

Object* Boolean::and_with(Object* other) const
{
    switch(other->type())
    {
        case Type::BOOLEAN:
            return new Boolean(_value && ((Boolean*)other)->get_value());
        default:
            return new None();
    }
}

Object* Boolean::or_with(Object* other) const
{
    switch(other->type())
    {
        case Type::BOOLEAN:
            return new Boolean(_value || ((Boolean*)other)->get_value());
        default:
            return new None();
    }
}

Object* Boolean::xor_with(Object* other) const
{
    switch(other->type())
    {
        case Type::BOOLEAN:
            return new Boolean(_value ^ ((Boolean*)other)->get_value());
        default:
            return new None();
    }
}

Object* Boolean::equals(Object* other) const
{
    switch(other->type())
    {
        case Type::BOOLEAN:
            return new Boolean(_value == ((Boolean*)other)->get_value());
        default:
            return new Boolean(false);
    }
}

Object* Boolean::copy()
{
    return new Boolean(_value);
}