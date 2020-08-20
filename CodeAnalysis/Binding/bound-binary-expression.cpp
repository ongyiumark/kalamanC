#include "binding.h"

using namespace CodeAnalysis;

BoundBinaryExpression::BoundBinaryExpression(BoundExpression* left, BoundBinaryOp* op, BoundExpression* right)
    : _left(left), _op(op), _right(right) {}

BoundBinaryOp* BoundBinaryExpression::get_op() const
{
    return _op;
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
    return _op->type();
}