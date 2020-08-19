#include "syntax.h"

using namespace CodeAnalysis;

int SyntaxNode::get_children_size() const 
{
    return _children.size();
}

SyntaxNode* SyntaxNode::get_child(int i) const
{
    int n = _children.size();
    if (n == 0) return NULL;
    if (i < 0) return _children[0];
    if (i >= n) return _children[n-1];
    return _children[i];
}