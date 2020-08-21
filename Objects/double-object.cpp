#include "object.h"

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