#include "Expressions/syntax-expressions.h"

using namespace Syntax;

Parser::Parser(std::string& text) : _position(0)
{
    Syntax::Lexer* lexer = new Syntax::Lexer(text);
    SyntaxToken* token;
    do
    {
        token = lexer->lex();
        switch(token->kind())
        {
            case SyntaxKind::BadToken:
            case SyntaxKind::WhitespaceToken:
                break;
            default:
                _tokens.push_back(token);
        }
    } while (token->kind() != Syntax::SyntaxKind::EndOfFileToken);
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

SyntaxToken* Parser::look_ahead() const
{
    return peek(1);
}

SyntaxToken* Parser::next_token()
{
    SyntaxToken* curr = current();
    _position++;
    return curr;
}

SyntaxToken* Parser::match_token(SyntaxKind kind)
{
    if (current()->kind() == kind)
        return next_token();

    _diagnostics->report_unexpected_token(Diagnostics::Position(current()->get_text(), current()->get_postion()), 
        kind_to_string(current()->kind()), kind_to_string(kind));
    return new SyntaxToken(kind, current()->get_postion(), "\0", NULL);
}

SyntaxNode* Parser::parse()
{
    return parse_expressions();
}

SyntaxNode* Parser::parse_expressions(int precedence)
{
    SyntaxNode* left;
    int unary_precedence = SyntaxFacts::get_unary_precedence(current()->kind());
    if (unary_precedence && unary_precedence >= precedence)
    {
        SyntaxToken* op_token = next_token();
        SyntaxNode* expression = parse_expressions(unary_precedence);
        left = new UnaryExpressionSyntax(op_token, expression);
    }
    else left = parse_atom();

    while(true)
    {
        int binary_precedence = SyntaxFacts::get_binary_precedence(current()->kind());
        if (!binary_precedence || binary_precedence <= precedence)
            break;
        
        SyntaxToken* op_token = next_token();
        SyntaxNode* right = parse_expressions(binary_precedence);
        left = new BinaryExpressionSyntax(left, op_token, right);
    }
    return left;
}

SyntaxNode* Parser::parse_atom()
{
    switch(current()->kind())
    {
        case SyntaxKind::IntegerToken:
        case SyntaxKind::StringToken:
        case SyntaxKind::DoubleToken:
        {
            SyntaxToken* token = next_token();
            return new LiteralExpressionSyntax(token);
        }
        case SyntaxKind::TrueKeyword:
        case SyntaxKind::FalseKeyword:
        {
            SyntaxToken* keyword = next_token();
            bool value = keyword->kind() == SyntaxKind::TrueKeyword;
            return new LiteralExpressionSyntax(keyword, new Objects::Boolean(value));
        }
        case SyntaxKind::LParenToken:
        {
            next_token();
            SyntaxNode* expression = parse_expressions();
            match_token(SyntaxKind::RParenToken);
            return expression;
        }
        case SyntaxKind::LSquareToken:
        {
            next_token();
            std::vector<SyntaxNode*> elements;
            if (current()->kind() == SyntaxKind::RSquareToken)
            {
                next_token();
                return new SequenceExpressionSyntax(elements, true);
            }

            SyntaxNode* expression = parse_expressions();
            elements.push_back(expression);

            while(current()->kind() == SyntaxKind::CommaToken)
            {
                next_token();
                SyntaxNode* expression = parse_expressions();
                elements.push_back(expression);
            }
            match_token(SyntaxKind::RSquareToken);
            return new SequenceExpressionSyntax(elements, true);
        }
        default:
        {
            SyntaxToken* identifier = match_token(SyntaxKind::IdentifierToken);
            return new VarAccessExpressionSyntax(identifier);
        }
    }
}