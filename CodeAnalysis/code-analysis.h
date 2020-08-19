#pragma once

#include <ostream>
#include <string>
#include <any>

namespace CodeAnalysis
{
    bool is_digit(char c);

    enum class SyntaxKind
    {
        NumberToken,
        WhitespaceToken,
        PlusToken,
        MinusToken,
        StarToken,
        SlashToken,
        LParenToken,
        RParenToken,
        BadToken,
        EndOfFileToken
    };

    std::string syntax_kind_to_string(const SyntaxKind& kind);
    std::ostream& operator<<(std::ostream& os, const SyntaxKind& kind);

    class SyntaxToken
    {
    private:
        SyntaxKind _kind;
        int _position;
        std::string _text;
        std::any _value;

    public:
        SyntaxToken(SyntaxKind kind, int position, std::string text, std::any value);

        SyntaxKind get_kind();
        int get_position();
        std::string get_text();
        std::any get_value();
    };

    class Lexer
    {
    private:
        const std::string _text;
        int _position;
        char get_current();
        void next();

    public:
        Lexer(std::string text);
        SyntaxToken* next_token();
    };
}