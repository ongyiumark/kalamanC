#include "parser.h"
#include "../constants.h"
#include "../tracers/position.h"

ParserResult Parser::statements()
{
	ParserResult result = ParserResult();

	// SEMICOLON* statement (SEMICOLON* statement)* SEMICOLON*
	Position start = curr_token->get_start();
	std::vector<Node*> nodes;
	Node* express = result.register_node(expr());
	if (result.get_error()) return result;
	nodes.push_back(express);

	if (!curr_token->matches(TokenType::SEMICOLON))
	{
		result.failure(new IllegalSyntax("Expected ';'", curr_token->get_start(), curr_token->get_end()));
		return result;
	}

	while(1)
	{

		while(curr_token->matches(TokenType::SEMICOLON))
		{
			advance();
			result.register_advance();
		}
		express = result.try_register_node(expr());
		if (!express) 
		{
			reverse_token(result.get_reverse_count());
			result.success(new ListNode(nodes, start, curr_token->get_end()));
			return result;
		}
		if (result.get_error()) return result;
		nodes.push_back(express);

		if (!curr_token->matches(TokenType::SEMICOLON))
		{
			result.failure(new IllegalSyntax("Expected ';'", curr_token->get_start(), curr_token->get_end()));
			return result;
		}
	}
}

ParserResult Parser::statement()
{
	ParserResult result = ParserResult();

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