#include "context.h"

using namespace Contexts;
using namespace Objects;

// I didn't want to use a pair.
ObjectSymbol::ObjectSymbol(Object* _object, SymbolTable* _symbol) : object(_object), symbol(_symbol) {}

// This basically compartmentalizes variables to allow for local variables within functions.
SymbolTable::SymbolTable(SymbolTable* parent) : _parent(parent) {}

SymbolTable::SymbolTable(SymbolTable&& other) noexcept
    : _table(std::move(other._table)), _parent(other._parent)
{
    other._table.clear();
    other._parent = nullptr;
}

SymbolTable& SymbolTable::operator=(SymbolTable&& other) noexcept
{
    if (this != &other)
    {
        clear();
        _table = std::move(other._table);
        _parent = other._parent;
        other._table.clear();
        other._parent = nullptr;
    }
    return *this;
}

// Garbage collection.
SymbolTable::~SymbolTable()
{
    clear();
}

void SymbolTable::clear()
{
    for (auto &it : _table)
        delete it.second;
    _table.clear();
}

// Grabs objects from parent symbol table if it doesn't exist locally.
ObjectSymbol SymbolTable::get_object(const std::string& name)
{
    for (SymbolTable* curr = this; curr != nullptr; curr = curr->_parent)
    {
        auto it = curr->_table.find(name);
        if (it != curr->_table.end())
            return ObjectSymbol(it->second, curr);
    }
    return ObjectSymbol(nullptr, nullptr);
}

void SymbolTable::set_object(const std::string& name, Object* object)
{
    auto it = _table.find(name);
    if (it != _table.end())
    {
        if (it->second != object)
            delete it->second;
        it->second = object;
    }
    else
    {
        _table.emplace(name, object);
    }
}

const std::unordered_map<std::string, Object*>& SymbolTable::get_table() const
{
    return _table;
}

SymbolTable* SymbolTable::get_parent() const
{
    return _parent;
}
