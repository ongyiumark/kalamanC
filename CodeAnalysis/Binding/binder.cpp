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
    std::any any_val = syntax->get_value();
    if (any_val.type() == typeid(NULL))
        any_val = std::make_any<int>(0);      

    return new BoundLiteralExpression(any_val);
}

BoundExpression* Binder::bind_unary_expression(UnaryExpressionSyntax* syntax)
{
    BoundExpression* operand = bind_expression(syntax->get_operand());
    BoundUnaryOp* op = BoundUnaryOp::bind(syntax->get_op_token()->get_kind(), operand->type());
    if (op == NULL)
    {
        std::ostringstream os;
        os << "Unary operator '" << syntax->get_op_token()->get_text() << "' ";
        os << "is not defined for type " << operand->type().name();
        _diagnostics.push_back(os.str());
        return operand;
    }
    return new BoundUnaryExpression(op, operand);
}

BoundExpression* Binder::bind_binary_expression(BinaryExpressionSyntax* syntax)
{
    BoundExpression* left = bind_expression(syntax->get_left());
    BoundExpression* right = bind_expression(syntax->get_right());
    BoundBinaryOp* op = BoundBinaryOp::bind(syntax->get_op_token()->get_kind(), left->type(), right->type());
    if (op == NULL)
    {
        std::ostringstream os;
        os << "Binary operator '" << syntax->get_op_token()->get_text() << "' ";
        os << "is not defined for types " << left->type().name() << " and " << right->type().name();
        _diagnostics.push_back(os.str());
        return left;
    }
    return new BoundBinaryExpression(left, op, right);
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
