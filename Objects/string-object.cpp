#include "object.h"

using namespace Objects;

String::String(std::string value) : _value(value) {}

Type String::type() const
{
    return Type::STRING;
}

int String::get_size() const
{
    return _value.size();
}

std::string String::get_value() const
{
    return _value;
} 

std::string String::to_string() const
{
    std::ostringstream os;
    os << _value;
    return os.str();
}

// Concatinates the string.
Object* String::added_by(Object* other) const
{
    switch (other->type())
    {
        case Type::STRING:
            return new String(_value + ((String*)other)->get_value());
        default:
            return none_result;
    }
}

// Binary multiplication here again. Refer to integer-object.cpp.
Object* String::multiplied_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
        {
            int e = ((Integer*)other)->get_value();
            if (e < 0) return none_result;
            std::string result;
            std::string base = _value;
            while (e > 0)
            {
                if (e&1) result += base;
                base += base;
                e >>=1;
            }
            return new String(result);
        }
        default:
            return none_result;
    }
}

// Returns a substring of length 1 because I didn't implement a character data type.
Object* String::accessed_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
        {
            int i = ((Integer*)other)->get_value();
            int n = _value.size();
            if (i < 0) i += n;
            if (i < 0 || i >= n) return none_result;
            return new String(_value.substr(i, 1));
        }
        default:
            return none_result;
    }
}

// Returns the respective booleans.
Object* String::less_than(Object* other) const
{
    switch (other->type())
    {
        case Type::STRING:
            return new Boolean(_value<((String*)other)->get_value());
        default:
            return none_result;
    }
}

Object* String::greater_than(Object* other) const
{
    switch (other->type())
    {
        case Type::STRING:
            return new Boolean(_value>((String*)other)->get_value());
        default:
            return none_result;
    }
}

Object* String::equals(Object* other) const
{
    switch (other->type())
    {
        case Type::STRING:
            return new Boolean(_value==((String*)other)->get_value());
        default:
            return new Boolean(false);
    }
}
