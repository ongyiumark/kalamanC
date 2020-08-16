#include "parser.h"
#include "../constants.h"
#include "../tracers/position.h"

ParserResult Parser::list_expr()
{
	ParserResult result = ParserResult();
	std::vector<Node*> elems;
	Position start = curr_token->get_start();
	if (!curr_token->matches(TokenType::LSQUARE))
	{
		result.failure(new IllegalSyntax("Expected '['", curr_token->get_start(), curr_token->get_end()));
		return result;
	}

	advance();
	result.register_advance();

	if (curr_token->matches(TokenType::RSQUARE))
	{
		advance();
		result.register_advance();
		result.success(new ListNode(elems, start, curr_token->get_end()));
		return result;
	}

	Node* expres = result.register_node(expr());
	if (result.get_error()) return result;

	elems.push_back(expres);

	while(curr_token->matches(TokenType::COMMA))
	{
		advance();
		result.register_advance();
		expres = result.register_node(expr());
		if (result.get_error()) return result;
		elems.push_back(expres);
	}

	if (!curr_token->matches(TokenType::RSQUARE))
	{
		result.failure(new IllegalSyntax("Expected ']'", curr_token->get_start(), curr_token->get_end()));
		return result;
	}

	advance();
	result.register_advance();
	result.success(new ListNode(elems, start, curr_token->get_end()));
	return result;
}

ParserResult Parser::body()
{
	ParserResult result = ParserResult();

	// SEMICOLON
	Position start = curr_token->get_start();
	if (curr_token->matches(TokenType::SEMICOLON))
	{
		advance();
		result.register_advance();
		result.success(new NoOpNode(start, curr_token->get_end()));
		return result;
	}

	// LCURLY statements RCURLY
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
		Node* cmpd = result.register_node(statements());
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

	// expr SEMICOLON
	Node* express = result.register_node(expr());
	if (result.get_error()) return result;
	
	if (!curr_token->matches(TokenType::SEMICOLON))
	{
		result.failure(new IllegalSyntax("Expected ';'", curr_token->get_start(), curr_token->get_end()));
		return result;
	}

	advance();
	result.register_advance();
	
	result.success(express);
	return result;
}