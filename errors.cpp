#include "errors.h"
#include "position.h"
#include "constants.h"

#include <string>
#include <iostream>

////////////////////////////////////
// ERROR BASE CLASS
////////////////////////////////////
Error::Error(std::string n, std::string dets, Position s, Position e)
	: name(n), details(dets), start(s), end(e) {}

// Generates a string that points at the error
std::string Error::arrowed_string() const
{
	std::string line, arrows;
	std::string script = *start.get_filetext();
	int i = start.get_ln_start();
	char c = script[i];
	while((i-end.get_ln_start() < end.get_col() || c != '\n') && i < script.size())
	{
		c = script[i];
		if (c == '\n' || c == '\t') 
		{	
			i++;
			continue;
		}

		line += c;
		if (i-start.get_ln_start() >= start.get_col() 
			&& i-end.get_ln_start() <= end.get_col())
			arrows += "^";
		else arrows += " ";

		i++;
	}

	return line+"\n"+arrows;
}

void Error::print(std::ostream& os) const 
{
	os << name << " : " << details << "\n\n";
	os << arrowed_string();
}

std::ostream& operator<<(std::ostream& os, const Error &err)
{
	err.print(os);
	return os;
}

////////////////////////////////////
// ILLEGAL CHARACTER CLASS
////////////////////////////////////
IllegalCharacter::IllegalCharacter(std::string dets, Position s, Position e)
	: Error("Illegal Character", dets, s, e) {}

////////////////////////////////////
// EXPECTED CHARACTER CLASS
////////////////////////////////////
ExpectedCharacter::ExpectedCharacter(std::string dets, Position s, Position e)
	: Error("Expected Character", dets, s, e) {}