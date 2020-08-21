#include "syntax.h"

using namespace Syntax;

SyntaxToken::SyntaxToken(SyntaxKind kind, int position, std::string text, Objects::Object* value)
    : _kind(kind), _position(position), _text(text), _value(value) {}

SyntaxKind SyntaxToken::kind() const
{
    return _kind;
}

int SyntaxToken::get_postion() const
{
    return _position;
}

std::string SyntaxToken::get_text() const
{
    return _text;
}

Objects::Object* SyntaxToken::get_object() const
{
    return _value;
}