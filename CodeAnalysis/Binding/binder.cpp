#include "binding.h"

using namespace CodeAnalysis;

BoundExpression* Binder::bind_expression(ExpressionSyntax* syntax)
{
    switch(syntax->get_kind())
    {
        case SyntaxKind::LiteralExpression:
            return bind_literal_expression((LiteralExpressionSyntax*)syntax);
        case SyntaxKind::UnaryExpression:
            return bind_unary_expression((UnaryExpressionSyntax*)syntax);
        case SyntaxKind::BinaryExpression:
            return bind_binary_expression((BinaryExpressionSyntax*)syntax);
        default:
        {
            std::ostringstream os;
            os << "Unexpected syntax " << syntax_kind_to_string(syntax->get_kind());
            throw os.str();
        }
    }
}

BoundExpression* Binder::bind_literal_expression(LiteralExpressionSyntax* syntax)
{
    std::any any_val = syntax->get_literal_token()->get_value();
    int * val_ptr = std::any_cast<int>(&any_val);
    int value = (val_ptr ? *val_ptr : 0); 
    return new BoundLiteralExpression(std::make_any<int>(value));
}

BoundExpression* Binder::bind_unary_expression(UnaryExpressionSyntax* syntax)
{
    BoundExpression* operand = bind_expression(syntax->get_operand());
    BoundUnaryOpKind op_kind = bind_unary_operator_kind(syntax->get_op_token()->get_kind(), operand->type());
    if (op_kind == BoundUnaryOpKind::Null)
    {
        std::ostringstream os;
        os << "Unary operator '" << syntax->get_op_token()->get_text() << "' ";
        os << "is not defined for type " << operand->type().name();
        _diagnostics.push_back(os.str());
        return operand;
    }
    return new BoundUnaryExpression(op_kind, operand);
}

BoundExpression* Binder::bind_binary_expression(BinaryExpressionSyntax* syntax)
{
    BoundExpression* left = bind_expression(syntax->get_left());
    BoundExpression* right = bind_expression(syntax->get_right());
    BoundBinaryOpKind op_kind = bind_binary_operator_kind(syntax->get_op_token()->get_kind(), left->type(), right->type());
    if (op_kind == BoundBinaryOpKind::Null)
    {
        std::ostringstream os;
        os << "Binary operator '" << syntax->get_op_token()->get_text() << "' ";
        os << "is not defined for types " << left->type().name() << " and " << right->type().name();
        _diagnostics.push_back(os.str());
        return left;
    }
    return new BoundBinaryExpression(left, op_kind, right);
}

BoundUnaryOpKind Binder::bind_unary_operator_kind(SyntaxKind kind, const std::type_info& type)
{
    if (type != typeid(int))
        return BoundUnaryOpKind::Null;

    switch(kind)
    {
        case SyntaxKind::PlusToken:
            return BoundUnaryOpKind::Identity;
        case SyntaxKind::MinusToken:
            return BoundUnaryOpKind::Negation;
        default:
        {
            std::ostringstream os;
            os << "Unexpected unary operator " << syntax_kind_to_string(kind);
            throw os.str();
        }
    }
}

BoundBinaryOpKind Binder::bind_binary_operator_kind(SyntaxKind kind, const std::type_info& l_type, const std::type_info& r_type)
{
    if (l_type != typeid(int) || r_type != typeid(int))
        return BoundBinaryOpKind::Null;

    switch(kind)
    {
        case SyntaxKind::PlusToken:
            return BoundBinaryOpKind::Addition;
        case SyntaxKind::MinusToken:
            return BoundBinaryOpKind::Subtraction;
        case SyntaxKind::StarToken:
            return BoundBinaryOpKind::Multiplication;
        case SyntaxKind::SlashToken:
            return BoundBinaryOpKind::Division;
        default:
        {
            std::ostringstream os;
            os << "Unexpected binary operator " << syntax_kind_to_string(kind);
            throw os.str();
        }
    }
}

int Binder::get_diagnostics_size() const
{
    return _diagnostics.size();
}

std::string Binder::get_diagnostic(int i) const
{
    int n = _diagnostics.size();
    if (n == 0) return NULL;
    if (i < 0) return _diagnostics[0];
    if (i >= n) return _diagnostics[n-1];
    return _diagnostics[i];
}

std::vector<std::string> Binder::get_diagnostics() const
{
    return _diagnostics;
}
