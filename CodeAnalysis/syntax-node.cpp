#include "code-analysis.h"

using namespace CodeAnalysis;

int SyntaxNode::get_children_size() const 
{
    return _children.size();
}

SyntaxNode* SyntaxNode::get_child(int i) const
{
    return _children[i];
}