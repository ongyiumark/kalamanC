#ifndef PARSER_H
#define PARSER_H

#include "../nodes/nodes.h"
#include "../tracers/errors.h"
#include "../lexer/token.h"

#include <iostream>
#include <vector>

class ParserResult
{
private:
	Node* node;
	Error* error;
	int advance_count, reverse_count;
public:
	ParserResult();
	ParserResult(Node* n, Error* e);

	Node* register_node(ParserResult result);
	Node* try_register_node(ParserResult result);

	void register_advance();
	void success(Node* node);
	void failure(Error* error);
	void print(std::ostream& os) const;

	Node* get_node() const;
	Error* get_error() const;
	int get_advance_count() const;
	int get_reverse_count() const;
};

std::ostream& operator<<(std::ostream& os, const ParserResult& parres);

class Parser
{
private:
	std::vector<Token*> tokens;
	Token* curr_token;
	int idx;
	bool show_output;
public:
	Parser(const std::vector<Token*>& toks, bool show);
	void update_token();
	void advance();
	void reverse_token(int count);

	ParserResult parse();

	ParserResult body();
	ParserResult statements();
	ParserResult statement();

	ParserResult if_stm();
	ParserResult for_stm();
	ParserResult while_stm();
	ParserResult funcdef_stm();

	ParserResult expr();
	ParserResult comp_expr();
	ParserResult arith_expr();
	ParserResult term();
	ParserResult factor();
	ParserResult power();
	ParserResult call();
	ParserResult atom();

	ParserResult list_expr();
};

#endif