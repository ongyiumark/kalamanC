#pragma once

#include <iostream>
#include <string>
#include <any>
#include <vector>

namespace CodeAnalysis
{
    bool is_digit(char c);

    enum SyntaxKind
    {
        NumberToken,
        WhitespaceToken,
        PlusToken,
        MinusToken,
        StarToken,
        SlashToken,
        LParenToken,
        RParenToken,
        BadToken,
        EndOfFileToken,

        NumberExpression,
        BinaryExpression,
        ParenExpression
    };

    std::string syntax_kind_to_string(const SyntaxKind& kind);
    std::ostream& operator<<(std::ostream& os, const SyntaxKind& kind);

    class SyntaxNode
    {
    protected:
        std::vector<SyntaxNode*> _children;
    public:
        virtual SyntaxKind get_kind() const = 0;
        virtual int get_children_size() const;
        virtual SyntaxNode* get_child(int i) const;
    };

    class SyntaxToken : public SyntaxNode
    {
    private:
        SyntaxKind _kind;
        int _position;
        std::string _text;
        std::any _value;

    public:
        SyntaxToken(SyntaxKind kind, int position, std::string text, std::any value);
        ~SyntaxToken();

        SyntaxKind get_kind() const;
        int get_position() const;
        std::string get_text() const;
        std::any get_value() const;
    };

    class Lexer
    {
    private:
        const std::string _text;
        int _position;
        char current() const;
        void next();

    public:
        Lexer(std::string text);
        SyntaxToken* next_token();
    };

    class ExpressionSyntax : public SyntaxNode
    {

    };

    class NumberExpressionSyntax : public ExpressionSyntax
    {
    private:
        SyntaxToken* _number_token;
    public:
        NumberExpressionSyntax(SyntaxToken* number_token);
        SyntaxKind get_kind() const;

        SyntaxToken* get_number_token() const;
    };

    class BinaryExpressionSyntax : public ExpressionSyntax
    {
    private:
        ExpressionSyntax* _left;
        SyntaxToken* _op_token;
        ExpressionSyntax* _right;
    public:
        BinaryExpressionSyntax(ExpressionSyntax* left, SyntaxToken* op_token, ExpressionSyntax* right);
        SyntaxKind get_kind() const;

        ExpressionSyntax* get_left() const;
        SyntaxToken* get_op_token() const;
        ExpressionSyntax* get_right() const;
    };

    class ParenExpressionSyntax : public ExpressionSyntax
    {
    public:
        SyntaxKind get_kind() const;
    };

    class Parser
    {
    private:
        std::vector<SyntaxToken*> _tokens;
        int _position;

        SyntaxToken* peek(int offset) const;
        SyntaxToken* current() const;
        SyntaxToken* next_token();
        SyntaxToken* match(SyntaxKind kind);

        ExpressionSyntax* parse_expression();
        ExpressionSyntax* parse_term();
        ExpressionSyntax* parse_factor();
        ExpressionSyntax* parse_primary();
    public:
        Parser(std::string text);
        ExpressionSyntax* parse();
    };

    void pretty_print(SyntaxNode *node, std::string indent="", bool is_last = true);
}