#ifndef TOKEN_H
#define TOKEN_H

#include "../tracers/position.h"

#include <string>
#include <iostream>

enum TokenType 
{
	INTEGER,
	DOUBLE,
	STRING,
	PLUS,
	MINUS,
	MUL,
	DIV,
	MOD,
	POW,
	LPAREN,
	RPAREN,
	LSQUARE,
	RSQUARE,
	LCURLY,
	RCURLY,
	EQUALS,
	NEQUALS,
	DEQUALS,
	LESS,
	GREATER,
	LEQUALS,
	GEQUALS,
	SEMICOLON,
	COMMA,
	KEYWORD,
	IDENTIFIER,
	ENDFILE
};

// For debugging
const std::string TOKENTYPES[] = 
{
	"INTEGER","DOUBLE","STRING",
	"PLUS","MINUS","MUL","DIV","MOD","POW",
	"LPAREN","RPAREN","LSQUARE","RSQUARE","LCURLY","RCURLY",
	"EQUALS","NEQUALS","DEQUALS",
	"LESS","GREATER","LEQUALS","GEQUALS",
	"SEMICOLON","COMMA","KEYWORD","IDENTIFIER","ENDFILE"
};

class Token
{
private:
	TokenType type;	
	Position start, end;
	std::string value;

public:
	Token(TokenType t, Position s, Position e, std::string val="");
	Token(TokenType t, Position s, std::string val="");

	bool matches(const TokenType t, const std::string val="") const;
	void print(std::ostream& os) const;

	TokenType get_type() const;
	Position get_start() const;
	Position get_end() const;
	std::string get_value() const;
	std::string as_string() const;
};

std::ostream& operator<<(std::ostream& os, const Token &tok);

#endif