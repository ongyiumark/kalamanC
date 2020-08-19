#include "code-analysis.h"

bool CodeAnalysis::is_digit(char c)
{
    return '0' <= c && c <= '9';
}

std::string CodeAnalysis::syntax_kind_to_string(const SyntaxKind& kind)
{
    const char* s = 0;
#define PROCESS_VAL(p) case(p): s = #p; break;
    switch(kind)
    {
        PROCESS_VAL(SyntaxKind::NumberToken);
        PROCESS_VAL(SyntaxKind::WhitespaceToken);
        PROCESS_VAL(SyntaxKind::PlusToken);
        PROCESS_VAL(SyntaxKind::MinusToken);
        PROCESS_VAL(SyntaxKind::StarToken);
        PROCESS_VAL(SyntaxKind::SlashToken);
        PROCESS_VAL(SyntaxKind::LParenToken);
        PROCESS_VAL(SyntaxKind::RParenToken);
        PROCESS_VAL(SyntaxKind::BadToken);
        PROCESS_VAL(SyntaxKind::EndOfFileToken);
    }
#undef PROCESS_VAL
    return s;
}

std::ostream& CodeAnalysis::operator<<(std::ostream& os, const SyntaxKind& kind)
{
    os << syntax_kind_to_string(kind);
    return os;
}