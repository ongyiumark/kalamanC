#include "code-analysis.h"

using namespace CodeAnalysis;

SyntaxToken::SyntaxToken(SyntaxKind kind, int position, std::string text, std::any value)
    : _kind(kind), _position(position), _text(text), _value(value) {}

SyntaxKind SyntaxToken::get_kind()
{
    return _kind;
}

int SyntaxToken::get_position()
{
    return _position;
}

std::string SyntaxToken::get_text()
{
    return _text;
}

std::any SyntaxToken::get_value()
{
    return _value;
}