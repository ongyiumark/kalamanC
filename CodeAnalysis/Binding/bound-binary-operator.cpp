#include "binding.h"

using namespace CodeAnalysis;

BoundBinaryOp::BoundBinaryOp(SyntaxKind syntax_kind, BoundBinaryOpKind kind, const std::type_info& left_type, 
               const std::type_info& right_type, const std::type_info& result_type)
        : _syntax_kind(syntax_kind), _kind(kind), _left_type(left_type),
            _right_type(right_type), _result_type(result_type) {}

BoundBinaryOp::BoundBinaryOp(SyntaxKind syntax_kind, BoundBinaryOpKind kind, const std::type_info& type, 
        const std::type_info& result_type)
        : _syntax_kind(syntax_kind), _kind(kind), _left_type(type),
            _right_type(type), _result_type(result_type) {}

BoundBinaryOp::BoundBinaryOp(SyntaxKind syntax_kind, BoundBinaryOpKind kind, const std::type_info& type)
        : _syntax_kind(syntax_kind), _kind(kind), _left_type(type),
            _right_type(type), _result_type(type) {}

const std::vector<BoundBinaryOp*> BoundBinaryOp::_operators = 
{
    new BoundBinaryOp(SyntaxKind::PlusToken, BoundBinaryOpKind::Addition, typeid(int)),
    new BoundBinaryOp(SyntaxKind::MinusToken, BoundBinaryOpKind::Subtraction, typeid(int)),
    new BoundBinaryOp(SyntaxKind::StarToken, BoundBinaryOpKind::Multiplication, typeid(int)),
    new BoundBinaryOp(SyntaxKind::SlashToken, BoundBinaryOpKind::Division, typeid(int)),

    new BoundBinaryOp(SyntaxKind::DEquals, BoundBinaryOpKind::Equals, typeid(int), typeid(bool)),
    new BoundBinaryOp(SyntaxKind::NEquals, BoundBinaryOpKind::NotEquals, typeid(int), typeid(bool)),
    

    new BoundBinaryOp(SyntaxKind::DAmpersandToken, BoundBinaryOpKind::LogicalAnd, typeid(bool)),
    new BoundBinaryOp(SyntaxKind::DPipeToken, BoundBinaryOpKind::LogicalOr, typeid(bool)),
    new BoundBinaryOp(SyntaxKind::DEquals, BoundBinaryOpKind::Equals, typeid(bool)),
    new BoundBinaryOp(SyntaxKind::NEquals, BoundBinaryOpKind::NotEquals, typeid(bool))
};

SyntaxKind BoundBinaryOp::get_syntax_kind() const
{
    return _syntax_kind;
}

BoundBinaryOpKind BoundBinaryOp::get_kind() const
{
    return _kind;
}

const std::type_info& BoundBinaryOp::get_left_type() const
{
    return _left_type;
}

const std::type_info& BoundBinaryOp::get_right_type() const
{
    return _right_type;
}

const std::type_info& BoundBinaryOp::type() const
{
    return _result_type;
}

BoundBinaryOp* BoundBinaryOp::bind(SyntaxKind syntax_kind, const std::type_info& left_type, const std::type_info& right_type)
{
    for (BoundBinaryOp* op : _operators)
    {
        if (op->get_syntax_kind() == syntax_kind 
            && op->get_left_type() == left_type
            && op->get_right_type() == right_type)
            return op;
    }
    return NULL;
}