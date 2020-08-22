#include "context.h"

using namespace Contexts;

Context::Context(std::string name, Context* parent, int parent_pos, SymbolTable* symbol_table)
    : _name(name), _parent(parent), _parent_pos(parent_pos), _symbol_table(symbol_table) {}

std::string Context::get_name() const
{
    return _name;
}

Context* Context::get_parent() const
{
    return _parent;
}

int Context::get_parent_pos() const
{
    return _parent_pos;
}

SymbolTable* Context::get_symbol_table() const
{
    return _symbol_table;
}