#include "binding.h"

using namespace CodeAnalysis;

BoundUnaryExpression::BoundUnaryExpression(BoundUnaryOp* op, BoundExpression* operand)
    : _op(op), _operand(operand) {}

BoundUnaryOp* BoundUnaryExpression::get_op() const
{
    return _op;
}

BoundExpression* BoundUnaryExpression::get_operand() const
{
    return _operand;
}

BoundNodeKind BoundUnaryExpression::get_kind() const
{
    return BoundNodeKind::UnaryExpression;
}

const std::type_info& BoundUnaryExpression::type() const
{
    return _op->type();
}