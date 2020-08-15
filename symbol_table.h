#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "values.h"
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

	Value* get_value(std::string name);
	void set_value(std::string name, Value* val);
	void remove_value(std::string name);

	SymbolTable* get_parent();
};

#endif