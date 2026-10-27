#include "object.h"
#include <algorithm>

using namespace Objects;

String::String(const std::string& value) : _value(value) {}

Type String::type() const
{
    return Type::STRING;
}

int String::get_size() const
{
    return _value.size();
}

const std::string& String::get_value() const
{
    return _value;
} 

std::string String::to_string() const
{
    return "'" + _value + "'";
}

std::string String::to_display_string() const
{
    return _value;
}

// Concatenates the string.
Object* String::added_by(Object* other) const
{
    switch (other->type())
    {
        case Type::STRING:
            return new String(_value + ((String*)other)->get_value());
        default:
            return new None();
    }
}

// String repetition when multiplied by an integer. Refer to object-helpers.cpp.
Object* String::multiplied_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
            return repeat_string(_value, ((Integer*)other)->get_value());
        default:
            return new None();
    }
}

// Returns a substring of length 1 because I didn't implement a character data type.
Object* String::accessed_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
        {
            long long i = ((Integer*)other)->get_value();
            long long n = (long long)_value.size();
            if (i < 0) i += n;
            if (i < 0 || i >= n) return new None();
            return new String(_value.substr((size_t)i, 1));
        }
        default:
            return new None();
    }
}

Object* String::slice(long long start, long long end, bool has_start, bool has_end) const
{
    long long n = (long long)_value.size();
    long long s = has_start ? start : 0;
    long long e = has_end ? end : n;

    if (s < 0) s += n;
    if (e < 0) e += n;

    s = std::max(0LL, std::min(s, n));
    e = std::max(0LL, std::min(e, n));

    if (s >= e) return new String("");
    return new String(_value.substr((size_t)s, (size_t)(e - s)));
}

// Returns the respective booleans.
Object* String::less_than(Object* other) const
{
    switch (other->type())
    {
        case Type::STRING:
            return new Boolean(_value < ((String*)other)->get_value());
        default:
            return new None();
    }
}

Object* String::greater_than(Object* other) const
{
    switch (other->type())
    {
        case Type::STRING:
            return new Boolean(_value > ((String*)other)->get_value());
        default:
            return new None();
    }
}

Object* String::equals(Object* other) const
{
    switch (other->type())
    {
        case Type::STRING:
            return new Boolean(_value == ((String*)other)->get_value());
        default:
            return new Boolean(false);
    }
}

Object* String::copy()
{
    return new String(_value);
}
