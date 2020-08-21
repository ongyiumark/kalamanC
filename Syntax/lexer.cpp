#include "syntax.h"
#include <iostream>

using namespace Syntax;

Lexer::Lexer(const std::string& text) : _text(text), _position(0) {}

char Lexer::peek(int offset) const
{
    int index = _position+offset;
    if (index > _text.size()) return '\0';
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
        while(is_valid_identifier(current()))
            next();
        
        int length = _position-start;
        std::string text = _text.substr(start, length);
        SyntaxKind kind = SyntaxFacts::get_keyword_kind(text);
        return new SyntaxToken(kind, start, text, NULL);
    }

    if (is_digit(current()))
    {
        int dot_count = 0;
        while(is_digit(current()) || current() == '.')
        {
            if (current() == '.')
                dot_count++;
            if (dot_count > 1)
                break;
            next();
        }

        int length = _position-start;
        std::string text = _text.substr(start, length);
        if (dot_count == 0)
        {
            int x;
            std::istringstream is(text);
            if (is >> x)
                return new SyntaxToken(SyntaxKind::IntegerToken, start, text, new Objects::Integer(x));

            _diagnostics->report_invalid_type(Diagnostics::Position(text, start), Objects::Type::INTEGER);
            return new SyntaxToken(SyntaxKind::IntegerToken, start, text, NULL);            
        }
        else
        {
            double x;
            std::istringstream is(text);
            if (is >> x)
                return new SyntaxToken(SyntaxKind::DoubleToken, start, text, new Objects::Double(x));
    
            _diagnostics->report_invalid_type(Diagnostics::Position(text, start), Objects::Type::DOUBLE);
            return new SyntaxToken(SyntaxKind::DoubleToken, start, text, NULL);    
        }
    }

    if (std::isspace(current()))
    {
        while(std::isspace(current()))
            next();

        int length = _position-start;
        std::string text = _text.substr(start, length);
        return new SyntaxToken(SyntaxKind::WhitespaceToken, start, text, NULL);
    }

    switch(current())
    {
        case '+':
            return new SyntaxToken(SyntaxKind::PlusToken, _position++, "+", NULL);
        case '-':
            return new SyntaxToken(SyntaxKind::MinusToken, _position++, "-", NULL);
        case '*':
            return new SyntaxToken(SyntaxKind::StarToken, _position++, "*", NULL);
        case '/':
            return new SyntaxToken(SyntaxKind::SlashToken, _position++, "/", NULL);
        case '%':
            return new SyntaxToken(SyntaxKind::ModuloToken, _position++, "%", NULL);
        case '^':
            return new SyntaxToken(SyntaxKind::PowerToken, _position++, "^", NULL);
        case '=':
        {
            if (look_ahead() == '=') 
            {
                _position+=2;
                return new SyntaxToken(SyntaxKind::DEqualsToken, start, "==", NULL);
            }
            return new SyntaxToken(SyntaxKind::EqualsToken, _position++, "=", NULL);
        }
        case '!':
        {
            if (look_ahead() == '=') 
            {
                _position+=2;
                return new SyntaxToken(SyntaxKind::BangEqualsToken, start, "!=", NULL);
            }
            return new SyntaxToken(SyntaxKind::BangToken, _position++, "!", NULL);
        }
        case '<':
        {
            if (look_ahead() == '=') 
            {
                _position+=2;
                return new SyntaxToken(SyntaxKind::LessEqualsToken, start, "<=", NULL);
            }
            return new SyntaxToken(SyntaxKind::LessThanToken, _position++, "<", NULL);
        }
        case '>':
        {
            if (look_ahead() == '=') 
            {
                _position+=2;
                return new SyntaxToken(SyntaxKind::GreaterEqualsToken, start, ">=", NULL);              
            }
            return new SyntaxToken(SyntaxKind::GreaterThanToken, _position++, ">", NULL);
        }
        case '&':
        {
            if (look_ahead() == '&') 
            {
                _position+=2;
                return new SyntaxToken(SyntaxKind::DAmpersandToken, start, "&&", NULL);  
            }
            break;           
        }
        case '|':
        {
            if (look_ahead() == '|') 
            {
                _position+=2;
                return new SyntaxToken(SyntaxKind::DPipeToken, start, "||", NULL);
            }
            break;           
        }
        case '(':
            return new SyntaxToken(SyntaxKind::LParenToken, _position++, "(", NULL);
        case ')':
            return new SyntaxToken(SyntaxKind::RParenToken, _position++, ")", NULL);
        case '[':
            return new SyntaxToken(SyntaxKind::LSquareToken, _position++, "[", NULL);
        case ']':
            return new SyntaxToken(SyntaxKind::RSquareToken, _position++, "]", NULL);
        case '{':
            return new SyntaxToken(SyntaxKind::LCurlyToken, _position++, "{", NULL);
        case '}':
            return new SyntaxToken(SyntaxKind::RCurlyToken, _position++, "}", NULL);
        case ',':
            return new SyntaxToken(SyntaxKind::CommaToken, _position++, ",", NULL);
        case ';':
            return new SyntaxToken(SyntaxKind::SemicolonToken, _position++, ";", NULL);
        case '"':
        {
            next();
            while(current() && current() != '"')
                next();
            
            int length = _position-start;
            std::string text = _text.substr(start+1, length-1);
            if (current() != '"')
            {
                _diagnostics->report_expected_character(Diagnostics::Position(_text.substr(start,length), _position), '"');
                return new SyntaxToken(SyntaxKind::StringToken, start, text, NULL);
            }
            next();
                       
            return new SyntaxToken(SyntaxKind::StringToken, start, text, new Objects::String(text));
        }
    }

    std::string text = _text.substr(start, 1);
    _diagnostics->report_bad_character(Diagnostics::Position(text, _position), current());
    return new SyntaxToken(SyntaxKind::BadToken, _position++, text, NULL);
}

