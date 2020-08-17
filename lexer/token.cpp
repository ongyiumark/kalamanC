#include "token.h"

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

// Checks if token matches
bool Token::matches(const TokenType t, const std::string val) const
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
TokenType Token::get_type() const
{
	return type;
}

Position Token::get_start() const
{
	return start;
}

Position Token::get_end() const
{
	return end;
}

std::string Token::get_value() const
{
	return value;
}

// Print to screen
std::ostream& operator<<(std::ostream& os, const Token &tok)
{
	tok.print(os);
	return os;
}

std::string Token::as_string() const
{
	std::string result;
	if (!value.empty()) result += TOKENTYPES[type] + ":" + value;
	else result += TOKENTYPES[type];
	return result;
}