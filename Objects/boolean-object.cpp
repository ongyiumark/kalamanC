#include "object.h"

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

std::string Boolean::to_string() const
{
    std::ostringstream os;
    os << _value;
    return os.str();
}