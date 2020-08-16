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

	bool matches(TokenType t, std::string val="");
	void print(std::ostream& os) const;

	TokenType get_type();
	Position get_start();
	Position get_end();
	std::string get_value();
	std::string as_string();
};

std::ostream& operator<<(std::ostream& os, const Token &tok);

#endif