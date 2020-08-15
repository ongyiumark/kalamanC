#include "position.h"
#include <string>

////////////////////////////////////
// POSITION CLASS
////////////////////////////////////
Position::Position() 
	: idx(-1), ln(0), col(-1), ln_start(0) {}
Position::Position(std::string name, std::string *text) 
	: idx(-1), ln(0), col(-1), ln_start(0), filename(name), filetext(text) {}

// Advances the position
void Position::advance(char curr_char)
{
	idx++;
	col++;
	if (curr_char == '\n')
	{
		ln++;
		ln_start = idx;
		col = 0;
	}
}

// Getters
int Position::get_idx() const
{
	return idx;
}

int Position::get_ln() const
{
	return ln;
}

int Position::get_col() const
{
	return col;
}

int Position::get_ln_start() const
{
	return ln_start;
}

std::string Position::get_filename() const
{
	return filename;
}

std::string* Position::get_filetext() const
{
	return filetext;
}