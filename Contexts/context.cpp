#include "context.h"

using namespace Contexts;

// Helps with a detailed traceback of errors.
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