#include "syntax.h"
#include "../constants.h"

using namespace Syntax;

// Yes, I'm using an if statements here. Switches only work for integers, so I'm out of options.
// I'm just hoping that the C++ compiler optimizes this.
SyntaxKind SyntaxFacts::get_keyword_kind(std::string text)
{
    if (text == KT_TRUE)
        return SyntaxKind::TrueKeyword;
    else if (text == KT_FALSE)
        return SyntaxKind::FalseKeyword;
    else if (text == KT_BOOL)
        return SyntaxKind::BooleanKeyword;
    else if (text == KT_INTEGER)
        return SyntaxKind::IntegerKeyword;
    else if (text == KT_DOUBLE)
        return SyntaxKind::DoubleKeyword;
    else if (text == KT_LIST)
        return SyntaxKind::ListKeyword;
    else if (text == KT_STRING)
        return SyntaxKind::StringKeyword;
    else if (text == KT_FUNCTION)
        return SyntaxKind::FunctionKeyword;
    else if (text == KT_DEFINE)
        return SyntaxKind::DefineFunctionKeyword;
    else if (text == KT_IF)
        return SyntaxKind::IfKeyword;
    else if (text == KT_ELIF)
        return SyntaxKind::ElifKeyword;
    else if (text == KT_ELSE)
        return SyntaxKind::ElseKeyword;
    else if (text == KT_WHILE)
        return SyntaxKind::WhileKeyword;
    else if (text == KT_FOR)
        return SyntaxKind::ForKeyword;
    else if (text == KT_AND)
        return SyntaxKind::AndKeyword;
    else if (text == KT_OR)
        return SyntaxKind::OrKeyword;
    else if (text == KT_XOR)
        return SyntaxKind::XorKeyword;
    else if (text == KT_NOT)
        return SyntaxKind::NotKeyword;
    else if (text == KT_RETURN)
        return SyntaxKind::ReturnKeyword;
    else if (text == KT_BREAK)
        return SyntaxKind::BreakKeyword;
    else if (text == KT_CONTINUE)
        return SyntaxKind::ContinueKeyword;
    else if (text == BI_PRINT)
        return SyntaxKind::PrintFunction;
    else if (text == BI_INPUT)
        return SyntaxKind::InputFunction;
    else if (text == BI_TO_INT)
        return SyntaxKind::ToIntFunction;
    else
        return SyntaxKind::IdentifierToken;
}

Objects::Type SyntaxFacts::get_keyword_type(SyntaxKind kind)
{
    switch (kind)
    {
        case SyntaxKind::IntegerKeyword:
            return Objects::Type::INTEGER;
        case SyntaxKind::DoubleKeyword:
            return Objects::Type::DOUBLE;
        case SyntaxKind::BooleanKeyword:
            return Objects::Type::BOOLEAN;
        case SyntaxKind::ListKeyword:
            return Objects::Type::LIST;
        case SyntaxKind::FunctionKeyword:
            return Objects::Type::FUNCTION;
        case SyntaxKind::StringKeyword:
            return Objects::Type::STRING;
        default:
            return Objects::Type::NONE;
    }
}

// Operator precedences.
int SyntaxFacts::get_unary_precedence(SyntaxKind kind)
{
    switch(kind)
    {
        case SyntaxKind::PlusToken:
        case SyntaxKind::MinusToken:
        case SyntaxKind::BangToken:
        case SyntaxKind::NotKeyword:
            return 6;
        default:
            return 0;
    }
}

int SyntaxFacts::get_binary_precedence(SyntaxKind kind)
{
    switch(kind)
    {
        case SyntaxKind::PowerToken:
            return 7;
        case SyntaxKind::StarToken:
        case SyntaxKind::SlashToken:
            return 5;
        case SyntaxKind::PlusToken:
        case SyntaxKind::MinusToken:
        case SyntaxKind::ModuloToken:
            return 4;
        case SyntaxKind::DEqualsToken:
        case SyntaxKind::BangEqualsToken:
        case SyntaxKind::LessEqualsToken:
        case SyntaxKind::LessThanToken:
        case SyntaxKind::GreaterEqualsToken:
        case SyntaxKind::GreaterThanToken:
            return 3;
        case SyntaxKind::DAmpersandToken:
        case SyntaxKind::AndKeyword:
            return 2;
        case SyntaxKind::DPipeToken:
        case SyntaxKind::OrKeyword:
        case SyntaxKind::XorKeyword:
            return 1;
        default:
            return 0;
    }
}
