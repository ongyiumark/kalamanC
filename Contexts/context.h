#pragma once

#include "../Objects/object.h"
#include <unordered_map>
#include <string>

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
        std::unordered_map<std::string, Objects::Object*> _table;
        SymbolTable* _parent;
    public:
        SymbolTable(SymbolTable* parent = nullptr);
        SymbolTable(const SymbolTable&) = delete;
        SymbolTable& operator=(const SymbolTable&) = delete;
        SymbolTable(SymbolTable&& other) noexcept;
        SymbolTable& operator=(SymbolTable&& other) noexcept;
        ~SymbolTable();
        
        ObjectSymbol get_object(const std::string& name);
        void set_object(const std::string& name, Objects::Object* object);
        void clear();

        const std::unordered_map<std::string, Objects::Object*>& get_table() const;
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
        Context(const std::string& name, Context* parent, SymbolTable* parent_symbol_table = nullptr);
        Context(const std::string& name, Context* parent, SymbolTable&& symbol_table);

        const std::string& get_name() const;
        Context* get_parent() const;
        SymbolTable* get_symbol_table();
    };
}
