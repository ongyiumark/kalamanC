#include "parser.h"
#include "../constants.h"
#include "../tracers/position.h"

ParserResult Parser::atom()
{
	ParserResult result = ParserResult();
	switch (curr_token->get_type())
	{
		// INT|DOUBLE
		case TokenType::INTEGER:
		case TokenType::DOUBLE:
		{
			result.success(new NumberNode(curr_token, curr_token->get_start(), curr_token->get_end()));
			advance();
			result.register_advance();
			return result;
		}
		// STRING
		case TokenType::STRING:
		{
			result.success(new StringNode(curr_token, curr_token->get_start(), curr_token->get_end()));
			advance();
			result.register_advance();
			return result;
		}
		// IDENTIFIER
		case TokenType::IDENTIFIER:
		{
			result.success(new VarAccessNode(curr_token, curr_token->get_start(), curr_token->get_end()));
			advance();
			result.register_advance();
			return result;
		}
		// LPAREN expr RPAREN
		case TokenType::LPAREN:
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
		case TokenType::LSQUARE:
		{
			result.register_node(list_expr());
			return result;
		}
		default:
		{
			std::string details = "Expected '(', '[', " + TOKENTYPES[TokenType::INTEGER];
			details += ", " + TOKENTYPES[TokenType::DOUBLE] + ", " + TOKENTYPES[TokenType::STRING] + ", ";
			details += TOKENTYPES[TokenType::IDENTIFIER];
			result.failure(new IllegalSyntax(details, curr_token->get_start(), curr_token->get_end()));
			return result;
		}
	}
}

ParserResult Parser::list_expr()
{
	// LSQUARE (expr (COMMA expr)*)? RSQUARE
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
	Position start = curr_token->get_start();

	switch (curr_token->get_type())
	{
		// SEMICOLON
		case TokenType::SEMICOLON:
		{
			advance();
			result.register_advance();
			result.success(new NoOpNode(start, curr_token->get_end()));
			return result;		
		}	
		// LCURLY statements RCURLY
		case TokenType::LCURLY:
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
		default:
		{
			// statement
			Node* express = result.register_node(statement());
			return result;
		}
	}
}