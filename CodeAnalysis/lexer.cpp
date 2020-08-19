#include "code-analysis.h"

using namespace CodeAnalysis;

Lexer::Lexer(std::string text) : _text(text), _position(0) {}

char Lexer::get_current()
{
    if (_position >= _text.size()) return '\0';
    return _text[_position];
}

void Lexer::next()
{
    _position++;
}

SyntaxToken* Lexer::next_token()
{
    if (_position >= _text.size())
        return new SyntaxToken(SyntaxKind::EndOfFileToken, _position, "\0", NULL);

    int start = _position;

    switch(get_current())
    {
        case '0': case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9':
        {
            while(is_digit(get_current()))
                next();
            
            int length = _position-start;
            std::string text = _text.substr(start, length);
            int value = stoi(text);
            return new SyntaxToken(SyntaxKind::NumberToken, start, text, value);
        }
        case ' ': case '\t': case '\n': case '\v': case '\f': case '\r':
        {
            while (std::isspace(get_current()))
                next();
            
            int length = _position-start;
            std::string text = _text.substr(start, length);
            return new SyntaxToken(SyntaxKind::WhitespaceToken, start, text, NULL);
        }
        case '+':
            return new SyntaxToken(SyntaxKind::PlusToken, _position++, "+", NULL);
        case '-':
            return new SyntaxToken(SyntaxKind::MinusToken, _position++, "-", NULL);
        case '*':
            return new SyntaxToken(SyntaxKind::StarToken, _position++, "*", NULL);
        case '/':
            return new SyntaxToken(SyntaxKind::SlashToken, _position++, "/", NULL);
        case '(':
            return new SyntaxToken(SyntaxKind::LParenToken, _position++, "(", NULL);
        case ')':
            return new SyntaxToken(SyntaxKind::RParenToken, _position++, ")", NULL);
        default:
            return new SyntaxToken(SyntaxKind::BadToken, _position++, _text.substr(_position-1, 1), NULL);
    }
}