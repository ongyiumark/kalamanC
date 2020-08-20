#include "syntax.h"
#include "../../constants.h"

using namespace CodeAnalysis;

int SyntaxFacts::get_unaryop_precedence(SyntaxKind kind)
{
    switch(kind)
    {
        case SyntaxKind::PlusToken:
        case SyntaxKind::MinusToken:
        case SyntaxKind::BangToken:
            return 5;

        default:
            return 0;
    }
}

int SyntaxFacts::get_binaryop_precedence(SyntaxKind kind)
{
    switch(kind)
    {
        case SyntaxKind::StarToken:
        case SyntaxKind::SlashToken:
            return 4;

        case SyntaxKind::PlusToken:
        case SyntaxKind::MinusToken:
            return 3;

        case SyntaxKind::DAmpersandToken:
            return 2;

        case SyntaxKind::DPipeToken:
            return 1;

        default:
            return 0;
    }
}



SyntaxKind SyntaxFacts::get_keyword_kind(std::string text)
{
    if (text == KT_TRUE)
        return SyntaxKind::TrueKeyword;
    else if (text == KT_FALSE)
        return SyntaxKind::FalseKeyword;
    else
        return SyntaxKind::IdentifierToken;
}