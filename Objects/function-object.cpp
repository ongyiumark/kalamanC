#include "object.h"

using namespace Objects;

// Uses shared_ptr<SyntaxNode> with a forward declaration so Function safely shares
// ownership of the AST body even after the top-level parse tree is freed.
Function::Function(const std::string& name, const std::vector<std::string>& argument_names,
                   std::shared_ptr<Syntax::SyntaxNode> body, bool built_in,
                   int min_args, int max_args)
    : _name(name),
      _argument_names(argument_names),
      _body(std::move(body)),
      _built_in(built_in),
      _min_args(min_args == -1 ? (int)argument_names.size() : min_args),
      _max_args(max_args == -1 && min_args == -1 ? (int)argument_names.size() : max_args) {}

Type Function::type() const
{
    return Type::FUNCTION;
}

const std::string& Function::get_name() const
{
    return _name;
}

int Function::get_argument_size() const
{
    return _argument_names.size();
}

int Function::get_min_args() const
{
    return _min_args;
}

int Function::get_max_args() const
{
    return _max_args;
}

const std::string& Function::get_argument_name(int i) const
{
    return _argument_names[i];
}

const std::vector<std::string>& Function::get_argument_names() const
{
    return _argument_names;
}

Syntax::SyntaxNode* Function::get_body() const
{
    return _body.get();
}

std::shared_ptr<Syntax::SyntaxNode> Function::get_body_shared() const
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
    return "<function:" + _name + ">";
}

// Checks if the two functions are actually the same function by comparing their body/name.
Object* Function::equals(Object* other) const
{
    switch (other->type())
    {
        case Type::FUNCTION:
        {
            Function* fn = (Function*)other;
            if (_built_in && fn->_built_in)
                return new Boolean(_name == fn->_name);
            return new Boolean(_body != nullptr && _body == fn->_body);
        }
        default:
            return new Boolean(false);
    }
}

Object* Function::copy()
{
    return new Function(_name, _argument_names, _body, _built_in, _min_args, _max_args);
}
