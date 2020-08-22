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

Object* String::added_by(Object* other) const
{
    switch (other->type())
    {
        case Type::STRING:
            return new String(_value + ((String*)other)->get_value());
    }
    return new None();
}

Object* String::multiplied_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
        {
            int e = ((Integer*)other)->get_value();
            if (e < 0) return new None();
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
    }
    return new None();
}

Object* String::accessed_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
        {
            int i = ((Integer*)other)->get_value();
            int n = _value.size();
            if (i < 0) i += n;
            if (i < 0 || i >= n) return new None();
            return new String(_value.substr(i, 1));
        }
    }
    return new None();
}

Object* String::less_than(Object* other) const
{
    switch (other->type())
    {
        case Type::STRING:
            return new Boolean(_value<((String*)other)->get_value());
    }
    return new None();
}

Object* String::greater_than(Object* other) const
{
    switch (other->type())
    {
        case Type::STRING:
            return new Boolean(_value>((String*)other)->get_value());
    }
    return new None();
}

Object* String::equals(Object* other) const
{
    switch (other->type())
    {
        case Type::STRING:
            return new Boolean(_value==((String*)other)->get_value());
    }
    return new None();
}
