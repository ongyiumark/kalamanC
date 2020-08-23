#include "syntax-expressions.h"

using namespace Syntax;

// This indexes a string or a list.
IndexExpressionSyntax::IndexExpressionSyntax(SyntaxNode* to_access, SyntaxNode* indexer)
    : _to_access(to_access), _indexer(indexer)  {}

SyntaxKind IndexExpressionSyntax::kind() const
{
    return SyntaxKind::IndexExpression;
}

SyntaxNode* IndexExpressionSyntax::get_to_access() const
{
    return _to_access;
}

SyntaxNode* IndexExpressionSyntax::get_indexer() const
{
    return _indexer;
}
