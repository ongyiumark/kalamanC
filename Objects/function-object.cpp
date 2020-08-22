#include "object.h"

using namespace Objects;

Function::Function(std::string name, std::vector<std::string>& argument_names, void* body, bool built_in)
    : _name(name), _argument_names(argument_names), _body(body), _built_in(built_in) {}

Type Function::type() const
{
    return Type::FUNCTION;
}

std::string Function::get_name() const
{
    return _name;
}

int Function::get_argument_size() const
{
    return _argument_names.size();
}

std::string Function::get_argument_name(int i) const
{
    return _argument_names[i];
}

std::vector<std::string> Function::get_argument_names() const
{
    return _argument_names;
}

void* Function::get_body() const
{
    return _body;
}

bool Function::is_built_in() const
{
    return _built_in;
}

std::string Function::to_string() const
{
    std::ostringstream os;
    os << "<function:" << _name << ">";
    return os.str();
}

Object* Function::equals(Object* other) const
{
    switch (other->type())
    {
        case Type::FUNCTION:
            return new Boolean(this == other);
    }
    return new Boolean(false);
}