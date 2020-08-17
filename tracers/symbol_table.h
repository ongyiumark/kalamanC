#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "../values/values.h"
#include <string>
#include <map>

class SymbolTable
{
private:
	std::map<std::string, Value*> symbols;
	SymbolTable* parent;
	
public:
	SymbolTable();
	SymbolTable(SymbolTable *par);

	Value* get_value(const std::string name) const;
	void set_value(const std::string name, Value* val);
	void remove_value(const std::string name);

	SymbolTable* get_parent() const;
};

#endif