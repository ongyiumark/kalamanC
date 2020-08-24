#include "context.h"

using namespace Contexts;

// This helps if I want to have detailed tracebacks of errors in the future.
Context::Context(std::string name, Context* parent, SymbolTable symbol_table)
    : _name(name), _parent(parent), _symbol_table(symbol_table) {}

std::string Context::get_name() const
{
    return _name;
}

Context* Context::get_parent() const
{
    return _parent;
}

SymbolTable* Context::get_symbol_table()
{
    return &_symbol_table;
}