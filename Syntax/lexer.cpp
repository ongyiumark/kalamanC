#include "syntax.h"
#include <iostream>

using namespace Syntax;
using namespace Objects;
using namespace Diagnostics;

// This tokenizes the input.
Lexer::Lexer(const std::string& text) : _text(text), _position(0), _ln(0), _col(0) {}

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
    _col++;
    if (current() == '\n')
    {
        _ln++;
        _col = 0;
    }
    _position++;
}

SyntaxToken Lexer::lex()
{
    int start = _position;
    int start_ln = _ln;
    int start_col = _col;

    // Inserts an end of file token at the end.
    if (_position >= (int)_text.size())
        return SyntaxToken(SyntaxKind::EndOfFileToken, Position(_ln, _col, start, start+1), "\0");
    
    

    if (is_letter(current()))
    {
        while(is_valid_identifier(current()))
            next();
        
        int length = _position-start;
        std::string text = _text.substr(start, length);
        SyntaxKind kind = SyntaxFacts::get_keyword_kind(text);
        return SyntaxToken(kind, Position(start_ln, start_col, start, _position), text);
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
        Position curr_pos = Position(start_ln, start_col, start, _position);
        if (dot_count == 0)
        {
            long long x;
            std::istringstream is(text);
            if (is >> x)
                return SyntaxToken(SyntaxKind::IntegerToken, curr_pos, text);

            Diagnostics::DiagnosticBag::report_invalid_type(text, type_to_string(Type::INTEGER), curr_pos);
            return SyntaxToken(SyntaxKind::IntegerToken, curr_pos, text);            
        }
        else
        {
            long double x;
            std::istringstream is(text);
            if (is >> x)
                return SyntaxToken(SyntaxKind::DoubleToken, curr_pos, text);
    
            Diagnostics::DiagnosticBag::report_invalid_type(text, type_to_string(Type::DOUBLE), curr_pos);
            return SyntaxToken(SyntaxKind::DoubleToken, curr_pos, text);    
        }
    }

    if (std::isspace(current()))
    {
        while(std::isspace(current()))
            next();

        int length = _position-start;
        std::string text = _text.substr(start, length);
        return SyntaxToken(SyntaxKind::WhitespaceToken, Position(start_ln, start_col, start, _position), text);
    }

    switch(current())
    {
        case '+':
            next();
            return SyntaxToken(SyntaxKind::PlusToken, Position(start_ln, start_col, start, _position), "+");
        case '-':
            next();
            return SyntaxToken(SyntaxKind::MinusToken, Position(start_ln, start_col, start, _position), "-");
        case '*':
            next();
            return SyntaxToken(SyntaxKind::StarToken, Position(start_ln, start_col, start, _position), "*");
        case '/':
            if (look_ahead() == '/')
            {
                while(current() && current() != '\n')
                    next();
                
                int length = _position-start;
                std::string text = _text.substr(start, length);
                return SyntaxToken(SyntaxKind::CommentToken, Position(start_ln, start_col, start, _position), text);
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
                return SyntaxToken(SyntaxKind::CommentToken, Position(start_ln, start_col, start, _position), text);
            }
            next();
            return SyntaxToken(SyntaxKind::SlashToken, Position(start_ln, start_col, start, _position), "/");
        case '%':
            next();
            return SyntaxToken(SyntaxKind::ModuloToken, Position(start_ln, start_col, start, _position), "%");
        case '^':
            next();
            return SyntaxToken(SyntaxKind::PowerToken, Position(start_ln, start_col, start, _position), "^");
        case '=':
        {
            if (look_ahead() == '=') 
            {
                next(); next();
                return SyntaxToken(SyntaxKind::DEqualsToken, Position(start_ln, start_col, start, _position), "==");
            }
            next();
            return SyntaxToken(SyntaxKind::EqualsToken, Position(start_ln, start_col, start, _position), "=");
        }
        case '!':
        {
            if (look_ahead() == '=') 
            {
                next(); next();
                return SyntaxToken(SyntaxKind::BangEqualsToken, Position(start_ln, start_col, start, _position), "!=");
            }
            next();
            return SyntaxToken(SyntaxKind::BangToken, Position(start_ln, start_col, start, _position), "!");
        }
        case '<':
        {
            if (look_ahead() == '=') 
            {
                next(); next();
                return SyntaxToken(SyntaxKind::LessEqualsToken, Position(start_ln, start_col, start, _position), "<=");
            }
            next();
            return SyntaxToken(SyntaxKind::LessThanToken, Position(start_ln, start_col, start, _position), "<");
        }
        case '>':
        {
            if (look_ahead() == '=') 
            {
                next(); next();
                return SyntaxToken(SyntaxKind::GreaterEqualsToken, Position(start_ln, start_col, start, _position), ">=");              
            }
            next();
            return SyntaxToken(SyntaxKind::GreaterThanToken, Position(start_ln, start_col, start, _position), ">");
        }
        case '&':
        {
            if (look_ahead() == '&') 
            {
                next(); next();
                return SyntaxToken(SyntaxKind::DAmpersandToken, Position(start_ln, start_col, start, _position), "&&");  
            }
            break;           
        }
        case '|':
        {
            if (look_ahead() == '|') 
            {
                next(); next();
                return SyntaxToken(SyntaxKind::DPipeToken, Position(start_ln, start_col, start, _position), "||");
            }
            break;           
        }
        case '(':
            next();
            return SyntaxToken(SyntaxKind::LParenToken, Position(start_ln, start_col, start, _position), "(");
        case ')':
            next();
            return SyntaxToken(SyntaxKind::RParenToken, Position(start_ln, start_col, start, _position), ")");
        case '[':
            next();
            return SyntaxToken(SyntaxKind::LSquareToken, Position(start_ln, start_col, start, _position), "[");
        case ']':
            next();
            return SyntaxToken(SyntaxKind::RSquareToken, Position(start_ln, start_col, start, _position), "]");
        case '{':
            next();
            return SyntaxToken(SyntaxKind::LCurlyToken, Position(start_ln, start_col, start, _position), "{");
        case '}':  
            next();
            return SyntaxToken(SyntaxKind::RCurlyToken, Position(start_ln, start_col, start, _position), "}");
        case ',':
            next();
            return SyntaxToken(SyntaxKind::CommaToken, Position(start_ln, start_col, start, _position), ",");
        case ';':
            next();
            return SyntaxToken(SyntaxKind::SemicolonToken, Position(start_ln, start_col, start, _position), ";");
        case '"':
        {
            next();
            while(current() && current() != '"')
                next();
            
            int length = _position-start;
            std::string text = _text.substr(start+1, length-1);
            Position curr_pos = Position(start_ln, start_col, start, _position);
            if (current() != '"')
            {
                Diagnostics::DiagnosticBag::report_expected_character('"', curr_pos);
                return SyntaxToken(SyntaxKind::StringToken, curr_pos, text);
            }
            next();
                       
            return SyntaxToken(SyntaxKind::StringToken, curr_pos, text);
        }
    }

    // Report and return a bad character.
    std::string text = _text.substr(start, 1);
    next();
    Position curr_pos = Position(start_ln, start_col, start, _position);
    Diagnostics::DiagnosticBag::report_bad_character(current(), curr_pos);

    return SyntaxToken(SyntaxKind::BadToken, curr_pos, text);
}

