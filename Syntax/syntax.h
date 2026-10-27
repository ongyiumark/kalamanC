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
        PlusPlusToken,
        MinusMinusToken,
        EqualsToken,
        PlusEqualsToken,
        MinusEqualsToken,
        StarEqualsToken,
        SlashEqualsToken,
        ModuloEqualsToken,
        PowerEqualsToken,
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
        ColonToken,
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
        InKeyword,
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
        SplitFunction,
        SizeFunction,
        TypeFunction,
        ToBoolFunction,
        ToIntFunction,
        ToDoubleFunction,
        ToStringFunction,
        SetIndexFunction,
        PushFunction,
        PopFunction,
        RangeFunction,
        SortFunction,
        ReverseFunction,
        JoinFunction,
        AbsFunction,
        MinFunction,
        MaxFunction,
        SqrtFunction,
        PowModFunction,
        SetModFunction,
        SetRecursionLimitFunction,
        GetRecursionLimitFunction,
        ReadFileFunction,
        WriteFileFunction,
        ClockFunction,
        ExitFunction,
    };

    std::string kind_to_string(SyntaxKind kind);
    std::string kind_to_display_string(SyntaxKind kind);

    class SyntaxNode
    {
    private:
        Diagnostics::Position _pos;
    public:
        SyntaxNode();
        SyntaxNode(Diagnostics::Position pos);

        virtual Diagnostics::Position get_pos();

        virtual ~SyntaxNode(); 
        virtual SyntaxKind kind() const = 0;
    };

    void pretty_print(SyntaxNode* node, std::string indent="", bool is_last=true);

    // Refer to syntax-token.cpp.
    class SyntaxToken final : public SyntaxNode
    {
    private:
        SyntaxKind _kind;
        std::string _text;
    public:
        SyntaxToken(SyntaxKind kind, Diagnostics::Position position, const std::string& text);
        SyntaxToken();
        ~SyntaxToken() override;

        SyntaxKind kind() const override;
        const std::string& get_text() const;
    };

    // Refer to lexer.cpp.
    class Lexer final
    {
    private:
        const std::string& _text;
        int _position;
        int _ln;
        int _col;
        
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
        static void detect_language(const std::string& text);
        static SyntaxKind get_keyword_kind(const std::string& text);
        static bool is_builtin_function(SyntaxKind kind);
        static bool is_type_keyword(SyntaxKind kind);
        static bool is_assignment_operator(SyntaxKind kind);
        static int get_binary_precedence(SyntaxKind kind);
        static int get_unary_precedence(SyntaxKind kind);
        static Objects::Type get_keyword_type(SyntaxKind kind);
    };
}
