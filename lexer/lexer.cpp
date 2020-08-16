#include "lexer.h"
#include "../constants.h"

////////////////////////////////////
// TOKEN ERROR STRUCT
////////////////////////////////////
TokenError::TokenError(Token* tok, Error* err) : token(tok), error(err) {}

////////////////////////////////////
// LEXER RESULT CLASS
////////////////////////////////////
LexerResult::LexerResult (Error* err) : tokens(std::vector<Token*>()), error(err) {}
LexerResult::LexerResult (std::vector<Token*> toks) : tokens(toks), error(NULL) {}

void LexerResult::print (std::ostream& os) const 
{
	if (error) os << *error;
	else if (tokens.size())
	{
		int n = tokens.size();
		os << "[";
		for (int i = 0; i < n; i++)
		{
			os << "(" << *tokens[i] << ")";
			if (i != n-1) os << ", ";
		}
		os << "]\n";
	}
	else os << "No tokens\n";
}

std::vector<Token*>& LexerResult::get_tokens()
{
	return tokens;
}

Error* LexerResult::get_error()
{
	return error;
}

std::ostream& operator<<(std::ostream& os, const LexerResult& lexres)
{
	lexres.print(os);
	return os;
}

////////////////////////////////////
// LEXER CLASS
////////////////////////////////////
Lexer::Lexer(std::string fname, std::string ftext)
	: pos(Position(fname, new std::string(ftext))), text(ftext)
	{
		advance();
	}

void Lexer::advance()
{
	pos.advance(curr_char);
	int idx = pos.get_idx();
	if (idx < text.size())
		curr_char = text[idx];
	else curr_char = '\0';
}

// Generate the tokens
LexerResult Lexer::make_tokens()
{
	std::vector<Token*> tokens;
	while(curr_char)
	{
		if (IGNORE.find(curr_char) != -1)
		{
			advance();
			continue;
		}

		if (LETTERS.find(curr_char) != -1)
		{
			tokens.push_back(make_identifier());
			continue;
		}

		if (DIGITS.find(curr_char) != -1)
		{
			tokens.push_back(make_number());
			continue;
		}

		switch (curr_char)
		{
			case '"':
				{
					TokenError result = make_string();
					if (result.error) return LexerResult(result.error);
					tokens.push_back(result.token);
					break;
				}
			case '+':
				tokens.push_back(new Token(TokenType::PLUS, pos));
				advance();
				break;
			case '-':
				tokens.push_back(new Token(TokenType::MINUS, pos));
				advance();
				break;
			case '*':
				tokens.push_back(new Token(TokenType::MUL, pos));
				advance();
				break;
			case '/':
				{
					Token* token = make_div_or_comment();
					if (token) tokens.push_back(token);
					break;
				}
			case '%':
				tokens.push_back(new Token(TokenType::MOD, pos));
				advance();
				break;
			case '^':
				tokens.push_back(new Token(TokenType::POW, pos));
				advance();
				break;
			case '(':
				tokens.push_back(new Token(TokenType::LPAREN, pos));
				advance();
				break;
			case ')':
				tokens.push_back(new Token(TokenType::RPAREN, pos));
				advance();
				break;
			case '[':
				tokens.push_back(new Token(TokenType::LSQUARE, pos));
				advance();
				break;
			case ']':
				tokens.push_back(new Token(TokenType::RSQUARE, pos));
				advance();
				break;
			case '{':
				tokens.push_back(new Token(TokenType::LCURLY, pos));
				advance();
				break;
			case '}':
				tokens.push_back(new Token(TokenType::RCURLY, pos));
				advance();
				break;
			case ';':
				tokens.push_back(new Token(TokenType::SEMICOLON, pos));
				advance();
				break;
			case ',':
				tokens.push_back(new Token(TokenType::COMMA, pos));
				advance();
				break;
			case '=':
				tokens.push_back(make_equals());
				break;
			case '!':
				{
					TokenError result = make_not_equals();
					if (result.error) return LexerResult(result.error);
					tokens.push_back(result.token);
					break;
				}
			case '<':
				tokens.push_back(make_less_than());
				break;
			case '>':
				tokens.push_back(make_greater_than());
				break;
			default:
				std::string details = "'";
				details += curr_char;
				details += "'";

				Position start = pos;
				advance();
				return LexerResult(new IllegalCharacter(details, start, pos));
		}
	}

	tokens.push_back(new Token(TokenType::ENDFILE, pos));
	return LexerResult(tokens);
}

// Helper functions to make tokens
Token* Lexer::make_number()
{
	std::string result;
	int dot_count = 0;
	Position start = pos;

	while(curr_char && (DIGITS+'.').find(curr_char) != -1)
	{	
		if (curr_char == '.')
		{
			if (dot_count) break;
			dot_count++;
		}
		result += curr_char;
		advance();
	}

	if (dot_count)
		return new Token(TokenType::DOUBLE, start, pos, result);
	else
		return new Token(TokenType::INTEGER, start, pos, result);
}

Token* Lexer::make_identifier()
{
	std::string result;
	Position start = pos;
	while(curr_char && (LETTERS+DIGITS+'_').find(curr_char) != -1)
	{
		result += curr_char;
		advance();
	}

	if (KEYWORDS.find(result) == KEYWORDS.end()) 
		return new Token(TokenType::IDENTIFIER, start, pos, result);
	
	return new Token(TokenType::KEYWORD, start, pos, result);
}

TokenError Lexer::make_not_equals()
{
	Position start = pos;
	advance();
	if (curr_char != '=')
	{
		return TokenError(NULL, new ExpectedCharacter("Expected '=' (after '!')", start, pos));
	}
	advance();
	return TokenError(new Token(TokenType::NEQUALS, start, pos), NULL);
}

Token* Lexer::make_equals()
{
	Position start = pos;
	advance();
	if (curr_char == '=')
	{
		advance();
		return new Token(TokenType::DEQUALS, start, pos);
	}

	return new Token(TokenType::EQUALS, start);
}

Token* Lexer::make_less_than()
{
	Position start = pos;
	advance();
	if (curr_char == '=')
	{
		advance();
		return new Token(TokenType::LEQUALS, start, pos);
	}

	return new Token(TokenType::LESS, start);
}

Token* Lexer::make_greater_than()
{
	Position start = pos;
	advance();
	if (curr_char == '=')
	{
		advance();
		return new Token(TokenType::GEQUALS, start, pos);
	}

	return new Token(TokenType::GREATER, start);
}

TokenError Lexer::make_string()
{
	Position start = pos;
	std::string result;
	bool escape_char = false;
	advance();
	while(curr_char && curr_char != '"')
	{
		if (escape_char)
		{
			if (ESCAPECHARS.count(curr_char) > 0) result += ESCAPECHARS.at(curr_char);
			else result += curr_char;
			escape_char = false;
		}
		else
		{
			if (curr_char == '\\') escape_char = true;
			else result += curr_char;
		}

		advance();
	}

	if (curr_char)
	{
		advance();
		return TokenError(new Token(TokenType::STRING, start, pos, result),NULL);
	}

	return TokenError(NULL, new ExpectedCharacter("Expected \" to end the string", start, pos));
}

void Lexer::single_line_comment()
{
	while(curr_char && curr_char != '\n')
		advance();

	if (curr_char) advance();
}

void Lexer::multi_line_comment()
{
	while(curr_char && curr_char != '*')
		advance();

	if (curr_char)
	{
		advance();
		if (curr_char && curr_char == '/') advance();
		else multi_line_comment();
	}
}

Token* Lexer::make_div_or_comment()
{
	Position start = pos;
	advance();
	if (curr_char && curr_char != '/' && curr_char != '*')
		return new Token(TokenType::DIV, start);

	if (curr_char && curr_char == '/') single_line_comment();
	else if (curr_char && curr_char == '*')
	{
		advance();
		multi_line_comment();
	}
	return NULL;
}