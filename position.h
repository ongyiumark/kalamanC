#ifndef POSITION_H
#define POSITION_H

#include <string>
class Position
{
private:
	int idx, ln, col, ln_start;
	std::string filename, *filetext;

public:
	Position();
	Position(std::string name, std::string *text);

	void advance(char curr_char='\0');
	int get_idx() const;
	int get_ln() const;
	int get_col() const;
	int get_ln_start() const;
	std::string get_filename() const;
	std::string* get_filetext() const;
};

#endif