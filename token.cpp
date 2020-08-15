#include "token.h"
#include "position.h"

#include <string>
#include <iostream>

////////////////////////////////////
// TOKEN CLASS
////////////////////////////////////
Token::Token(TokenType t, Position s, Position e, std::string val)
	: type(t), start(s), end(e), value(val) {}

Token::Token(TokenType t, Position s, std::string val)
	: type(t), start(s), end(s), value(val) 
	{
		end.advance();
	}

// Checks if Token matches
bool Token::matches(TokenType t, std::string val)
{
	if (!val.empty()) return (t==type)&&(val == value);
	return t==type;
}

void Token::print(std::ostream& os) const 
{	
	if (!value.empty()) os << TOKENTYPES[type] << ":" << value;
	else os << TOKENTYPES[type];
}

// Getters
TokenType Token::get_type()
{
	return type;
}

Position Token::get_start()
{
	return start;
}

Position Token::get_end()
{
	return end;
}

std::string Token::get_value()
{
	return value;
}

// Print to screen
std::ostream& operator<<(std::ostream& os, const Token &tok)
{
	tok.print(os);
	return os;
}

