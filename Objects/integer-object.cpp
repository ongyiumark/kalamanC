#include "object.h"

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