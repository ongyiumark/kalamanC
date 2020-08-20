#include "object.h"

using namespace Objects;

String::String(std::string* value) : _value(value) {}

Type String::get_type() const
{
    return Type::STRING;
}

int String::get_size() const
{
    return _value->size();
}

std::string* String::get_value() const
{
    return _value;
} 

