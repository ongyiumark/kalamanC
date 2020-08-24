#include "object.h"

using namespace Objects;

//  I used a void* here because I can't declare SyntaxNode here since 'syntax.h' needs to includes 'object.h'.
//  I could move the declaration of SyntaxNode here, but that would mess up my organization.
//  It should be fine since I'm not going to cast it into anything but SyntaxNode... I think.
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

// Prints the name of the function in a special format.
std::string Function::to_string() const
{
    std::ostringstream os;
    os << "<function:" << _name << ">";
    return os.str();
}

// Checks if the two functions are actually the same function by comparing their address.
Object* Function::equals(Object* other) const
{
    switch (other->type())
    {
        case Type::FUNCTION:
            return new Boolean(this == other);
        default:
            return new Boolean(false);
    }
}

Object* Function::copy()
{
    return new Function(_name, _argument_names, _body, _built_in);
}