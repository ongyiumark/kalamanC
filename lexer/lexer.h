#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include "../tracers/errors.h"

#include <vector>
#include <iostream>
#include <string>

struct TokenError
{
	Token* token;
	Error* error;

	TokenError(Token* tok, Error* err);
};

class LexerResult
{
private:
	std::vector<Token*> tokens;
	Error* error;

public:
	LexerResult (Error* err);
	LexerResult (std::vector<Token*> toks);

	void print (std::ostream& os) const;
	std::vector<Token*> get_tokens() const;
	Error* get_error() const;
};

std::ostream& operator<<(std::ostream& os, const LexerResult &lexres);

class Lexer
{
private:
	Position pos;
	char curr_char;
	std::string text;
	
public:
	Lexer(std::string fname, std::string ftext);

	void advance();

	LexerResult make_tokens();
	Token* make_number();
	Token* make_identifier();
	TokenError make_not_equals();
	Token* make_equals();
	Token* make_less_than();
	Token* make_greater_than();
	TokenError make_string();

	void single_line_comment();
	void multi_line_comment();
	Token* make_div_or_comment();
};

#endif