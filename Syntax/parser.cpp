#include "Expressions/syntax-expressions.h"

using namespace Syntax;

Parser::Parser(std::string& text) : _position(0)
{
    Syntax::Lexer* lexer = new Syntax::Lexer(text);
    SyntaxToken* token;
    do
    {
        token = lexer->lex();
        switch(token->kind())
        {
            case SyntaxKind::BadToken:
            case SyntaxKind::WhitespaceToken:
                break;
            default:
                _tokens.push_back(token);
        }
    } while (token->kind() != Syntax::SyntaxKind::EndOfFileToken);
}

SyntaxToken* Parser::peek(int offset) const
{  
    int index = _position+offset;
    if (index >= _tokens.size()) 
        index = _tokens.size()-1;
    
    return _tokens[index];
}

SyntaxToken* Parser::current() const
{
    return peek(0);
}

SyntaxToken* Parser::look_ahead() const
{
    return peek(1);
}

SyntaxToken* Parser::next_token()
{
    SyntaxToken* curr = current();
    _position++;
    return curr;
}

SyntaxToken* Parser::match_token(SyntaxKind kind)
{
    if (current()->kind() == kind)
        return next_token();

    _diagnostics->report_unexpected_token(kind_to_string(current()->kind()), kind_to_string(kind));
    return new SyntaxToken(kind, current()->get_position(), "\0", NULL);
}

SyntaxNode* Parser::parse()
{
    SyntaxNode* program = parse_program();
    match_token(SyntaxKind::EndOfFileToken);
    return program;
}

SyntaxNode* Parser::parse_program(bool sub_program)
{
    std::vector<SyntaxNode*> program_seq;
    if (sub_program)
    {
        while(current()->kind() != SyntaxKind::RCurlyToken)
        {
            program_seq.push_back(parse_statement());
            switch(current()->kind())
            {
                case SyntaxKind::RParenToken:
                case SyntaxKind::RSquareToken:
                case SyntaxKind::CommaToken:
                {
                    _diagnostics->report_unexpected_token(kind_to_string(current()->kind()), kind_to_string(SyntaxKind::SemicolonToken));
                    next_token();
                }
                case SyntaxKind::EndOfFileToken:
                {
                    match_token(SyntaxKind::RCurlyToken);
                    return new SequenceExpressionSyntax(program_seq); 
                }
            }
        }
        next_token();
        return new SequenceExpressionSyntax(program_seq); 
    }

    while(current()->kind() != SyntaxKind::EndOfFileToken)
    {
        program_seq.push_back(parse_statement());
        switch(current()->kind())
        {
            case SyntaxKind::RParenToken:
            case SyntaxKind::RSquareToken:
            case SyntaxKind::RCurlyToken:
            case SyntaxKind::CommaToken:
            {
                _diagnostics->report_unexpected_token(kind_to_string(current()->kind()), kind_to_string(SyntaxKind::SemicolonToken));
                next_token();
            }
        }
    }
        
    return new SequenceExpressionSyntax(program_seq);
}

SyntaxNode* Parser::parse_statement()
{
    switch(current()->kind())
    {
        case SyntaxKind::LCurlyToken:
        {
            next_token();
            SyntaxNode* sub_program = parse_program(true);
            return sub_program;
        }
        case SyntaxKind::IfKeyword:
        {
            std::vector<SyntaxNode*> conditions;
            std::vector<SyntaxNode*> bodies;
            SyntaxNode* else_body = NULL;

            next_token();
            match_token(SyntaxKind::LParenToken);
            SyntaxNode* condition = parse_expression();
            match_token(SyntaxKind::RParenToken);
            SyntaxNode* body = parse_statement();

            conditions.push_back(condition);
            bodies.push_back(body);

            while(current()->kind() == SyntaxKind::ElifKeyword)
            {
                next_token();

                match_token(SyntaxKind::LParenToken);
                condition = parse_expression();
                match_token(SyntaxKind::RParenToken);
                body = parse_statement();

                conditions.push_back(condition);
                bodies.push_back(body);
            }

            if (current()->kind() == SyntaxKind::ElseKeyword)
            {
                next_token();
                else_body = parse_statement();
            }

            return new IfExpressionSyntax(conditions, bodies, else_body);
        }
        case SyntaxKind::WhileKeyword:
        {
            next_token();
            match_token(SyntaxKind::LParenToken);
            SyntaxNode* condition = parse_expression();
            match_token(SyntaxKind::RParenToken);

            SyntaxNode* body = parse_statement();
            return new WhileExpressionSyntax(condition, body);
        }
        case SyntaxKind::ForKeyword:
        {
            next_token();
            match_token(SyntaxKind::LParenToken);
            SyntaxNode* init = parse_expression();
            match_token(SyntaxKind::SemicolonToken);
            SyntaxNode* condition = parse_expression();
            match_token(SyntaxKind::SemicolonToken);
            SyntaxNode* update = parse_expression();
            match_token(SyntaxKind::RParenToken);

            SyntaxNode* body = parse_statement();
            return new ForExpressionSyntax(init, condition, update, body);
        }
        case SyntaxKind::DefineFunctionKeyword:
        {
            next_token();
            SyntaxToken* identifier = match_token(SyntaxKind::IdentifierToken);
            match_token(SyntaxKind::LParenToken);
            std::vector<SyntaxToken*> arg_names;

            if (current()->kind() != SyntaxKind::RParenToken)
            {
                SyntaxToken* arg_name = match_token(SyntaxKind::IdentifierToken);
                arg_names.push_back(arg_name);
                while(current()->kind() == SyntaxKind::CommaToken)
                {
                    next_token();
                    arg_name = match_token(SyntaxKind::IdentifierToken);
                    arg_names.push_back(arg_name);
                }
            }
            
            match_token(SyntaxKind::RParenToken);
            SyntaxNode* body = parse_statement();
            return new FuncDefineExpressionSyntax(identifier, arg_names, body);
        }
        case SyntaxKind::ReturnKeyword:
        {
            next_token();
            if (current()->kind() == SyntaxKind::SemicolonToken)
            {
                next_token();
                return new ReturnExpressionSyntax(NULL);
            }

            SyntaxNode* expression = parse_expression();
            match_token(SyntaxKind::SemicolonToken);
            return new ReturnExpressionSyntax(expression);
        }
        case SyntaxKind::BreakKeyword:
        {
            next_token();
            match_token(SyntaxKind::SemicolonToken);
            return new BreakExpressionSyntax();
        }
        case SyntaxKind::ContinueKeyword:
        {
            next_token();
            match_token(SyntaxKind::SemicolonToken);
            return new ContinueExpressionSyntax();
        }
        case SyntaxKind::SemicolonToken:
        {
            next_token();
            return new NoneExpressionSyntax();
        }
        default:
        {
            SyntaxNode* expression = parse_expression();
            match_token(SyntaxKind::SemicolonToken);
            return expression;
            break;
        }
    }
}

SyntaxNode* Parser::parse_expression(int precedence)
{
    SyntaxNode* left;
    int unary_precedence = SyntaxFacts::get_unary_precedence(current()->kind());
    if (unary_precedence && unary_precedence >= precedence)
    {
        SyntaxToken* op_token = next_token();
        SyntaxNode* expression = parse_expression(unary_precedence);
        left = new UnaryExpressionSyntax(op_token, expression);
    }
    else 
    {
        switch(current()->kind())
        {
            case SyntaxKind::IntegerKeyword:
            case SyntaxKind::DoubleKeyword:
            case SyntaxKind::BooleanKeyword:
            case SyntaxKind::ListKeyword:
            case SyntaxKind::FunctionKeyword:
            case SyntaxKind::StringKeyword:
            {
                SyntaxToken* var_keyword = next_token();
                SyntaxToken* identifier = match_token(SyntaxKind::IdentifierToken);
                SyntaxNode* var_decl = new VarDeclareExpressionSyntax(var_keyword, identifier);
                if (current()->kind() == SyntaxKind::SemicolonToken)
                    return var_decl;
                
                match_token(SyntaxKind::EqualsToken);
                SyntaxNode* expression = parse_expression(precedence);
                SyntaxNode* var_ass = new VarAssignExpressionSyntax(identifier, expression);
                std::vector<SyntaxNode*> seq = {var_decl, var_ass};
                return new SequenceExpressionSyntax(seq);
            }
            case SyntaxKind::IdentifierToken:
            {
                if (look_ahead()->kind() == SyntaxKind::EqualsToken)
                {
                    SyntaxToken* identifier = next_token();
                    next_token();
                    SyntaxNode* expression = parse_expression(precedence);
                    return new VarAssignExpressionSyntax(identifier, expression);
                }
            }        
        }
        left = parse_molecule();
    }

    while(true)
    {
        int binary_precedence = SyntaxFacts::get_binary_precedence(current()->kind());
        if (!binary_precedence || binary_precedence <= precedence)
            break;
        
        SyntaxToken* op_token = next_token();
        SyntaxNode* right = parse_expression(binary_precedence);
        left = new BinaryExpressionSyntax(left, op_token, right);
    }
    return left;
}

SyntaxNode* Parser::parse_molecule()
{
    SyntaxNode* left = parse_atom();
    switch(current()->kind())
    {
        case SyntaxKind::LSquareToken:
        {
            while(current()->kind() == SyntaxKind::LSquareToken)
            {
                next_token();
                SyntaxNode* right = parse_expression();
                match_token(SyntaxKind::RSquareToken);
                left = new IndexExpressionSyntax(left, right);
            }
        }
        default:
            return left;        
    }
}

SyntaxNode* Parser::parse_atom()
{
    switch(current()->kind())
    {
        case SyntaxKind::IntegerToken:
        case SyntaxKind::StringToken:
        case SyntaxKind::DoubleToken:
        {
            SyntaxToken* token = next_token();
            return new LiteralExpressionSyntax(token);
        }
        case SyntaxKind::TrueKeyword:
        case SyntaxKind::FalseKeyword:
        {
            SyntaxToken* keyword = next_token();
            bool value = keyword->kind() == SyntaxKind::TrueKeyword;
            return new LiteralExpressionSyntax(keyword, new Objects::Boolean(value));
        }
        case SyntaxKind::LParenToken:
        {
            next_token();
            SyntaxNode* expression = parse_expression();
            match_token(SyntaxKind::RParenToken);
            return expression;
        }
        case SyntaxKind::LSquareToken:
        {
            next_token();
            std::vector<SyntaxNode*> elements;
            if (current()->kind() == SyntaxKind::RSquareToken)
            {
                next_token();
                return new SequenceExpressionSyntax(elements, true);
            }

            SyntaxNode* expression = parse_expression();
            elements.push_back(expression);

            while(current()->kind() == SyntaxKind::CommaToken)
            {
                next_token();
                SyntaxNode* expression = parse_expression();
                elements.push_back(expression);
            }
            match_token(SyntaxKind::RSquareToken);
            return new SequenceExpressionSyntax(elements, true);
        }
        case SyntaxKind::PrintFunction:
        case SyntaxKind::InputFunction:
        {
            SyntaxToken* identifier = next_token();
            match_token(SyntaxKind::LParenToken);
            std::vector<SyntaxNode*> args;
            if (current()->kind() == SyntaxKind::RParenToken)
            {
                next_token();
                return new FuncCallExpressionSyntax(identifier, args);
            }

            SyntaxNode* expression = parse_expression();
            args.push_back(expression);

            while(current()->kind() == SyntaxKind::CommaToken)
            {
                next_token();
                SyntaxNode* expression = parse_expression();
                args.push_back(expression);
            }
            match_token(SyntaxKind::RParenToken);
            return new FuncCallExpressionSyntax(identifier, args);
        }
        default:
        {
            SyntaxToken* identifier = match_token(SyntaxKind::IdentifierToken);
            if (current()->kind() == SyntaxKind::LParenToken)
            {
                next_token();
                std::vector<SyntaxNode*> args;
                if (current()->kind() == SyntaxKind::RParenToken)
                {
                    next_token();
                    return new FuncCallExpressionSyntax(identifier, args);
                }

                SyntaxNode* expression = parse_expression();
                args.push_back(expression);

                while(current()->kind() == SyntaxKind::CommaToken)
                {
                    next_token();
                    SyntaxNode* expression = parse_expression();
                    args.push_back(expression);
                }
                match_token(SyntaxKind::RParenToken);
                return new FuncCallExpressionSyntax(identifier, args);
            }
            return new VarAccessExpressionSyntax(identifier);
        }
    }
}