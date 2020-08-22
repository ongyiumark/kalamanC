#include "object.h"

using namespace Objects;

List::List(std::vector<Object*>& values) : _values(values) {}

Type List::type() const
{
    return Type::LIST;
}

int List::get_size() const
{
    return _values.size();
}

Object* List::get_value(int i) const
{
    return _values[i];
}

std::vector<Object*> List::get_values() const
{
    return _values;
}

std::string List::to_string() const
{
    std::ostringstream os;
    int n = _values.size();
    os << "[";
    for (int i = 0; i < n; i++)
    {
        os << _values[i]->to_string();
        if (i != n-1) os << ", ";
    }
    os << "]";
    return os.str();
}

Object* List::added_by(Object* other) const
{
    switch (other->type())
    {
        case Type::LIST:
        {
            std::vector<Object*> result = _values;
            List* other_list = (List*)other;
            int n = other_list->get_size();
            for (int i = 0; i < n; i++)
                result.push_back(other_list->get_value(i));

            return new List(result);
        }
    }

    std::vector<Object*> result = _values;
    result.push_back(other);
    return new List(result);
}

Object* List::accessed_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
        {
            int i = ((Integer*)other)->get_value();
            int n = _values.size();
            if (i < 0) i += n;
            if (i < 0 || i >= n) return new None();
            return _values[i];
        }
    }
    return new None();
}

Object* List::equals(Object* other) const
{
    switch (other->type())
    {
        case Type::LIST:
        {
            int n = _values.size();
            List* other_list = (List*)other;
            int m = other_list->get_size();
            if (m != n) return new Boolean(false);
            
            bool result = true;
            for (int i = 0; i < n; i++)
            {
                result &= ((Boolean*)(_values[i]->equals(other_list->get_value(i))))->get_value();
            }
            return new Boolean(result);
        }
    }
    return new Boolean(false);
}