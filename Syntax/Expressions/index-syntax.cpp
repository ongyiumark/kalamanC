#include "syntax-expressions.h"

using namespace Syntax;
using Diagnostics::Position;
// This indexes a string or a list.
IndexExpressionSyntax::IndexExpressionSyntax(SyntaxNode* to_access, SyntaxNode* indexer, Position pos)
    : SyntaxNode(pos), _to_access(to_access), _indexer(indexer)  {}

IndexExpressionSyntax::~IndexExpressionSyntax()
{
    delete _to_access;
    delete _indexer;
}

SyntaxKind IndexExpressionSyntax::kind() const
{
    return SyntaxKind::IndexExpression;
}

SyntaxNode* IndexExpressionSyntax::get_to_access()
{
    return _to_access;
}

SyntaxNode* IndexExpressionSyntax::get_indexer()
{
    return _indexer;
}
