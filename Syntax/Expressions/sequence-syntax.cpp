#include "syntax-expressions.h"

using namespace Syntax;

// This is for sequences and lists.
// Retruns a python-style list when '_to_return' is set to true.
SequenceExpressionSyntax::SequenceExpressionSyntax(std::vector<SyntaxNode*>& nodes, Diagnostics::Position pos, bool to_return)
    : SyntaxNode(pos), _nodes(nodes), _to_return(to_return) {}

SequenceExpressionSyntax::~SequenceExpressionSyntax()
{
    for (auto &o : _nodes)
        delete o;
}

SyntaxKind SequenceExpressionSyntax::kind() const
{
    return SyntaxKind::SequenceExpression;
}

int SequenceExpressionSyntax::get_nodes_size() const
{
    return _nodes.size();
}

SyntaxNode* SequenceExpressionSyntax::get_node(int i)
{
    return _nodes[i];
}

std::vector<SyntaxNode*> SequenceExpressionSyntax::get_nodes()
{
    return _nodes;
}

bool SequenceExpressionSyntax::get_to_return()
{
    return _to_return;
}