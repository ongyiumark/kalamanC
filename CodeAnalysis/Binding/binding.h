#pragma once

#include "../Syntax/syntax.h"

namespace CodeAnalysis
{
    enum class BoundNodeKind
    {
        LiteralExpression,
        UnaryExpression,
        BinaryExpression
    };

    std::string bound_node_kind_to_string(const BoundNodeKind& kind);

    class BoundNode
    {
    public:
        virtual BoundNodeKind get_kind() const = 0;
    };

    class BoundExpression : public BoundNode
    {
    public:
        virtual const std::type_info& type() const = 0;
    };

    class BoundLiteralExpression final : public BoundExpression
    {
    private:
        std::any _value;
    public:
        BoundLiteralExpression(std::any value);

        std::any get_value() const;
        const std::type_info& type() const;
        BoundNodeKind get_kind() const;
    };

    enum class BoundUnaryOpKind
    {
        Identity,
        Negation,
        Null
    };

    std::string bound_unaryop_kind_to_string(const BoundUnaryOpKind& kind);

    class BoundUnaryExpression final : public BoundExpression
    {
    private:
        BoundUnaryOpKind _op_kind;
        BoundExpression* _operand;
    public:
        BoundUnaryExpression(BoundUnaryOpKind op_kind, BoundExpression* operand);

        BoundUnaryOpKind get_op_kind() const;
        BoundExpression* get_operand() const;
        const std::type_info& type() const;
        BoundNodeKind get_kind() const;
    };

    enum class BoundBinaryOpKind
    {
        Addition,
        Subtraction,
        Multiplication,
        Division,
        Null
    };

    std::string bound_binaryop_kind_to_string(const BoundBinaryOpKind& kind);

    class BoundBinaryExpression final : public BoundExpression
    {
    private:
        BoundExpression* _left;
        BoundBinaryOpKind _op_kind;
        BoundExpression* _right;
    public:
        BoundBinaryExpression(BoundExpression* left, BoundBinaryOpKind op_kind, BoundExpression* right);

        BoundBinaryOpKind get_op_kind() const;
        BoundExpression* get_left() const;
        BoundExpression* get_right() const;
        const std::type_info& type() const;
        BoundNodeKind get_kind() const;
    };

    class Binder final
    {
    private:
        std::vector<std::string> _diagnostics;

        BoundExpression* bind_literal_expression(LiteralExpressionSyntax* syntax);
        BoundExpression* bind_unary_expression(UnaryExpressionSyntax* syntax);
        BoundExpression* bind_binary_expression(BinaryExpressionSyntax* syntax);
        BoundUnaryOpKind bind_unary_operator_kind(SyntaxKind kind, const std::type_info& type);
        BoundBinaryOpKind bind_binary_operator_kind(SyntaxKind kind, const std::type_info& l_type, const std::type_info& r_type);
    public:
        BoundExpression* bind_expression(ExpressionSyntax* syntax);

        int get_diagnostics_size() const;
        std::string get_diagnostic(int i) const;
        std::vector<std::string> get_diagnostics() const;
    };

}