#include "context.h"

using namespace Diagnostics;

SymbolTable::SymbolTable(SymbolTable* parent) : _parent(parent) {}

Objects::Object* SymbolTable::get_object(const std::string name) const
{
    if (_table.count(name) > 0) return _table.at(name);
    if (_parent) return _parent->get_object(name);
    return new Objects::None();
}

void SymbolTable::set_object(const std::string name, Objects::Object* object)
{
    _table[name] = object;
}

SymbolTable* SymbolTable::get_parent() const
{
    return _parent;
}