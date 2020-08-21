#pragma once

#include "../Objects/object.h"
#include "../Diagnostics/diagnostic.h"

namespace Syntax
{
    bool is_digit(char c);
    bool is_letter(char c);
    bool is_valid_identifier(char c);

    enum class SyntaxKind
    {
        // Tokens
        BadToken,
        EndOfFileToken,
        WhitespaceToken,
        IntegerToken,
        DoubleToken,
        StringToken,
        PlusToken,
        MinusToken,
        StarToken,
        SlashToken,
        EqualsToken,
        BangToken,
        DAmpersandToken,
        DPipeToken,
        DEqualsToken,
        BangEqualsToken,
        LessThanToken,
        LessEqualsToken,
        GreaterThanToken,
        GreaterEqualsToken,
        LParenToken,
        RParenToken,
        LSquareToken,
        RSquareToken,
        LCurlyToken,
        RCurlyToken,
        CommaToken,
        SemicolonToken,
        IdentifierToken,
        
        // Keywords
        TrueKeyword,
        FalseKeyword,
        BooleanKeyword,
        IntegerKeyword,
        DoubleKeyword,
        StringKeyword,
        ListKeyword,
        FunctionKeyword,
        DefineKeyword,
        IFKeyword,
        ElifKeyword,
        ElseKeyword,
        ForKeyword,
        WhileKeyword,
        AndKeyword,
        OrKeyword,
        XorKeyword,

        // Expressions
        LiteralExpression,
        UnaryExpressions,
        BinaryExpression,
        ParenExpression,
        VarDeclareExpression,
        VarAssignExpression,
        VarAccessExpression,
        ListExpression,
        SequenceExpression,
        IfExpression,
        WhileExpression,
        ForExpression,
        DefineExpression,
        CallExpression,
        IndexExpression
    };

    std::string kind_to_string(SyntaxKind kind);

    class SyntaxNode
    {
    protected:
        std::vector<SyntaxNode*> _children;
    public:
        virtual SyntaxKind kind() const = 0;
        virtual int children_size() const;
        virtual SyntaxNode* child(int i) const;
    };

    class SyntaxToken final : public SyntaxNode
    {
    private:
        SyntaxKind _kind;
        int _position;
        std::string _text;
        Objects::Object* _value;
    public:
        SyntaxToken(SyntaxKind kind, int position, std::string text, Objects::Object* value);

        SyntaxKind kind() const;
        int get_postion() const;
        std::string get_text() const;
        Objects::Object* get_object() const;
    };

    class Lexer final
    {
    private:
        const std::string _text;
        int _position;
        Diagnostics::DiagnosticBag* _diagnostics;

        char peek(int offset) const;
        char current() const;
        char look_ahead() const;
        void next(); 
    public:
        Lexer(const std::string& text);
        SyntaxToken* lex();
    };

    class SyntaxFacts
    {
    public:
        static SyntaxKind get_keyword_kind(std::string text);
    };
}