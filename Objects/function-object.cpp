#include "object.h"

using namespace Objects;

Function::Function(std::string name, std::vector<std::string>& argument_names, BoundExpression* body)
    : _name(name), _argument_names(argument_names), _body(body) {}

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

BoundExpression* Function::get_body() const
{
    return _body;
}

std::string Function::to_string() const
{
    std::ostringstream os;
    os << "<function:" << _name << ">";
    return os.str();
}