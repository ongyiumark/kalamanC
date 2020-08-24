#include "syntax.h"
#include <iostream>

using namespace Syntax;
using namespace Objects;

// This tokenizes the input.
Lexer::Lexer(const std::string& text) : _text(text), _position(0) {}

char Lexer::peek(int offset) const
{
    unsigned int index = _position+offset;
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

SyntaxToken Lexer::lex()
{
    // Inserts an end of file token at the end.
    if (_position >= (int)_text.size())
        return SyntaxToken(SyntaxKind::EndOfFileToken, _position, "\0");
    
    int start = _position;

    if (is_letter(current()))
    {
        while(is_valid_identifier(current()))
            next();
        
        int length = _position-start;
        std::string text = _text.substr(start, length);
        SyntaxKind kind = SyntaxFacts::get_keyword_kind(text);
        return SyntaxToken(kind, start, text);
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
            long long x;
            std::istringstream is(text);
            if (is >> x)
                return SyntaxToken(SyntaxKind::IntegerToken, start, text);

            _diagnostics->report_invalid_type(text, type_to_string(Type::INTEGER));
            return SyntaxToken(SyntaxKind::IntegerToken, start, text);            
        }
        else
        {
            long double x;
            std::istringstream is(text);
            if (is >> x)
                return SyntaxToken(SyntaxKind::DoubleToken, start, text);
    
            _diagnostics->report_invalid_type(text, type_to_string(Type::DOUBLE));
            return SyntaxToken(SyntaxKind::DoubleToken, start, text);    
        }
    }

    if (std::isspace(current()))
    {
        while(std::isspace(current()))
            next();

        int length = _position-start;
        std::string text = _text.substr(start, length);
        return SyntaxToken(SyntaxKind::WhitespaceToken, start, text);
    }

    switch(current())
    {
        case '+':
            return SyntaxToken(SyntaxKind::PlusToken, _position++, "+");
        case '-':
            return SyntaxToken(SyntaxKind::MinusToken, _position++, "-");
        case '*':
            return SyntaxToken(SyntaxKind::StarToken, _position++, "*");
        case '/':
            if (look_ahead() == '/')
            {
                while(current() && current() != '\n')
                    next();
                
                int length = _position-start;
                std::string text = _text.substr(start, length);
                return SyntaxToken(SyntaxKind::CommentToken, start, text);
            }
            else if (look_ahead() == '*')
            {
                next(); next();
                while(true)
                {
                    if (current() == '*' && look_ahead() == '/')
                    {
                        next(); next();
                        break;
                    }
                    if (!current())
                        break;
                    next();
                }
                int length = _position-start;
                std::string text = _text.substr(start, length);
                return SyntaxToken(SyntaxKind::CommentToken, start, text);
            }
            return SyntaxToken(SyntaxKind::SlashToken, _position++, "/");
        case '%':
            return SyntaxToken(SyntaxKind::ModuloToken, _position++, "%");
        case '^':
            return SyntaxToken(SyntaxKind::PowerToken, _position++, "^");
        case '=':
        {
            if (look_ahead() == '=') 
            {
                _position+=2;
                return SyntaxToken(SyntaxKind::DEqualsToken, start, "==");
            }
            return SyntaxToken(SyntaxKind::EqualsToken, _position++, "=");
        }
        case '!':
        {
            if (look_ahead() == '=') 
            {
                _position+=2;
                return SyntaxToken(SyntaxKind::BangEqualsToken, start, "!=");
            }
            return SyntaxToken(SyntaxKind::BangToken, _position++, "!");
        }
        case '<':
        {
            if (look_ahead() == '=') 
            {
                _position+=2;
                return SyntaxToken(SyntaxKind::LessEqualsToken, start, "<=");
            }
            return SyntaxToken(SyntaxKind::LessThanToken, _position++, "<");
        }
        case '>':
        {
            if (look_ahead() == '=') 
            {
                _position+=2;
                return SyntaxToken(SyntaxKind::GreaterEqualsToken, start, ">=");              
            }
            return SyntaxToken(SyntaxKind::GreaterThanToken, _position++, ">");
        }
        case '&':
        {
            if (look_ahead() == '&') 
            {
                _position+=2;
                return SyntaxToken(SyntaxKind::DAmpersandToken, start, "&&");  
            }
            break;           
        }
        case '|':
        {
            if (look_ahead() == '|') 
            {
                _position+=2;
                return SyntaxToken(SyntaxKind::DPipeToken, start, "||");
            }
            break;           
        }
        case '(':
            return SyntaxToken(SyntaxKind::LParenToken, _position++, "(");
        case ')':
            return SyntaxToken(SyntaxKind::RParenToken, _position++, ")");
        case '[':
            return SyntaxToken(SyntaxKind::LSquareToken, _position++, "[");
        case ']':
            return SyntaxToken(SyntaxKind::RSquareToken, _position++, "]");
        case '{':
            return SyntaxToken(SyntaxKind::LCurlyToken, _position++, "{");
        case '}':
            return SyntaxToken(SyntaxKind::RCurlyToken, _position++, "}");
        case ',':
            return SyntaxToken(SyntaxKind::CommaToken, _position++, ",");
        case ';':
            return SyntaxToken(SyntaxKind::SemicolonToken, _position++, ";");
        case '"':
        {
            next();
            while(current() && current() != '"')
                next();
            
            int length = _position-start;
            std::string text = _text.substr(start+1, length-1);
            if (current() != '"')
            {
                _diagnostics->report_expected_character('"');
                return SyntaxToken(SyntaxKind::StringToken, start, text);
            }
            next();
                       
            return SyntaxToken(SyntaxKind::StringToken, start, text);
        }
    }

    // Report and return a bad character.
    std::string text = _text.substr(start, 1);
    _diagnostics->report_bad_character(current());
    return SyntaxToken(SyntaxKind::BadToken, _position++, text);
}

