#include "parser.h"
#include "../constants.h"
#include "../tracers/position.h"

ParserResult Parser::statements()
{
	ParserResult result = ParserResult();
	Position start = curr_token->get_start();

	// ENDFILE|(SEMICOLON* statement (SEMICOLON* statement)* SEMICOLON*)
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
	while (curr_token->get_type() != TokenType::ENDFILE
		&& curr_token->get_type() != TokenType::RCURLY)
	{
		Node* stm = result.register_node(statement());
		if (result.get_error()) return result;
		nodes.push_back(stm);
		while (curr_token->matches(TokenType::SEMICOLON))
		{
			advance();
			result.register_advance();
		}
	}

	result.success(new SequenceNode(nodes, start, curr_token->get_end()));
	if (show_output) result.success(new ListNode(nodes, start, curr_token->get_end()));
	return result;
}

ParserResult Parser::statement()
{
	ParserResult result = ParserResult();
	Position start = curr_token->get_start();

	// KEYWORD:RETURN expr? SEMICOLON
	if (curr_token->matches(TokenType::KEYWORD, KT_RETURN))
	{
		advance();
		result.register_advance();

		Node * ret = result.register_node(expr());
		if (!ret) reverse_token(result.get_reverse_count());

		if (!curr_token->matches(TokenType::SEMICOLON))
		{
			result.failure(new IllegalSyntax("Expected ';'", curr_token->get_start(), curr_token->get_end()));
			return result;
		}
		advance();
		result.register_advance();

		result.success(new ReturnNode(ret, start, curr_token->get_end()));
		return result;
	}

	// KEYWORD:BREAK
	if (curr_token->matches(TokenType::KEYWORD, KT_BREAK))
	{
		advance();
		result.register_advance();

		if (!curr_token->matches(TokenType::SEMICOLON))
		{
			result.failure(new IllegalSyntax("Expected ';'", curr_token->get_start(), curr_token->get_end()));
			return result;
		}
		advance();
		result.register_advance();

		result.success(new BreakNode(start, curr_token->get_end()));
		return result;
	}

	// KEYWORD:CONTINUE
	if (curr_token->matches(TokenType::KEYWORD, KT_CONTINUE))
	{
		advance();
		result.register_advance();

		if (!curr_token->matches(TokenType::SEMICOLON))
		{
			result.failure(new IllegalSyntax("Expected ';'", curr_token->get_start(), curr_token->get_end()));
			return result;
		}
		advance();
		result.register_advance();

		result.success(new ContinueNode(start, curr_token->get_end()));
		return result;
	}

	// if-stm
	if (curr_token->matches(TokenType::KEYWORD, KT_IF))
	{
		result.register_node(if_stm());
		return result;
	}
	// for-stm
	if (curr_token->matches(TokenType::KEYWORD, KT_FOR))
	{
		result.register_node(for_stm());
		return result;
	}
	// while-stm
	if (curr_token->matches(TokenType::KEYWORD, KT_WHILE))
	{
		result.register_node(while_stm());
		return result;
	}
	// funcdef-stm
	if (curr_token->matches(TokenType::KEYWORD, KT_FUNCDEF))
	{
		result.register_node(funcdef_stm());
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

ParserResult Parser::if_stm()
{
	// KEYWORD:IF LPAREN expr RPAREN body 
	//	(KEYWORD:ELIF LPAREN expr RPAREN body)*
	//	(KEYWORD:ELSE body)?
	ParserResult result = ParserResult();

	Position start = curr_token->get_start();
	std::vector<Node*> cases;
	std::vector<Node*> expressions;
	Node* else_case = NULL;

	if (!curr_token->matches(TokenType::KEYWORD, KT_IF))
	{
		result.failure(new IllegalSyntax("Expected " + KT_IF, curr_token->get_start(), curr_token->get_end()));
		return result;
	}

	advance();
	result.register_advance();

	if (!curr_token->matches(TokenType::LPAREN))
	{
		result.failure(new IllegalSyntax("Expected '('", curr_token->get_start(), curr_token->get_end()));
		return result;
	}	

	advance();
	result.register_advance();

	Node* condition = result.register_node(expr());
	if (result.get_error()) return result;
	cases.push_back(condition);

	if (!curr_token->matches(TokenType::RPAREN))
	{
		result.failure(new IllegalSyntax("Expected ')'", curr_token->get_start(), curr_token->get_end()));
		return result;
	}	

	advance();
	result.register_advance();

	Node* express = result.register_node(body());
	if (result.get_error()) return result;
	expressions.push_back(express);

	while(curr_token->matches(TokenType::KEYWORD, KT_ELIF))
	{
		advance();
		result.register_advance();

		if (!curr_token->matches(TokenType::LPAREN))
		{
			result.failure(new IllegalSyntax("Expected '('", curr_token->get_start(), curr_token->get_end()));
			return result;
		}	

		advance();
		result.register_advance();

		condition = result.register_node(expr());
		if (result.get_error()) return result;
		cases.push_back(condition);

		if (!curr_token->matches(TokenType::RPAREN))
		{
			result.failure(new IllegalSyntax("Expected ')'", curr_token->get_start(), curr_token->get_end()));
			return result;
		}	

		advance();
		result.register_advance();

		express = result.register_node(body());
		if (result.get_error()) return result;
		expressions.push_back(express);
	} 	

	if (curr_token->matches(TokenType::KEYWORD, KT_ELSE))
	{
		advance();
		result.register_advance();

		express = result.register_node(body());
		if (result.get_error()) return result;
		else_case = express;
	}

	result.success(new ConditionNode(cases, expressions, else_case, 
		start, curr_token->get_end()));
	return result;
}

ParserResult Parser::for_stm()
{
	// KEYWORD:FOR LPAREN expr SEMICOLON expr SEMICOLON expr RPAREN body
	ParserResult result = ParserResult();
	Position start = curr_token->get_start();

	if (!curr_token->matches(TokenType::KEYWORD, KT_FOR))
	{
		result.failure(new IllegalSyntax("Expected " + KT_FOR, curr_token->get_start(), curr_token->get_end()));
		return result;
	}

	advance();
	result.register_advance();	

	if (!curr_token->matches(TokenType::LPAREN))
	{
		result.failure(new IllegalSyntax("Expected '('", curr_token->get_start(), curr_token->get_end()));
		return result;
	}

	advance();
	result.register_advance();

	Node* init = result.register_node(expr());
	if (result.get_error()) return result;

	if (!curr_token->matches(TokenType::SEMICOLON))
	{
		result.failure(new IllegalSyntax("Expected ';'", curr_token->get_start(), curr_token->get_end()));
		return result;
	}

	advance();
	result.register_advance();

	Node* condition = result.register_node(expr());
	if (result.get_error()) return result;

	if (!curr_token->matches(TokenType::SEMICOLON))
	{
		result.failure(new IllegalSyntax("Expected ';'", curr_token->get_start(), curr_token->get_end()));
		return result;
	}

	advance();
	result.register_advance();

	Node* update = result.register_node(expr());
	if (result.get_error()) return result;

	if (!curr_token->matches(TokenType::RPAREN))
	{
		result.failure(new IllegalSyntax("Expected ')'", curr_token->get_start(), curr_token->get_end()));
		return result;
	}
	advance();
	result.register_advance();

	Node* loop_body = result.register_node(body());
	if (result.get_error()) return result;

	result.success(new ForLoopNode(init, condition, update, loop_body, start, curr_token->get_end()));
	return result;
}

ParserResult Parser::while_stm()
{
	// KEYWORD:WHILE LPAREN expr RPAREN body
	ParserResult result = ParserResult();

	Position start = curr_token->get_start();
	if (!curr_token->matches(TokenType::KEYWORD, KT_WHILE))
	{
		result.failure(new IllegalSyntax("Expected " + KT_WHILE, curr_token->get_start(), curr_token->get_end()));
		return result;
	}

	advance();
	result.register_advance();	

	if (!curr_token->matches(TokenType::LPAREN))
	{
		result.failure(new IllegalSyntax("Expected '('", curr_token->get_start(), curr_token->get_end()));
		return result;
	}

	advance();
	result.register_advance();

	Node* condition = result.register_node(expr());
	if (result.get_error()) return result;

	if (!curr_token->matches(TokenType::RPAREN))
	{
		result.failure(new IllegalSyntax("Expected ')'", curr_token->get_start(), curr_token->get_end()));
		return result;
	}

	advance();
	result.register_advance();

	Node* loop_body = result.register_node(body());
	if (result.get_error()) return result;

	result.success(new WhileLoopNode(condition, loop_body, start, curr_token->get_end()));
	return result;
}

ParserResult Parser::funcdef_stm()
{
	// KEYWORD:FUNCDEF IDENTIFIER LPAREN (IDENTIFIER (COMMA IDENTIFIER)*)? RPAREN body
	ParserResult result = ParserResult();
	Position start = curr_token->get_start();
	Token* identifier;
	std::vector<Token*> arg_names;
	Node* body_node = NULL;

	if (!curr_token->matches(TokenType::KEYWORD, KT_FUNCDEF))
	{
		result.failure(new IllegalSyntax("Expected " + KT_FUNCDEF, curr_token->get_start(), curr_token->get_end()));
		return result;
	}

	advance();
	result.register_advance();

	if (!curr_token->matches(TokenType::IDENTIFIER))
	{
		result.failure(new IllegalSyntax("Expected " + TOKENTYPES[TokenType::IDENTIFIER], 
			curr_token->get_start(), curr_token->get_end()));
		return result;
	}
	
	identifier = curr_token;

	advance();
	result.register_advance();

	if (!curr_token->matches(TokenType::LPAREN))
	{
		result.failure(new IllegalSyntax("Expected '('", curr_token->get_start(), curr_token->get_end()));
		return result;
	}

	advance();
	result.register_advance();

	if (curr_token->matches(TokenType::RPAREN))
	{
		advance();
		result.register_advance();

		body_node = result.register_node(body());
		if (result.get_error()) return result;

		result.success(new FuncDefNode(identifier, arg_names, body_node, start, curr_token->get_end()));
		return result;
	}

	if (!curr_token->matches(TokenType::IDENTIFIER))
	{
		result.failure(new IllegalSyntax("Expected " + TOKENTYPES[TokenType::IDENTIFIER], 
			curr_token->get_start(), curr_token->get_end()));
		return result;
	}
	
	arg_names.push_back(curr_token);

	advance();
	result.register_advance();

	while(curr_token->matches(TokenType::COMMA))
	{
		advance();
		result.register_advance();
		if (!curr_token->matches(TokenType::IDENTIFIER))
		{
			result.failure(new IllegalSyntax("Expected " + TOKENTYPES[TokenType::IDENTIFIER], 
				curr_token->get_start(), curr_token->get_end()));
			return result;
		}
		
		arg_names.push_back(curr_token);
		advance();
		result.register_advance();
	}

	if (!curr_token->matches(TokenType::RPAREN))
	{
		result.failure(new IllegalSyntax("Expected ')'", curr_token->get_start(), curr_token->get_end()));
		return result;
	}

	advance();
	result.register_advance();

	body_node = result.register_node(body());
	if (result.get_error()) return result;

	result.success(new FuncDefNode(identifier, arg_names, body_node, start, curr_token->get_end()));
	return result;
}