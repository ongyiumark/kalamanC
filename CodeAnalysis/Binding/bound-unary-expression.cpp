#include "binding.h"

using namespace CodeAnalysis;

BoundUnaryExpression::BoundUnaryExpression(BoundUnaryOpKind op_kind, BoundExpression* operand)
    : _op_kind(op_kind), _operand(operand) {}

BoundUnaryOpKind BoundUnaryExpression::get_op_kind() const
{
    return _op_kind;
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
    return _operand->type();
}