#ifndef CONTEXT_H
#define CONTEXT_H

#include "symbol_table.h"

#include <string>

class Context
{
private:
	std::string display_name;
	Context *parent;
	Position parent_pos;
	SymbolTable *symbol_table;
public:
	Context(std::string name, SymbolTable* table);
	Context(std::string name, Context* par, Position par_pos, SymbolTable* table);

	std::string get_name();
	Context* get_parent();
	SymbolTable* get_table();
	Position get_parent_pos();
};

#endif