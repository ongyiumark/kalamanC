#pragma once

#include <iostream>
#include <string>
#include <any>
#include <vector>
#include <sstream>

namespace CodeAnalysis
{
    bool is_digit(char c);
    bool is_letter(char c);

    enum class SyntaxKind
    {
        // Tokens
        BadToken,
        EndOfFileToken,
        WhitespaceToken,
        NumberToken,
        PlusToken,
        MinusToken,
        StarToken,
        SlashToken,
        LParenToken,
        RParenToken,
        IdentifierToken,
        BangToken,
        DAmpersandToken,
        DPipeToken,

        // Expressions
        LiteralExpression,
        BinaryExpression,
        UnaryExpression,
        ParenExpression,

        // Keywords
        TrueKeyword,
        FalseKeyword
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

    class SyntaxToken final : public SyntaxNode
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

    class Lexer final
    {
    private:
        const std::string _text;
        int _position;
        std::vector<std::string> _diagnostics;

        char peek(int offset) const;
        char current() const;
        char look_ahead() const;
        void next();

    public:
        Lexer(const std::string& text);
        SyntaxToken* lex();

        int get_diagnostics_size() const;
        std::string get_diagnostic(int i) const;
        std::vector<std::string> get_diagnostics() const;
    };

    class ExpressionSyntax : public SyntaxNode
    {

    };

    class LiteralExpressionSyntax final : public ExpressionSyntax 
    {
    private:
        SyntaxToken* _literal_token;
        std::any _value;
    public:
        LiteralExpressionSyntax(SyntaxToken* literal_token);
        LiteralExpressionSyntax(SyntaxToken* literal_token, std::any value);
        SyntaxKind get_kind() const;

        SyntaxToken* get_literal_token() const;
        std::any get_value() const;
    };

    class BinaryExpressionSyntax final : public ExpressionSyntax
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

    class UnaryExpressionSyntax final : public ExpressionSyntax
    {
    private:
        SyntaxToken* _op_token;
        ExpressionSyntax* _operand;
    public:
        UnaryExpressionSyntax( SyntaxToken* op_token, ExpressionSyntax* operand);
        SyntaxKind get_kind() const;

        SyntaxToken* get_op_token() const;
        ExpressionSyntax* get_operand() const;
    };

    class ParenExpressionSyntax final : public ExpressionSyntax
    {
        SyntaxToken* _lparen_token;
        ExpressionSyntax* _expression;
        SyntaxToken* _rparen_token;
    public:
        ParenExpressionSyntax(SyntaxToken* lparen_token, ExpressionSyntax* expression, SyntaxToken* rparen_token);
        SyntaxKind get_kind() const;

        ExpressionSyntax* get_expression() const;
    };

    class SyntaxTree final
    {
    private:
        ExpressionSyntax* _root;
        SyntaxToken* _endoffile_token;
        const std::vector<std::string> _diagnostics;
    public:
        SyntaxTree(const std::vector<std::string>& diagnostics, ExpressionSyntax* root, SyntaxToken* endoffile_token);
        ExpressionSyntax* get_root();
        SyntaxToken* get_endoffile_token();

        int get_diagnostics_size() const;
        std::string get_diagnostic(int i) const;
        std::vector<std::string> get_diagnostics() const;

        static SyntaxTree* parse(const std::string& text);
    };

    class SyntaxFacts
    {
    public:
        static int get_binaryop_precedence(SyntaxKind kind);
        static int get_unaryop_precedence(SyntaxKind kind);
        static SyntaxKind get_keyword_kind(std::string text);
    };

    class Parser final
    {
    private:
        std::vector<SyntaxToken*> _tokens;
        int _position;
        std::vector<std::string> _diagnostics;

        SyntaxToken* peek(int offset) const;
        SyntaxToken* current() const;
        SyntaxToken* next_token();
        SyntaxToken* match_token(SyntaxKind kind);

        ExpressionSyntax* parse_expression(int parent_precedence = 0);
        
        ExpressionSyntax* parse_primary();
    public:
        Parser(const std::string& text);
        SyntaxTree* parse();

        int get_diagnostics_size() const;
        std::string get_diagnostic(int i) const;
        std::vector<std::string> get_diagnostics() const;
    };

    void pretty_print(SyntaxNode *node, std::string indent="", bool is_last = true);
}