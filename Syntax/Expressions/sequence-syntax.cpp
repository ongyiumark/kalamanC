#include "syntax-expressions.h"

using namespace Syntax;

SequenceExpressionSyntax::SequenceExpressionSyntax(std::vector<SyntaxNode*>& nodes, bool to_return)
    : _nodes(nodes), _to_return(to_return)
{
    _children = nodes;
}

SyntaxKind SequenceExpressionSyntax::kind() const
{
    return SyntaxKind::SequenceExpression;
}

int SequenceExpressionSyntax::get_nodes_size() const
{
    return _nodes.size();
}

SyntaxNode* SequenceExpressionSyntax::get_node(int i) const
{
    return _nodes[i];
}

std::vector<SyntaxNode*> SequenceExpressionSyntax::get_nodes() const
{
    return _nodes;
}

bool SequenceExpressionSyntax::get_to_return() const
{
    return _to_return;
}