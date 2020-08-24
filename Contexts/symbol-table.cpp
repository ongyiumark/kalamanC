#include "context.h"

using namespace Contexts;
using namespace Objects;

// I didn't want to use a pair
ObjectSymbol::ObjectSymbol(Object* _object, SymbolTable* _symbol) : object(_object), symbol(_symbol) {}

// This basically compartmentalizes variables to allow for local variables within functions.
SymbolTable::SymbolTable(SymbolTable* parent) : _parent(parent) {}

// Garbage collection.
SymbolTable::~SymbolTable()
{
    for (auto &it : _table)
        delete it.second;
}

// Grabs objects from parent symbol table if it doesn't exist.
ObjectSymbol SymbolTable::get_object(const std::string name)
{
    if (_table.count(name) > 0) return ObjectSymbol(_table.at(name), this);
    if (_parent) return _parent->get_object(name);
    return ObjectSymbol(new None(), NULL);
}

void SymbolTable::set_object(const std::string name, Object* object)
{
    _table[name] = object;
}

SymbolTable* SymbolTable::get_parent() const
{
    return _parent;
}