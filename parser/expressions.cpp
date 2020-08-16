#include "parser.h"
#include "../constants.h"
#include "../tracers/position.h"

ParserResult Parser::expr()
{
	ParserResult result = ParserResult();
	// comp-expr (KEYWORD:(AND|OR|XOR) comp-expr)*
	Node* left = result.register_node(comp_expr());
	if (result.get_error()) return result;

	while(curr_token->matches(TokenType::KEYWORD, KT_AND)
		|| curr_token->matches(TokenType::KEYWORD, KT_OR)
		|| curr_token->matches(TokenType::KEYWORD, KT_XOR))
	{
		Token* op = curr_token;
		advance();
		result.register_advance();

		Node* right = result.register_node(comp_expr());
		if (result.get_error()) return result;

		left = new BinOpNode(op, left, right, left->get_start(), right->get_end());
	}


	result.success(left);
	return result;
}

ParserResult Parser::comp_expr()
{
	ParserResult result = ParserResult();

	// KEYWORD:NOT comp-expr 
	if (curr_token->matches(TokenType::KEYWORD, KT_NOT))
	{
		Token* op = curr_token;
		advance();
		result.register_advance();

		Node* child = result.register_node(comp_expr());
		if (result.get_error()) return result;

		result.success(new UnaryOpNode(op, child, op->get_start(), child->get_end()));
		return result;
	}

	//arith-expr ((EQUALS|NEQUALS|LESS|GREATER|GEQUALS|LEQUALS) arith-expr)*
	Node* left = result.register_node(arith_expr());
	if (result.get_error()) return result;

	while(curr_token->matches(TokenType::EQUALS)
		|| curr_token->matches(TokenType::NEQUALS)
		|| curr_token->matches(TokenType::LESS)
		|| curr_token->matches(TokenType::GREATER)
		|| curr_token->matches(TokenType::GEQUALS)
		|| curr_token->matches(TokenType::LEQUALS))
	{
		Token* op = curr_token;
		advance();
		result.register_advance();

		Node* right = result.register_node(arith_expr());
		if (result.get_error()) return result;

		left = new BinOpNode(op, left, right, left->get_start(), right->get_end());
	}

	result.success(left);
	return result;
}

ParserResult Parser::arith_expr()
{
	ParserResult result = ParserResult();
	// term ((PLUS|MINUS) term)*
	Node* left = result.register_node(term());
	if (result.get_error()) return result;

	while(curr_token->matches(TokenType::PLUS) || curr_token->matches(TokenType::MINUS))
	{
		Token* op = curr_token;
		advance();
		result.register_advance();
		
		Node* right = result.register_node(term());
		if (result.get_error()) return result;

		left = new BinOpNode(op, left, right, left->get_start(), right->get_end());
	}

	result.success(left);
	return result;
}