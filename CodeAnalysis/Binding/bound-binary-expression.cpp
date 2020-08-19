#include "binding.h"

using namespace CodeAnalysis;

BoundBinaryExpression::BoundBinaryExpression(BoundExpression* left, BoundBinaryOpKind op_kind, BoundExpression* right)
    : _left(left), _op_kind(op_kind), _right(right) {}

BoundBinaryOpKind BoundBinaryExpression::get_op_kind() const
{
    return _op_kind;
}

BoundExpression* BoundBinaryExpression::get_left() const
{
    return _left;
}

BoundExpression* BoundBinaryExpression::get_right() const
{
    return _right;
}

BoundNodeKind BoundBinaryExpression::get_kind() const
{
    return BoundNodeKind::BinaryExpression;
}

const std::type_info& BoundBinaryExpression::type() const
{
    return _left->type();
}