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
    else
        return SyntaxKind::IdentifierToken;
}