#pragma once

#include "../Objects/object.h"
#include <map>

namespace Contexts
{
    class SymbolTable;
    
    // Refer to symbol-table.cpp.
    struct ObjectSymbol
    {
        Objects::Object* object;
        SymbolTable* symbol;
        ObjectSymbol(Objects::Object* _object, SymbolTable* _symbol);
    };

    // Refer to symbol-table.cpp.
    class SymbolTable final
    {
    private:
        std::map<std::string, Objects::Object*> _table;
        SymbolTable* _parent;
    public:
        SymbolTable(SymbolTable* parent);
        ~SymbolTable();
        
        ObjectSymbol get_object(const std::string name);
        void set_object(const std::string name, Objects::Object* object);

        SymbolTable* get_parent() const;
    };

    // Refer to context.cpp.
    class Context final
    {
    private:
        std::string _name;
        Context* _parent;
        SymbolTable _symbol_table;
    public:
        Context(std::string name, Context* parent, SymbolTable symbol_table);

        std::string get_name() const;
        Context* get_parent() const;
        SymbolTable* get_symbol_table();
    };
}