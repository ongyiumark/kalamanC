#include "parser.h"
#include "constants.h"
#include "position.h"

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

ParserResult Parser::stm()
{
	ParserResult result = ParserResult();
	// LCURLY stm (SEMICOLON* stm)* SEMICOLON* RCURLY
	if (curr_token->matches(TokenType::LCURLY))
	{
		std::vector<Node*> elems;
		Position start = curr_token->get_start();

		advance();
		result.register_advance();

		if (curr_token->matches(TokenType::RCURLY))
		{
			advance();
			result.register_advance();
			result.success(new NoOpNode(curr_token->get_start(), curr_token->get_end()));
			return result;
		}
		Node* statement = result.register_node(stm());
		if (result.get_error()) return result;
		elems.push_back(statement);

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

		while (!curr_token->matches(TokenType::RCURLY))
		{
			statement = result.register_node(stm());
			if (result.get_error()) return result;
			elems.push_back(statement);

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
		}

		advance();
		result.register_advance();

		result.success(new ListNode(elems, start, curr_token->get_end()));
		return result;
	}

	// expr
	result.register_node(expr());

	return result;
}

ParserResult Parser::expr()
{
	ParserResult result = ParserResult();
	// SEMICOLON+
	if (curr_token->matches(TokenType::SEMICOLON))
	{
		Position start = curr_token->get_start();
		while(curr_token->matches(TokenType::SEMICOLON))
		{
			advance();
			result.register_advance();
		}
		result.success(new NoOpNode(start, curr_token->get_end()));
		return result;
	}

	// val-expr
	result.register_node(val_expr());
	return result;
}

ParserResult Parser::val_expr()
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

	std::string details = "Expected '(', " + TOKENTYPES[TokenType::INTEGER];
	details += ", " + TOKENTYPES[TokenType::DOUBLE];
	result.failure(new IllegalSyntax(details, curr_token->get_start(), curr_token->get_end()));
	return result;
}
