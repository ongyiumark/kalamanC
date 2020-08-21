#include "syntax.h"

bool Syntax::is_digit(char c)
{
    return '0' <= c && c <= '9';
}

bool Syntax::is_letter(char c)
{
    return ('a' <= c && c <= 'z')||('A' <= c && c <='z');
}

bool Syntax::is_valid_identifier(char c)
{
    return is_digit(c) || is_letter(c) || c == '_';
}

std::string Syntax::kind_to_string(SyntaxKind kind)
{
    const char* s = 0;
#define PROCESS_VAL(p) case(p): s = #p; break;
    switch(kind)
    {
        // Tokens
        PROCESS_VAL(SyntaxKind::BadToken);
        PROCESS_VAL(SyntaxKind::EndOfFileToken);
        PROCESS_VAL(SyntaxKind::WhitespaceToken);
        PROCESS_VAL(SyntaxKind::IntegerToken);
        PROCESS_VAL(SyntaxKind::DoubleToken);
        PROCESS_VAL(SyntaxKind::StringToken);
        PROCESS_VAL(SyntaxKind::PlusToken);
        PROCESS_VAL(SyntaxKind::MinusToken);
        PROCESS_VAL(SyntaxKind::StarToken);
        PROCESS_VAL(SyntaxKind::SlashToken);
        PROCESS_VAL(SyntaxKind::EqualsToken);
        PROCESS_VAL(SyntaxKind::BangToken);
        PROCESS_VAL(SyntaxKind::DAmpersandToken);
        PROCESS_VAL(SyntaxKind::DPipeToken);
        PROCESS_VAL(SyntaxKind::DEqualsToken);
        PROCESS_VAL(SyntaxKind::BangEqualsToken);
        PROCESS_VAL(SyntaxKind::LessThanToken);
        PROCESS_VAL(SyntaxKind::LessEqualsToken);
        PROCESS_VAL(SyntaxKind::GreaterThanToken);
        PROCESS_VAL(SyntaxKind::GreaterEqualsToken);
        PROCESS_VAL(SyntaxKind::LParenToken);
        PROCESS_VAL(SyntaxKind::RParenToken);
        PROCESS_VAL(SyntaxKind::LSquareToken);
        PROCESS_VAL(SyntaxKind::RSquareToken);
        PROCESS_VAL(SyntaxKind::LCurlyToken);
        PROCESS_VAL(SyntaxKind::RCurlyToken);
        PROCESS_VAL(SyntaxKind::CommaToken);
        PROCESS_VAL(SyntaxKind::SemicolonToken);
        PROCESS_VAL(SyntaxKind::IdentifierToken);

        //Keywords
        PROCESS_VAL(SyntaxKind::TrueKeyword);
        PROCESS_VAL(SyntaxKind::FalseKeyword);
        PROCESS_VAL(SyntaxKind::BooleanKeyword);
        PROCESS_VAL(SyntaxKind::IntegerKeyword);
        PROCESS_VAL(SyntaxKind::DoubleKeyword);
        PROCESS_VAL(SyntaxKind::StringKeyword);
        PROCESS_VAL(SyntaxKind::ListKeyword);
        PROCESS_VAL(SyntaxKind::FunctionKeyword);
        PROCESS_VAL(SyntaxKind::DefineKeyword);
        PROCESS_VAL(SyntaxKind::IFKeyword);
        PROCESS_VAL(SyntaxKind::ElifKeyword);
        PROCESS_VAL(SyntaxKind::ElseKeyword);
        PROCESS_VAL(SyntaxKind::WhileKeyword);
        PROCESS_VAL(SyntaxKind::ForKeyword);
        PROCESS_VAL(SyntaxKind::AndKeyword);
        PROCESS_VAL(SyntaxKind::OrKeyword);
        PROCESS_VAL(SyntaxKind::XorKeyword);

        // Expressions
        PROCESS_VAL(SyntaxKind::LiteralExpression);
        PROCESS_VAL(SyntaxKind::UnaryExpressions);
        PROCESS_VAL(SyntaxKind::BinaryExpression);
        PROCESS_VAL(SyntaxKind::ParenExpression);
        PROCESS_VAL(SyntaxKind::VarDeclareExpression);
        PROCESS_VAL(SyntaxKind::VarAssignExpression);
        PROCESS_VAL(SyntaxKind::VarAccessExpression);
        PROCESS_VAL(SyntaxKind::ListExpression);
        PROCESS_VAL(SyntaxKind::SequenceExpression);
        PROCESS_VAL(SyntaxKind::IfExpression);
        PROCESS_VAL(SyntaxKind::WhileExpression);
        PROCESS_VAL(SyntaxKind::ForExpression);
        PROCESS_VAL(SyntaxKind::DefineExpression);
        PROCESS_VAL(SyntaxKind::CallExpression);
        PROCESS_VAL(SyntaxKind::IndexExpression);
    }
#undef PROCESS_VAL
    return s;
}