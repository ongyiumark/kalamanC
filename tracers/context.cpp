#include "context.h"

////////////////////////////////////
// CONTEXT CLASS
////////////////////////////////////
Context::Context(std::string name, SymbolTable* table)
	: display_name(name), parent(NULL), symbol_table(table) {}
Context::Context(std::string name, Context* par, Position par_pos, SymbolTable* table)
	: display_name(name), parent(par), parent_pos(par_pos), symbol_table(table) {}


// Getters
std::string Context::get_name() const
{
	return display_name;
}

Context* Context::get_parent() const
{
	return parent;
}

SymbolTable* Context::get_table() const
{
	return symbol_table;
}

Position Context::get_parent_pos() const
{
	return parent_pos;
}

Context* Context::copy() const
{
	return new Context(display_name, parent, parent_pos, symbol_table);
}
