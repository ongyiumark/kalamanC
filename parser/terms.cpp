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
	// call (POW factor)*
	Node* left = result.register_node(call());
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

ParserResult Parser::call()
{
	// atom (LPAREN (expr (COMMA expr)*)? RPAREN)?
	ParserResult result = ParserResult();
	Node* atom_node = result.register_node(atom());

	if (curr_token->matches(TokenType::LPAREN))
	{
		Position start = curr_token->get_start();
		std::vector<Node*> arg_nodes;

		advance();
		result.register_advance();

		if (curr_token->matches(TokenType::RPAREN))
		{
			advance();
			result.register_advance();
			result.success(new FuncCallNode(atom_node, arg_nodes, start, curr_token->get_end()));
			return result;
		}

		Node* expres = result.register_node(expr());
		if (result.get_error()) return result;

		arg_nodes.push_back(expres);

		while(curr_token->matches(TokenType::COMMA))
		{
			advance();
			result.register_advance();
			expres = result.register_node(expr());
			if (result.get_error()) return result;
			arg_nodes.push_back(expres);
		}

		if (!curr_token->matches(TokenType::RPAREN))
		{
			result.failure(new IllegalSyntax("Expected ')'", curr_token->get_start(), curr_token->get_end()));
			return result;
		}

		advance();
		result.register_advance();

		result.success(new FuncCallNode(atom_node, arg_nodes, start, curr_token->get_end()));
		return result;
	}

	result.success(atom_node);
	return result;
}


