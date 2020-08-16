#include "parser.h"
#include "../constants.h"
#include "../tracers/position.h"

////////////////////////////////////
// PARSER RESULT CLASS
////////////////////////////////////
ParserResult::ParserResult()
	: node(NULL), error(NULL), advance_count(0), reverse_count(0) {}

ParserResult::ParserResult(Node* n, Error* e)
	: node(n), error(e), advance_count(0), reverse_count(0) {}

Node* ParserResult::register_node(ParserResult result)
{
	error = result.get_error();
	advance_count += result.get_advance_count();
	node = result.get_node();
	return node;
}

Node* ParserResult::try_register_node(ParserResult result)
{
	if (result.get_error())
	{
		reverse_count = result.get_advance_count();
		return NULL;
	}
	return register_node(result);
}

void ParserResult::register_advance()
{
	advance_count++;
}

void ParserResult::success(Node* n)
{
	error = NULL;
	node = n;
}

void ParserResult::failure(Error* err)
{
	node = NULL;
	error = err;
}

void ParserResult::print(std::ostream& os) const
{
	if (error) os << *error;
	else if (node) os << *node;
	else os << "No node";
}

std::ostream& operator<<(std::ostream& os, const ParserResult& parres)
{
	parres.print(os);
	return os;
}

Node* ParserResult::get_node() const
{
	return node;
}

Error* ParserResult::get_error() const
{
	return error;
}

int ParserResult::get_advance_count() const
{
	return advance_count;
}

int ParserResult::get_reverse_count() const
{
	return reverse_count;
}

////////////////////////////////////
// PARSER CLASS
////////////////////////////////////
Parser::Parser(const std::vector<Token*>& toks)
	: tokens(toks), curr_token(NULL), idx(-1) 
	{
		advance();
	}

void Parser::update_token()
{
	if (0 <= idx && idx < tokens.size())
		curr_token = tokens[idx];
}

void Parser::advance()
{
	idx++;
	update_token();
}

void Parser::reverse_token(int count)
{
	idx -= count;
	update_token();
}

std::string Parser::expected_details(const std::vector<std::string>& chars, const std::vector<std::string>& toks)
{
	std::string details = "Expected ";
	int n = chars.size();
	for (int i = 0; i < n; i++)
	{
		details += "'";
		details += chars[i];
		details += "'";
		if (i != n-1) details += ", ";
	}

	n = toks.size();
	for (int i = 0; i < n; i++)
	{
		details += chars[i];
		if (i != n-1) details += ", ";
	}
	return details;
}

ParserResult Parser::parse()
{
	ParserResult result = stm();
	if (!result.get_error() && !curr_token->matches(TokenType::ENDFILE))
	{
		std::string details = "Expected end of file";
		result.failure(new IllegalSyntax(details, curr_token->get_start(), curr_token->get_end()));
		return result;
	}
	return result;
}