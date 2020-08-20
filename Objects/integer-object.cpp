#include "object.h"

using namespace Objects;

Integer::Integer(int* value) : _value(value) {}

Type Integer::get_type() const
{
    return Type::INTEGER;
}

int* Integer::get_value() const
{
    return _value;
}