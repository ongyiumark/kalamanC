#include "syntax.h"
using namespace CodeAnalysis;

Parser::Parser(const std::string& text) : _position(0)
{
    std::vector<SyntaxToken*> tokens;
    Lexer lexer = Lexer(text);
    
    SyntaxToken* token;
    do
    {
        token = lexer.lex();
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
    _diagnostics = lexer.get_diagnostics();
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

SyntaxToken* Parser::match_token(SyntaxKind kind)
{
    if (current()->get_kind() == kind)
        return next_token();
    
    std::ostringstream os;
    os << "ERROR: unexpected token <" << syntax_kind_to_string(current()->get_kind()) << ">";
    os << ", expected <" << syntax_kind_to_string(kind) << ">";
    _diagnostics.push_back(os.str());
    return new SyntaxToken(kind, current()->get_position(), "", NULL);
}

SyntaxTree* Parser::parse()
{
    ExpressionSyntax* expression = parse_expression();
    SyntaxToken* endoffile_token = match_token(SyntaxKind::EndOfFileToken);
    return new SyntaxTree(_diagnostics, expression, endoffile_token);
} 

ExpressionSyntax* Parser::parse_expression(int parent_precedence)
{
    ExpressionSyntax* left;
    int unary_precedence = SyntaxFacts::get_unaryop_precedence(current()->get_kind());
    if (unary_precedence && unary_precedence >= parent_precedence)
    {
        SyntaxToken* op_token = next_token();
        ExpressionSyntax* operand = parse_expression(unary_precedence);
        left = new UnaryExpressionSyntax(op_token, operand);
    }
    else left = parse_primary();
    while(true)
    {
        int precedence = SyntaxFacts::get_binaryop_precedence(current()->get_kind());
        if (!precedence || precedence <= parent_precedence)
            break;

        SyntaxToken* op_token = next_token();
        ExpressionSyntax* right = parse_expression(precedence);
        left = new BinaryExpressionSyntax(left, op_token, right);
    }
    return left;
}

ExpressionSyntax* Parser::parse_primary()
{
    switch(current()->get_kind())
    {
        case SyntaxKind::LParenToken:
        {
            SyntaxToken* left = next_token();
            ExpressionSyntax* expression = parse_expression();
            SyntaxToken* right = match_token(SyntaxKind::RParenToken);
            return new ParenExpressionSyntax(left, expression, right);
        }
        case SyntaxKind::TrueKeyword:
        case SyntaxKind::FalseKeyword:
        {
            SyntaxToken* keyword_token = next_token();
            bool value = (keyword_token->get_kind() == SyntaxKind::TrueKeyword);
            return new LiteralExpressionSyntax(keyword_token, value);
        }
        default:
        {
            SyntaxToken* number_token = match_token(SyntaxKind::NumberToken);
            return new LiteralExpressionSyntax(number_token);
        }
    }
}

int Parser::get_diagnostics_size() const
{
    return _diagnostics.size();
}

std::string Parser::get_diagnostic(int i) const
{
    int n = _diagnostics.size();
    if (n == 0) return NULL;
    if (i < 0) return _diagnostics[0];
    if (i >= n) return _diagnostics[n-1];
    return _diagnostics[i];
}

std::vector<std::string> Parser::get_diagnostics() const
{
    return _diagnostics;
}
