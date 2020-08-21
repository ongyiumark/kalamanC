#include "syntax.h"

using namespace Syntax;

int SyntaxNode::children_size() const
{
    return _children.size();
}

SyntaxNode* SyntaxNode::child(int i) const
{
    return _children[i];
}