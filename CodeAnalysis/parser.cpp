#include "code-analysis.h"
using namespace CodeAnalysis;

Parser::Parser(std::string text) : _position(0)
{
    std::vector<SyntaxToken*> tokens;
    Lexer lexer = Lexer(text);
    SyntaxToken* token;
    do
    {
        token = lexer.next_token();
        switch(token->get_kind())
        {
            case SyntaxKind::WhitespaceToken:
            case SyntaxKind::BadToken:
                break;
            default:
                tokens.push_back(token);
        }
    } while (token->get_kind() != SyntaxKind::EndOfFileToken);
    _tokens = tokens;
}

SyntaxToken* Parser::peek(int offset) const
{
    int index = _position+offset;
    if (index >= _tokens.size())
        index = _tokens.size()-1;

    return _tokens[index];
}

SyntaxToken* Parser::current() const
{
    return peek(0);
}

SyntaxToken* Parser::next_token()
{
    SyntaxToken* curr = current();
    _position++;
    return curr;
}

SyntaxToken* Parser::match(SyntaxKind kind)
{
    if (current()->get_kind() == kind)
        return next_token();
    
    return new SyntaxToken(kind, current()->get_position(), "", NULL);
}

ExpressionSyntax* Parser::parse()
{
    return parse_expression();
} 

ExpressionSyntax* Parser::parse_expression()
{
    return parse_term();
}

ExpressionSyntax* Parser::parse_term()
{
    ExpressionSyntax* left = parse_factor();
    while(current()->get_kind() == SyntaxKind::PlusToken 
        || current()->get_kind() == SyntaxKind::MinusToken)
    {
        SyntaxToken* op_token = next_token();
        ExpressionSyntax* right = parse_factor();
        left = new BinaryExpressionSyntax(left, op_token, right);
    }
    return left;
}

ExpressionSyntax* Parser::parse_factor()
{
    ExpressionSyntax* left = parse_primary();
    while(current()->get_kind() == SyntaxKind::StarToken 
        || current()->get_kind() == SyntaxKind::SlashToken)
    {
        SyntaxToken* op_token = next_token();
        ExpressionSyntax* right = parse_primary();
        left = new BinaryExpressionSyntax(left, op_token, right);
    }
    return left; 
}

ExpressionSyntax* Parser::parse_primary()
{
    SyntaxToken* number_token = match(SyntaxKind::NumberToken);
    return new NumberExpressionSyntax(number_token);
}