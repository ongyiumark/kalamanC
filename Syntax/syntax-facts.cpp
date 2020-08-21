#include "syntax.h"
#include "../constants.h"

using namespace Syntax;

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
    else if (text == KT_FUNCTION)
        return SyntaxKind::FunctionKeyword;
    else if (text == KT_DEFINE)
        return SyntaxKind::DefineKeyword;
    else if (text == KT_IF)
        return SyntaxKind::IFKeyword;
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
    else
        return SyntaxKind::IdentifierToken;
}

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