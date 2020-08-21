#include "syntax-expressions.h"

using namespace Syntax;

IndexExpressionSyntax::IndexExpressionSyntax(SyntaxNode* to_access, SyntaxNode* indexer)
    : _to_access(to_access), _indexer(indexer) 
{
    _children = {to_access, indexer};
}

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
