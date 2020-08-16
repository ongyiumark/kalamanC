#include "context.h"

////////////////////////////////////
// CONTEXT CLASS
////////////////////////////////////
Context::Context(std::string name, SymbolTable* table)
	: display_name(name), parent(NULL), parent_pos(Position()), symbol_table(table) {}
Context::Context(std::string name, Context* par, Position par_pos, SymbolTable* table)
	: display_name(name), parent(par), parent_pos(par_pos), symbol_table(table) {}


// Getters
std::string Context::get_name()
{
	return display_name;
}

Context* Context::get_parent()
{
	return parent;
}

SymbolTable* Context::get_table()
{
	return symbol_table;
}

Position Context::get_parent_pos()
{
	return parent_pos;
}
