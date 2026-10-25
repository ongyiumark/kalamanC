#include "syntax-expressions.h"

using namespace Syntax;
using Diagnostics::Position;

// This indexes or slices a string or a list.
IndexExpressionSyntax::IndexExpressionSyntax(SyntaxNode* to_access, SyntaxNode* indexer, Position pos)
    : SyntaxNode(pos), _to_access(to_access), _indexer(indexer), _slice_end(nullptr), _is_slice(false) {}

IndexExpressionSyntax::IndexExpressionSyntax(SyntaxNode* to_access, SyntaxNode* start, SyntaxNode* end, bool is_slice, Position pos)
    : SyntaxNode(pos), _to_access(to_access), _indexer(start), _slice_end(end), _is_slice(is_slice) {}

IndexExpressionSyntax::~IndexExpressionSyntax()
{
    delete _to_access;
    delete _indexer;
    delete _slice_end;
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

SyntaxNode* IndexExpressionSyntax::get_slice_end()
{
    return _slice_end;
}

bool IndexExpressionSyntax::is_slice() const
{
    return _is_slice;
}
