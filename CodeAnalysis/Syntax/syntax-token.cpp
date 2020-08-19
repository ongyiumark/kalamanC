#include "syntax.h"

using namespace CodeAnalysis;

SyntaxToken::SyntaxToken(SyntaxKind kind, int position, std::string text, std::any value)
    : _kind(kind), _position(position), _text(text), _value(value) 
{
    _children = std::vector<SyntaxNode*>();
}

SyntaxToken::~SyntaxToken() {}

SyntaxKind SyntaxToken::get_kind() const
{
    return _kind;
}

int SyntaxToken::get_position() const
{
    return _position;
}

std::string SyntaxToken::get_text() const
{
    return _text;
}

std::any SyntaxToken::get_value() const
{
    return _value;
}
