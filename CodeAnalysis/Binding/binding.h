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
        LogicalNegation,
        Null
    };

    class BoundUnaryOp final 
    {
    private:
        SyntaxKind _syntax_kind;
        BoundUnaryOpKind _kind;
        const std::type_info& _operand_type;
        const std::type_info& _result_type;

        BoundUnaryOp(SyntaxKind syntax_kind, BoundUnaryOpKind kind, const std::type_info& operand_type, 
                const std::type_info& result_type);
        BoundUnaryOp(SyntaxKind syntax_kind, BoundUnaryOpKind kind, const std::type_info& operand_type);
        const static std::vector<BoundUnaryOp*> _operators;
    
    public:
        static BoundUnaryOp* bind(SyntaxKind syntax_kind, const std::type_info& operand_type);
        SyntaxKind get_syntax_kind() const;
        BoundUnaryOpKind get_kind() const;
        const std::type_info& get_operand_type() const;
        const std::type_info& get_result_type() const;
    };

    std::string bound_unaryop_kind_to_string(const BoundUnaryOpKind& kind);

    class BoundUnaryExpression final : public BoundExpression
    {
    private:
        BoundUnaryOp* _op;
        BoundExpression* _operand;
    public:
        BoundUnaryExpression(BoundUnaryOp* op, BoundExpression* operand);

        BoundUnaryOp* get_op() const;
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
        LogicalAnd,
        LogicalOr,
        Null
    };

    class BoundBinaryOp final 
    {
    private:
        SyntaxKind _syntax_kind;
        BoundBinaryOpKind _kind;
        const std::type_info& _left_type;
        const std::type_info& _right_type;
        const std::type_info& _result_type;

        BoundBinaryOp(SyntaxKind syntax_kind, BoundBinaryOpKind kind, const std::type_info& type);

        BoundBinaryOp(SyntaxKind syntax_kind, BoundBinaryOpKind kind, const std::type_info& left_type, 
               const std::type_info& right_type, const std::type_info& result_type);

        const static std::vector<BoundBinaryOp*> _operators;
    
    public:
        static BoundBinaryOp* bind(SyntaxKind syntax_kind, const std::type_info& left_type, const std::type_info& right_type );
        SyntaxKind get_syntax_kind() const;
        BoundBinaryOpKind get_kind() const;
        const std::type_info& get_left_type() const;
        const std::type_info& get_right_type() const;
        const std::type_info& get_result_type() const;
    };

    std::string bound_binaryop_kind_to_string(const BoundBinaryOpKind& kind);

    class BoundBinaryExpression final : public BoundExpression
    {
    private:
        BoundExpression* _left;
        BoundBinaryOp* _op;
        BoundExpression* _right;
    public:
        BoundBinaryExpression(BoundExpression* left, BoundBinaryOp* op, BoundExpression* right);

        BoundBinaryOp* get_op() const;
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