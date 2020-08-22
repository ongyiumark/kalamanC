#include "syntax.h"

using namespace Syntax;

// These are just to help with the pretty print.
int SyntaxNode::children_size() const
{
    return _children.size();
}

SyntaxNode* SyntaxNode::child(int i) const
{
    return _children[i];
}