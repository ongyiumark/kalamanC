#include "parser.h"
#include "../constants.h"
#include "../tracers/position.h"

ParserResult Parser::stm()
{
	ParserResult result = ParserResult();

	while (curr_token->matches(TokenType::SEMICOLON))
	{
		advance();
		result.register_advance();
	}

	// LCURLY cmpd-expr* RCURLY
	if (curr_token->matches(TokenType::LCURLY))
	{
		advance();
		result.register_advance();

		if (curr_token->matches(TokenType::RCURLY))
		{
			advance();
			result.register_advance();
			result.success(new NoOpNode(curr_token->get_start(), curr_token->get_end()));
			return result;
		}
		Node* cmpd = result.register_node(cmpd_expr());
		if (result.get_error()) return result;

		if (!curr_token->matches(TokenType::RCURLY))
		{
			result.failure(new IllegalSyntax("Expected '}'", curr_token->get_start(), curr_token->get_end()));
			return result;
		}

		advance();
		result.register_advance();

		result.success(cmpd);
		return result;
	}

	// expr SEMICOLON+;
	Node* express = result.register_node(expr());
	if (result.get_error()) return result;
	
	if (!curr_token->matches(TokenType::SEMICOLON))
	{
		result.failure(new IllegalSyntax("Expected ';'", curr_token->get_start(), curr_token->get_end()));
		return result;
	}

	while (curr_token->matches(TokenType::SEMICOLON))
	{
		advance();
		result.register_advance();
	}

	result.success(express);
	return result;
}