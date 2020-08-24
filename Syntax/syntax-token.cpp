#include "syntax.h"

using namespace Syntax;
using namespace Objects;

// These are the tokens that the lexer tokenizes with.
SyntaxToken::SyntaxToken(SyntaxKind kind, int position, std::string text)
    : _kind(kind), _position(position), _text(text) {}

SyntaxToken::SyntaxToken() : _kind(SyntaxKind::BadToken), _position(-1), _text("") {}

SyntaxToken::~SyntaxToken() {}

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