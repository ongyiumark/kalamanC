#include "binding.h"

using namespace CodeAnalysis;

BoundUnaryOp::BoundUnaryOp(SyntaxKind syntax_kind, BoundUnaryOpKind kind, 
    const std::type_info& operand_type, const std::type_info& result_type)
        : _syntax_kind(syntax_kind), _kind(kind), _operand_type(operand_type), _result_type(result_type) {}

BoundUnaryOp::BoundUnaryOp(SyntaxKind syntax_kind, BoundUnaryOpKind kind, 
    const std::type_info& operand_type)
        : _syntax_kind(syntax_kind), _kind(kind), _operand_type(operand_type), _result_type(operand_type) {}

const std::vector<BoundUnaryOp*> BoundUnaryOp::_operators = 
{
    new BoundUnaryOp(SyntaxKind::BangToken, BoundUnaryOpKind::LogicalNegation, typeid(bool)),
    new BoundUnaryOp(SyntaxKind::PlusToken, BoundUnaryOpKind::Identity, typeid(int)),
    new BoundUnaryOp(SyntaxKind::MinusToken, BoundUnaryOpKind::Negation, typeid(int))
};

SyntaxKind BoundUnaryOp::get_syntax_kind() const
{
    return _syntax_kind;
}

BoundUnaryOpKind BoundUnaryOp::get_kind() const
{
    return _kind;
}

const std::type_info& BoundUnaryOp::get_operand_type() const
{
    return _operand_type;
}

const std::type_info& BoundUnaryOp::get_result_type() const
{
    return _result_type;
}

BoundUnaryOp* BoundUnaryOp::bind(SyntaxKind syntax_kind, const std::type_info& operand_type)
{
    for (BoundUnaryOp* op : _operators)
    {
        if (op->get_syntax_kind() == syntax_kind && op->get_operand_type() == operand_type)
            return op;
    }
    return NULL;
}