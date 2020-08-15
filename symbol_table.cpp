#include "symbol_table.h"
#include "values.h"
#include <string>
#include <map>

////////////////////////////////////
// SYMBOL TABLE CLASS
////////////////////////////////////
SymbolTable::SymbolTable() : parent(NULL) {}
SymbolTable::SymbolTable(SymbolTable* par) : parent(par) {}

Value* SymbolTable::get_value(std::string name)
{
	if (symbols.count(name) > 0) return symbols[name];
	if (parent) return parent->get_value(name);
	return new Integer(0);
}

void SymbolTable::set_value(std::string name, Value* val)
{
	symbols[name] = val;
}

void SymbolTable::remove_value(std::string name)
{
	if (symbols.count(name) > 0)
		symbols.erase(symbols.find(name));
}

SymbolTable* SymbolTable::get_parent()
{
	return parent;
}