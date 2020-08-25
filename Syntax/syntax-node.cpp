#include "syntax.h"

using namespace Syntax;
using namespace Diagnostics;

SyntaxNode::SyntaxNode() : _pos(Position()) {}
SyntaxNode::SyntaxNode(Position pos) : _pos(pos) {}

Position SyntaxNode::get_pos()
{
    return _pos;
}

SyntaxNode::~SyntaxNode() {}