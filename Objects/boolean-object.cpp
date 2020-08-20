#include "object.h"

using namespace Objects;

Boolean::Boolean(bool* value) : _value(value) {}

Type Boolean::get_type() const
{
    return Type::BOOLEAN;
}

bool* Boolean::get_value() const
{
    return _value;
}