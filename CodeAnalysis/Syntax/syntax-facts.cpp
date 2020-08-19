#include "syntax.h"
using namespace CodeAnalysis;

int SyntaxFacts::get_binaryop_precedence(SyntaxKind kind)
{
    switch(kind)
    {
        case SyntaxKind::StarToken:
        case SyntaxKind::SlashToken:
            return 2;

        case SyntaxKind::PlusToken:
        case SyntaxKind::MinusToken:
            return 1;

        default:
            return 0;
    }
}

int SyntaxFacts::get_unaryop_precedence(SyntaxKind kind)
{
    switch(kind)
    {
        case SyntaxKind::PlusToken:
        case SyntaxKind::MinusToken:
            return 3;

        default:
            return 0;
    }
}