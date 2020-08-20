#include "binding.h"

using namespace CodeAnalysis;

BoundLiteralExpression::BoundLiteralExpression(std::any value) 
    : _value(value) {}

std::any BoundLiteralExpression::get_value() const
{
    return _value;
}

BoundNodeKind BoundLiteralExpression::get_kind() const
{
    return BoundNodeKind::LiteralExpression;
}

const std::type_info& BoundLiteralExpression::type() const
{
    return _value.type();
}