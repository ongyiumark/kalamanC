#pragma once

#include "../Objects/object.h"
#include <map>

namespace Contexts
{
    class SymbolTable final
    {
    private:
        std::map<std::string, Objects::Object*> _table;
        SymbolTable* _parent;
    public:
        SymbolTable(SymbolTable* parent);

        Objects::Object* get_object(const std::string name) const;
        void set_object(const std::string name, Objects::Object* object);

        SymbolTable* get_parent() const;
    };

    class Context final
    {
    private:
        std::string _name;
        int _parent_pos;
        Context* _parent;
        SymbolTable* _symbol_table;
    public:
        Context(std::string name, Context* parent, int parent_pos, SymbolTable* symbol_table);

        std::string get_name() const;
        Context* get_parent() const;
        int get_parent_pos() const;
        SymbolTable* get_symbol_table() const;
    };
}