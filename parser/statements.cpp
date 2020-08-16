#include "parser.h"
#include "../constants.h"
#include "../tracers/position.h"

ParserResult Parser::statements()
{
	ParserResult result = ParserResult();

	// ENDFILE|(SEMICOLON* statement (SEMICOLON* statement)* SEMICOLON*)
	Position start = curr_token->get_start();
	if (curr_token->matches(TokenType::ENDFILE))
	{
		result.success(new NoOpNode(start, curr_token->get_end()));
		return result;
	}

	if (curr_token->matches(TokenType::SEMICOLON))
	{
		while (curr_token->matches(TokenType::SEMICOLON))
		{
			advance();
			result.register_advance();
		}
		if (curr_token->matches(TokenType::ENDFILE))
		{
			result.success(new NoOpNode(start, curr_token->get_end()));
			return result;
		}

		result.register_node(statements());
		return result;
	}

	std::vector<Node*> nodes;
	Node* stm = result.register_node(statement());
	if (result.get_error()) return result;
	nodes.push_back(stm);

	while (curr_token->matches(TokenType::SEMICOLON))
	{
		advance();
		result.register_advance();
	}

	while (1)
	{
		stm = result.try_register_node(statement());
		if (!stm)
		{
			reverse_token(result.get_reverse_count());
			result.success(new ListNode(nodes, start, curr_token->get_end()));
			return result;
		}
		if (result.get_error()) return result;
		nodes.push_back(stm);

		while (curr_token->matches(TokenType::SEMICOLON))
		{
			advance();
			result.register_advance();
		}
	}
}

ParserResult Parser::statement()
{
	ParserResult result = ParserResult();

	// KEYWORD:(INT|DOUBLE|STRING|LIST|FUNC) IDENTIFIER (EQUALS expr)? SEMICOLON 
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
			result.failure(new IllegalSyntax("Expected IDENTIFIER", 
				curr_token->get_start(), curr_token->get_end()));
			return result;
		}

		Token* id = curr_token;

		advance();
		result.register_advance();

		Node* decl = new VarDeclareNode(val_type, id, start, curr_token->get_end());
		if (curr_token->matches(TokenType::SEMICOLON))
		{
			advance();
			result.register_advance();
			result.success(decl);
			return result;
		}

		if (!curr_token->matches(TokenType::EQUALS))
		{
			result.failure(new IllegalSyntax("Expected '='", 
				curr_token->get_start(), curr_token->get_end()));
			return result;
		}

		advance();
		result.register_advance();

		Node* express = result.register_node(expr());
		if (result.get_error()) return result;

		if (!curr_token->matches(TokenType::SEMICOLON))
		{
			result.failure(new IllegalSyntax("Expected ';'", 
				curr_token->get_start(), curr_token->get_end()));
			return result;
		}

		Node* assgn = new VarAssignNode(id, express, curr_token->get_start(), curr_token->get_end());
		advance();
		result.register_advance();

		std::vector<Node*> nodes_seq = {decl, assgn};
		Node* seq = new SequenceNode(nodes_seq, start, curr_token->get_end());
		result.success(seq);
		return result;
	}

	// IDENTIFIER EQUALS expr SEMICOLON
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

			if (!curr_token->matches(TokenType::SEMICOLON))
			{
				result.failure(new IllegalSyntax("Expected ';'", 
					curr_token->get_start(), curr_token->get_end()));
				return result;
			}

			Node* assgn = new VarAssignNode(id, express, start, curr_token->get_end());
			advance();
			result.register_advance();
			result.success(assgn);
			return result;
		}
		else reverse_token(1);
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