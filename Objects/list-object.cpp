#include "object.h"

using namespace Objects;

List::List(std::vector<Object*>& values) : _values(values) {}

Type List::get_type() const
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