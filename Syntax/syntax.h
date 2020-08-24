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
        CommentToken,
        IntegerToken,
        DoubleToken,
        StringToken,
        PlusToken,
        MinusToken,
        StarToken,
        SlashToken,
        PowerToken,
        ModuloToken,
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
        DefineFunctionKeyword,
        IfKeyword,
        ElifKeyword,
        ElseKeyword,
        ForKeyword,
        WhileKeyword,
        AndKeyword,
        OrKeyword,
        XorKeyword,
        NotKeyword,
        ReturnKeyword,
        BreakKeyword,
        ContinueKeyword,

        // Expressions
        LiteralExpression,
        UnaryExpression,
        BinaryExpression,
        VarDeclareExpression,
        VarAssignExpression,
        VarAccessExpression,
        SequenceExpression,
        IfExpression,
        WhileExpression,
        ForExpression,
        FuncDefineExpression,
        FuncCallExpression,
        IndexExpression,
        ReturnExpression,
        BreakExpression,
        ContinueExpression,
        NoneExpression,

        // Builtin Functions
        PrintFunction,
        InputFunction,
        ToIntFunction,
    };

    std::string kind_to_string(SyntaxKind kind);

    class SyntaxNode
    {
    public:
        virtual ~SyntaxNode(); 
        virtual SyntaxKind kind() const = 0;
    };

    void pretty_print(SyntaxNode* node, std::string indent="", bool is_last=true);

    // Refer to syntax-token.cpp.
    class SyntaxToken final : public SyntaxNode
    {
    private:
        SyntaxKind _kind;
        int _position;
        std::string _text;
    public:
        SyntaxToken(SyntaxKind kind, int position, std::string text);
        SyntaxToken();
        ~SyntaxToken();

        SyntaxKind kind() const;
        int get_position() const;
        std::string get_text() const;
    };

    // Refer to lexer.cpp.
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
        SyntaxToken lex();
    };

    // Refer to syntax-facts.cpp.
    class SyntaxFacts final
    {
    public:
        static SyntaxKind get_keyword_kind(std::string text);
        static int get_binary_precedence(SyntaxKind kind);
        static int get_unary_precedence(SyntaxKind kind);
        static Objects::Type get_keyword_type(SyntaxKind kind);
    };
}