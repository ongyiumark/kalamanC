#include "syntax.h"

using namespace CodeAnalysis;

Lexer::Lexer(const std::string& text) : _text(text), _position(0) {}

char Lexer::peek(int offset) const
{
    int index = _position+offset;
    if (index >= _text.size()) return '\0';
    return _text[index];
}

char Lexer::current() const
{
    return peek(0);
}

char Lexer::look_ahead() const
{
    return peek(1);
}

void Lexer::next()
{
    _position++;
}

SyntaxToken* Lexer::lex()
{
    if (_position >= _text.size())
        return new SyntaxToken(SyntaxKind::EndOfFileToken, _position, "\0", NULL);

    int start = _position;
    
    if (is_letter(current()))
    {
        while (is_letter(current()))
            next();
        
        int length = _position-start;
        std::string text = _text.substr(start, length);
        SyntaxKind kind = SyntaxFacts::get_keyword_kind(text);
        return new SyntaxToken(kind, start, text, NULL);
    }

    switch(current())
    {
        case '0': case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9':
        {
            while(is_digit(current()))
                next();
            
            int length = _position-start;
            std::string text = _text.substr(start, length);
            int value;
            
            std::istringstream is(text);
            if (is >> value)
                return new SyntaxToken(SyntaxKind::NumberToken, start, text, std::make_any<int>(value));
            else
            {
                std::ostringstream os;
                os << "The number " << text << " is not a valid int";
                _diagnostics.push_back(os.str());
                return new SyntaxToken(SyntaxKind::NumberToken, start, text, NULL);
            }

            return new SyntaxToken(SyntaxKind::NumberToken, start, text, std::make_any<int>(value));
        }
        case ' ': case '\t': case '\n': case '\v': case '\f': case '\r':
        {
            while (std::isspace(current()))
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
        case '!':
            return new SyntaxToken(SyntaxKind::BangToken, _position++, "!", NULL);
        case '&':
            if (look_ahead() == '&')
                return new SyntaxToken(SyntaxKind::DAmpersandToken, _position+=2, "&&", NULL);
        case '|':
            if (look_ahead() == '|')
                return new SyntaxToken(SyntaxKind::DPipeToken, _position+=2, "&&", NULL);
        default:
        {
            std::ostringstream os;
            os << "ERROR: bad character in input: '" << current() << "'";
            _diagnostics.push_back(os.str());
            return new SyntaxToken(SyntaxKind::BadToken, _position++, _text.substr(_position-1, 1), NULL);
        }
    }
}

int Lexer::get_diagnostics_size() const
{
    return _diagnostics.size();
}

std::string Lexer::get_diagnostic(int i) const
{
    int n = _diagnostics.size();
    if (n == 0) return NULL;
    if (i < 0) return _diagnostics[0];
    if (i >= n) return _diagnostics[n-1];
    return _diagnostics[i];
}

std::vector<std::string> Lexer::get_diagnostics() const
{
    return _diagnostics;
}