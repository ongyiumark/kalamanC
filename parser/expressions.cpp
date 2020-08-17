#include "parser.h"
#include "../constants.h"
#include "../tracers/position.h"

ParserResult Parser::expr()
{
	ParserResult result = ParserResult();
	// KEYWORD:(INT|DOUBLE|STRING|LIST|FUNC) IDENTIFIER (EQUALS expr)?  
	if (curr_token->matches(TokenType::KEYWORD, KT_INT)
		|| curr_token->matches(TokenType::KEYWORD, KT_DOUBLE)
		|| curr_token->matches(TokenType::KEYWORD, KT_STRING)
		|| curr_token->matches(TokenType::KEYWORD, KT_LIST)
		|| curr_token->matches(TokenType::KEYWORD, KT_FUNC))
	{
		Position start = curr_token->get_start();

		ValueType val_type = ValueType::NULLTYPE;
		if (curr_token->get_value() == KT_INT)
			val_type = ValueType::INTEGERTYPE;
		else if (curr_token->get_value() == KT_DOUBLE)
			val_type = ValueType::DOUBLETYPE;
		else if (curr_token->get_value() == KT_STRING)
			val_type = ValueType::STRINGTYPE;
		else if (curr_token->get_value() == KT_LIST)
			val_type = ValueType::LISTTYPE;
		else if (curr_token->get_value() == KT_FUNC)
			val_type = ValueType::FUNCTIONTYPE;

		advance();
		result.register_advance();

		if (!curr_token->matches(TokenType::IDENTIFIER))
		{
			result.failure(new IllegalSyntax("Expected " + TOKENTYPES[TokenType::IDENTIFIER], 
				curr_token->get_start(), curr_token->get_end()));
			return result;
		}

		Token* id = curr_token;

		if (RESERVED.find(id->get_value()) != RESERVED.end())
		{
			std::string details = "'";
			details += id->get_value() + "'";
			details += " is reserved";
			result.failure(new IllegalSyntax(details,curr_token->get_start(), curr_token->get_end()));
			return result;
		}
		advance();
		result.register_advance();

		Node* decl = new VarDeclareNode(val_type, id, start, curr_token->get_end());

		if (!curr_token->matches(TokenType::EQUALS))
		{
			result.success(decl);
			return result;
		}

		advance();
		result.register_advance();

		Node* express = result.register_node(expr());
		if (result.get_error()) return result;

		Node* assgn = new VarAssignNode(id, express, curr_token->get_start(), curr_token->get_end());

		std::vector<Node*> nodes_seq = {decl, assgn};
		Node* seq = new SequenceNode(nodes_seq, start, curr_token->get_end());
		
		result.success(seq);
		return result;
	}

	// IDENTIFIER EQUALS expr 
	if (curr_token->matches(TokenType::IDENTIFIER))
	{
		Position start = curr_token->get_start();
		Token* id = curr_token;
		advance();
		result.register_advance();

		if (curr_token->matches(TokenType::EQUALS))
		{
			advance();
			result.register_advance();

			Node* express = result.register_node(expr());
			if (result.get_error()) return result;

			Node* assgn = new VarAssignNode(id, express, start, curr_token->get_end());
			result.success(assgn);
			return result;
		}
		else reverse_token(1);
	}

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

	while(curr_token->matches(TokenType::DEQUALS)
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