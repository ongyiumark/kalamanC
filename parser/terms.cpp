#include "parser.h"
#include "../constants.h"
#include "../tracers/position.h"

ParserResult Parser::term()
{
	ParserResult result = ParserResult();
	// factor ((MUL|DIV|MOD) factor)*
	Node* left = result.register_node(factor());
	if (result.get_error()) return result;

	while(curr_token->matches(TokenType::MUL) 
		|| curr_token->matches(TokenType::DIV)
		|| curr_token->matches(TokenType::MOD) )
	{
		Token* op = curr_token;
		advance();
		result.register_advance();

		Node* right = result.register_node(factor());
		if (result.get_error()) return result;

		left = new BinOpNode(op, left, right, left->get_start(), right->get_end());
	}

	result.success(left);
	return result;
}

ParserResult Parser::factor()
{
	ParserResult result = ParserResult();

	// (PLUS|MINUS)* factor
	if (curr_token->matches(TokenType::PLUS) || curr_token->matches(TokenType::MINUS))
	{
		Token* op = curr_token;
		advance();
		result.register_advance();

		Node* child = result.register_node(factor());
		if (result.get_error()) return result;

		result.success(new UnaryOpNode(op, child, op->get_start(), child->get_end()));
		return result;
	}

	// power
	result.register_node(power());
	return result;
}

ParserResult Parser::power()
{
	ParserResult result = ParserResult();
	// atom (POW factor)*
	Node* left = result.register_node(atom());
	if (result.get_error()) return result;

	while(curr_token->matches(TokenType::POW))
	{
		Token* op = curr_token;
		advance();
		result.register_advance();

		Node* right = result.register_node(factor());
		if (result.get_error()) return result;

		left = new BinOpNode(op, left, right, left->get_start(), right->get_end());
	}

	result.success(left);
	return result;
}

ParserResult Parser::atom()
{
	ParserResult result = ParserResult();
	// INT|DOUBLE
	if (curr_token->matches(TokenType::INTEGER)
		|| curr_token->matches(TokenType::DOUBLE))
	{
		result.success(new NumberNode(curr_token, curr_token->get_start(), curr_token->get_end()));
		advance();
		result.register_advance();
		return result;
	}
	else if (curr_token->matches(TokenType::STRING))
	{
		result.success(new StringNode(curr_token, curr_token->get_start(), curr_token->get_end()));
		advance();
		result.register_advance();
		return result;
	}
	// LPAREN expr RPAREN
	else if (curr_token->matches(TokenType::LPAREN))
	{
		advance();
		result.register_advance();

		Node* express = result.register_node(expr());
		if (result.get_error()) return result;

		if (!curr_token->matches(TokenType::RPAREN))
		{
			result.failure(new IllegalSyntax("Expected ')'", curr_token->get_start(), curr_token->get_end()));
			return result;
		}

		advance();
		result.register_advance();

		result.success(express);
		return result;
	}
	// list-expr
	else if (curr_token->matches(TokenType::LSQUARE))
	{
		result.register_node(list_expr());
		return result;
	}


	std::string details = "Expected '(', '['" + TOKENTYPES[TokenType::INTEGER];
	details += ", " + TOKENTYPES[TokenType::DOUBLE] + ", " + TOKENTYPES[TokenType::STRING];
	result.failure(new IllegalSyntax(details, curr_token->get_start(), curr_token->get_end()));
	return result;
}