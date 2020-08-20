#include "object.h"

using namespace Objects;

Double::Double(double* value) : _value(value) {}

Type Double::get_type() const
{
    return Type::DOUBLE;
}

double* Double::get_value() const
{
    return _value;
}

