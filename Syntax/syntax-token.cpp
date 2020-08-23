#include "syntax.h"

using namespace Syntax;
using namespace Objects;

// These are the tokens that the lexer tokenizes with.
// Literals such as booleans, integers, doubles, and strings are converted and stored here.
SyntaxToken::SyntaxToken(SyntaxKind kind, int position, std::string text, Object* value)
    : _kind(kind), _position(position), _text(text), _value(value) {}

SyntaxKind SyntaxToken::kind() const
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

Object* SyntaxToken::get_object() const
{
    return _value;
}