#include "syntax.h"

using namespace Syntax;
using namespace Objects;
using namespace Diagnostics;

// These are the tokens that the lexer tokenizes with.
SyntaxToken::SyntaxToken(SyntaxKind kind, Position position, const std::string& text)
    : SyntaxNode(position), _kind(kind), _text(text) {}

SyntaxToken::SyntaxToken() : _kind(SyntaxKind::BadToken), _text("") {}

SyntaxToken::~SyntaxToken() {}

SyntaxKind SyntaxToken::kind() const
{
    return _kind;
}

const std::string& SyntaxToken::get_text() const
{
    return _text;
}
