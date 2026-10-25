#include "Expressions/syntax-expressions.h"

#include <iostream>
using namespace Syntax;
using namespace Objects;
using namespace Diagnostics;

// This turns the tokens into a tree of syntax. 
Parser::Parser(std::string& text, bool show_return) : _position(0)
{
    Lexer lexer(text);
    SyntaxToken token;
    do
    {
        token = lexer.lex();
        switch(token.kind())
        {
            case SyntaxKind::BadToken:
            case SyntaxKind::WhitespaceToken:
            case SyntaxKind::CommentToken:
                break;
            default:
                _tokens.push_back(token);
        }
    } while (token.kind() != SyntaxKind::EndOfFileToken);
}

SyntaxToken Parser::peek(int offset) const
{  
    int index = _position + offset;
    if (index < 0) index = 0;
    if (index >= (int)_tokens.size()) 
        index = (int)_tokens.size() - 1;
    
    return _tokens[index];
}

SyntaxToken Parser::current() const
{
    return peek(0);
}

SyntaxToken Parser::look_ahead() const
{
    return peek(1);
}

SyntaxToken Parser::next_token()
{
    SyntaxToken curr = current();
    _position++;
    return curr;
}

// Inserts the relevant token, but reports an error.
SyntaxToken Parser::match_token(SyntaxKind kind)
{
    if (current().kind() == kind)
        return next_token();

    Diagnostics::DiagnosticBag::report_unexpected_token(kind_to_display_string(current().kind()), 
        kind_to_display_string(kind), current().get_pos());
    return SyntaxToken(kind, current().get_pos(), "\0");
}

// Refer to grammar.txt for the full summary of syntax.
SyntaxNode* Parser::parse()
{
    SyntaxNode* program = parse_program();
    match_token(SyntaxKind::EndOfFileToken);
    return program;
}

SyntaxNode* Parser::parse_program(bool sub_program)
{
    std::vector<SyntaxNode*> program_seq;
    Position start = current().get_pos();
    // It's a sub-program when it's enclosed in curly braces.
    if (sub_program)
    {
        while(current().kind() != SyntaxKind::RCurlyToken)
        {
            int prev_pos = _position;
            program_seq.push_back(parse_statement());
            switch(current().kind())
            {
                case SyntaxKind::RParenToken:
                case SyntaxKind::RSquareToken:
                case SyntaxKind::CommaToken:
                {
                    Diagnostics::DiagnosticBag::report_unexpected_token(kind_to_display_string(current().kind()), 
                        kind_to_display_string(SyntaxKind::SemicolonToken), current().get_pos());
                    next_token();
                    break;
                }
                case SyntaxKind::EndOfFileToken:
                {
                    match_token(SyntaxKind::RCurlyToken);
                    return new SequenceExpressionSyntax(program_seq, 
                        Position(start.ln, start.col, start.start, current().get_pos().end), 
                        false); 
                }
                default:
                    if (_position == prev_pos)
                        next_token();
                    break;
            }
        }
        next_token();
        return new SequenceExpressionSyntax(program_seq, 
            Position(start.ln, start.col, start.start, current().get_pos().end),  
            false); 
    }

    while(current().kind() != SyntaxKind::EndOfFileToken)
    {
        int prev_pos = _position;
        program_seq.push_back(parse_statement());
        switch(current().kind())
        {
            case SyntaxKind::RParenToken:
            case SyntaxKind::RSquareToken:
            case SyntaxKind::RCurlyToken:
            case SyntaxKind::CommaToken:
            {
                Diagnostics::DiagnosticBag::report_unexpected_token(kind_to_display_string(current().kind()), 
                    kind_to_display_string(SyntaxKind::SemicolonToken), current().get_pos());
                next_token();
                break;
            }
            default:
                if (_position == prev_pos && current().kind() != SyntaxKind::EndOfFileToken)
                    next_token();
                break;
        }
    }
        
    return new SequenceExpressionSyntax(program_seq,
        Position(start.ln, start.col, start.start, current().get_pos().end), 
        false);
}

SyntaxNode* Parser::parse_statement()
{
    switch(current().kind())
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
            SyntaxNode* else_body = nullptr;
            Position start = current().get_pos();
            next_token();
            match_token(SyntaxKind::LParenToken);
            SyntaxNode* condition = parse_expression();
            match_token(SyntaxKind::RParenToken);
            SyntaxNode* body = parse_statement();

            conditions.push_back(condition);
            bodies.push_back(body);

            while(current().kind() == SyntaxKind::ElifKeyword)
            {
                next_token();

                match_token(SyntaxKind::LParenToken);
                condition = parse_expression();
                match_token(SyntaxKind::RParenToken);
                body = parse_statement();

                conditions.push_back(condition);
                bodies.push_back(body);
            }

            if (current().kind() == SyntaxKind::ElseKeyword)
            {
                next_token();
                else_body = parse_statement();
            }

            return new IfExpressionSyntax(conditions, bodies, else_body, 
                Position(start.ln, start.col, start.start, current().get_pos().end));
        }
        case SyntaxKind::WhileKeyword:
        {
            Position start = current().get_pos();
            next_token();
            match_token(SyntaxKind::LParenToken);
            SyntaxNode* condition = parse_expression();
            match_token(SyntaxKind::RParenToken);

            SyntaxNode* body = parse_statement();
            return new WhileExpressionSyntax(condition, body, Position(start.ln, start.col, start.start, current().get_pos().end));
        }
        case SyntaxKind::ForKeyword:
        {
            Position start = current().get_pos();
            next_token();
            match_token(SyntaxKind::LParenToken);

            // Check for foreach loop: for (int x in list) / for (x in list) / for (int x : list)
            bool is_typed_foreach = SyntaxFacts::is_type_keyword(current().kind()) &&
                                    look_ahead().kind() == SyntaxKind::IdentifierToken &&
                                    (peek(2).kind() == SyntaxKind::InKeyword || peek(2).kind() == SyntaxKind::ColonToken);
            bool is_untyped_foreach = current().kind() == SyntaxKind::IdentifierToken &&
                                      (look_ahead().kind() == SyntaxKind::InKeyword || look_ahead().kind() == SyntaxKind::ColonToken);

            if (is_typed_foreach || is_untyped_foreach)
            {
                Position var_start = current().get_pos();
                SyntaxNode* var_node = nullptr;
                if (is_typed_foreach)
                {
                    SyntaxToken var_kw = next_token();
                    SyntaxToken ident = match_token(SyntaxKind::IdentifierToken);
                    var_node = new VarDeclareExpressionSyntax(var_kw, ident,
                        Position(var_start.ln, var_start.col, var_start.start, ident.get_pos().end));
                }
                else
                {
                    SyntaxToken ident = match_token(SyntaxKind::IdentifierToken);
                    var_node = new VarAccessExpressionSyntax(ident, ident.get_pos());
                }
                next_token(); // consume 'in' / 'sa' / ':'
                SyntaxNode* iterable = parse_expression();
                match_token(SyntaxKind::RParenToken);
                SyntaxNode* body = parse_statement();
                return new ForExpressionSyntax(var_node, iterable, nullptr, body,
                    Position(start.ln, start.col, start.start, current().get_pos().end), true);
            }

            SyntaxNode* init = nullptr;
            if (current().kind() == SyntaxKind::SemicolonToken)
                init = new NoneExpressionSyntax(current().get_pos());
            else
                init = parse_expression();
            match_token(SyntaxKind::SemicolonToken);

            SyntaxNode* condition = nullptr;
            if (current().kind() == SyntaxKind::SemicolonToken)
                condition = new LiteralExpressionSyntax(new Boolean(true), current().get_pos());
            else
                condition = parse_expression();
            match_token(SyntaxKind::SemicolonToken);

            SyntaxNode* update = nullptr;
            if (current().kind() == SyntaxKind::RParenToken)
                update = new NoneExpressionSyntax(current().get_pos());
            else
                update = parse_expression();
            match_token(SyntaxKind::RParenToken);

            SyntaxNode* body = parse_statement();
            return new ForExpressionSyntax(init, condition, update, body,
                Position(start.ln, start.col, start.start, current().get_pos().end), false);
        }
        case SyntaxKind::DefineFunctionKeyword:
        {
            Position start = current().get_pos();
            next_token();
            SyntaxToken identifier = match_token(SyntaxKind::IdentifierToken);
            match_token(SyntaxKind::LParenToken);
            std::vector<SyntaxToken> arg_names;

            if (current().kind() != SyntaxKind::RParenToken)
            {
                if (SyntaxFacts::is_type_keyword(current().kind()))
                    next_token();
                SyntaxToken arg_name = match_token(SyntaxKind::IdentifierToken);
                arg_names.push_back(arg_name);
                while(current().kind() == SyntaxKind::CommaToken)
                {
                    next_token();
                    if (SyntaxFacts::is_type_keyword(current().kind()))
                        next_token();
                    arg_name = match_token(SyntaxKind::IdentifierToken);
                    arg_names.push_back(arg_name);
                }
            }
            
            match_token(SyntaxKind::RParenToken);
            SyntaxNode* body = parse_statement();
            return new FuncDefineExpressionSyntax(identifier, arg_names, body,
                Position(start.ln, start.col, start.start, current().get_pos().end));
        }
        case SyntaxKind::ReturnKeyword:
        {
            Position start = current().get_pos();
            next_token();
            if (current().kind() == SyntaxKind::SemicolonToken)
            {
                next_token();
                return new ReturnExpressionSyntax(nullptr, start);
            }

            SyntaxNode* expression = parse_expression();
            match_token(SyntaxKind::SemicolonToken);
            return new ReturnExpressionSyntax(expression, Position(start.ln, start.col, start.start, expression->get_pos().end));
        }
        case SyntaxKind::BreakKeyword:
        {
            Position pos = current().get_pos();
            next_token();
            match_token(SyntaxKind::SemicolonToken);
            return new BreakExpressionSyntax(pos);
        }
        case SyntaxKind::ContinueKeyword:
        {
            Position pos = current().get_pos();
            next_token();
            match_token(SyntaxKind::SemicolonToken);
            return new ContinueExpressionSyntax(pos);
        }
        case SyntaxKind::SemicolonToken:
        {
            Position pos = current().get_pos();
            next_token();
            return new NoneExpressionSyntax(pos);
        }
        default:
        {
            SyntaxNode* expression = parse_expression();
            match_token(SyntaxKind::SemicolonToken);
            return expression;
        }
    }
}

SyntaxNode* Parser::parse_expression(int precedence)
{
    SyntaxNode* left = nullptr;
    int unary_precedence = SyntaxFacts::get_unary_precedence(current().kind());
    if (unary_precedence && unary_precedence >= precedence)
    {
        SyntaxToken op_token = next_token();
        SyntaxNode* expression = parse_expression(unary_precedence);
        left = new UnaryExpressionSyntax(op_token, expression, 
            Position(op_token.get_pos().ln, op_token.get_pos().col, op_token.get_pos().start, expression->get_pos().end));
    }
    else if ((current().kind() == SyntaxKind::PlusPlusToken || current().kind() == SyntaxKind::MinusMinusToken) && precedence <= 6)
    {
        Position start = current().get_pos();
        SyntaxToken op_token = next_token();
        SyntaxNode* target = parse_molecule();
        if (target->kind() == SyntaxKind::VarAccessExpression)
        {
            SyntaxToken ident = *((VarAccessExpressionSyntax*)target)->get_identifier();
            Position end_pos = target->get_pos();
            delete target;
            left = new VarAssignExpressionSyntax(ident, op_token, nullptr,
                Position(start.ln, start.col, start.start, end_pos.end));
        }
        else if (target->kind() == SyntaxKind::IndexExpression)
        {
            Position end_pos = target->get_pos();
            left = new VarAssignExpressionSyntax(target, op_token, nullptr,
                Position(start.ln, start.col, start.start, end_pos.end));
        }
        else
        {
            Diagnostics::DiagnosticBag::report_unexpected_token(kind_to_display_string(op_token.kind()), "variable or list index", start);
            left = target;
        }
    }
    else 
    {
        if (SyntaxFacts::is_type_keyword(current().kind()))
        {
            Position start = current().get_pos();
            SyntaxToken var_keyword = next_token();
            SyntaxToken identifier = match_token(SyntaxKind::IdentifierToken);
            SyntaxNode* var_decl = new VarDeclareExpressionSyntax(var_keyword, identifier,
                Position(start.ln, start.col, start.start, identifier.get_pos().end));
            if (current().kind() != SyntaxKind::EqualsToken)
                return var_decl;
            
            next_token(); // consume '='
            SyntaxNode* expression = parse_expression(0);
            SyntaxNode* var_ass = new VarAssignExpressionSyntax(identifier, expression,
                Position(start.ln, start.col, start.start, expression->get_pos().end));
            std::vector<SyntaxNode*> seq = {var_decl, var_ass};
            return new SequenceExpressionSyntax(seq, Position(start.ln, start.col, start.start, expression->get_pos().end), false);
        }

        left = parse_molecule();
    }

    while(true)
    {
        int binary_precedence = SyntaxFacts::get_binary_precedence(current().kind());
        if (!binary_precedence || binary_precedence <= precedence)
            break;
        
        SyntaxToken op_token = next_token();
        // Power operator '^' is right-associative
        int next_prec = (op_token.kind() == SyntaxKind::PowerToken) ? (binary_precedence - 1) : binary_precedence;
        SyntaxNode* right = parse_expression(next_prec);

        Position left_pos = left->get_pos();
        Position right_pos = right->get_pos();
        left = new BinaryExpressionSyntax(left, op_token, right, 
            Position(left_pos.ln, left_pos.col, left_pos.start, right_pos.end));
    }
    return left;
}

SyntaxNode* Parser::parse_molecule()
{
    SyntaxNode* left = parse_atom();
    while(current().kind() == SyntaxKind::LSquareToken)
    {
        Position start = left->get_pos();
        next_token(); // consume '['

        if (current().kind() == SyntaxKind::ColonToken)
        {
            next_token(); // consume ':'
            SyntaxNode* slice_end = nullptr;
            if (current().kind() != SyntaxKind::RSquareToken)
                slice_end = parse_expression();
            SyntaxToken rsq = match_token(SyntaxKind::RSquareToken);
            left = new IndexExpressionSyntax(left, nullptr, slice_end, true,
                Position(start.ln, start.col, start.start, rsq.get_pos().end));
        }
        else
        {
            SyntaxNode* first_expr = parse_expression();
            if (current().kind() == SyntaxKind::ColonToken)
            {
                next_token(); // consume ':'
                SyntaxNode* slice_end = nullptr;
                if (current().kind() != SyntaxKind::RSquareToken)
                    slice_end = parse_expression();
                SyntaxToken rsq = match_token(SyntaxKind::RSquareToken);
                left = new IndexExpressionSyntax(left, first_expr, slice_end, true,
                    Position(start.ln, start.col, start.start, rsq.get_pos().end));
            }
            else
            {
                SyntaxToken rsq = match_token(SyntaxKind::RSquareToken);
                left = new IndexExpressionSyntax(left, first_expr,
                    Position(start.ln, start.col, start.start, rsq.get_pos().end));
            }
        }
    }

    // Handle postfix ++ / -- or assignment (=, +=, -=, *=, /=, %=, ^=) on variables and index expressions
    if (left->kind() == SyntaxKind::VarAccessExpression || left->kind() == SyntaxKind::IndexExpression)
    {
        if (current().kind() == SyntaxKind::PlusPlusToken || current().kind() == SyntaxKind::MinusMinusToken)
        {
            Position start = left->get_pos();
            SyntaxToken op_token = next_token();
            if (left->kind() == SyntaxKind::VarAccessExpression)
            {
                SyntaxToken ident = *((VarAccessExpressionSyntax*)left)->get_identifier();
                delete left;
                return new VarAssignExpressionSyntax(ident, op_token, nullptr,
                    Position(start.ln, start.col, start.start, op_token.get_pos().end));
            }
            else
            {
                return new VarAssignExpressionSyntax(left, op_token, nullptr,
                    Position(start.ln, start.col, start.start, op_token.get_pos().end));
            }
        }
        else if (SyntaxFacts::is_assignment_operator(current().kind()))
        {
            Position start = left->get_pos();
            SyntaxToken op_token = next_token();
            SyntaxNode* rhs = parse_expression(0);
            if (left->kind() == SyntaxKind::VarAccessExpression)
            {
                SyntaxToken ident = *((VarAccessExpressionSyntax*)left)->get_identifier();
                delete left;
                return new VarAssignExpressionSyntax(ident, op_token, rhs,
                    Position(start.ln, start.col, start.start, rhs->get_pos().end));
            }
            else
            {
                return new VarAssignExpressionSyntax(left, op_token, rhs,
                    Position(start.ln, start.col, start.start, rhs->get_pos().end));
            }
        }
    }

    return left;   
}

SyntaxNode* Parser::parse_atom()
{
    switch(current().kind())
    {
        case SyntaxKind::IntegerToken:
        {
            SyntaxToken literal_token = next_token();
            std::istringstream is(literal_token.get_text());
            long long x = 0;
            if (is >> x) return new LiteralExpressionSyntax(new Integer(x), literal_token.get_pos());
            
            return new LiteralExpressionSyntax(new None(), literal_token.get_pos());
        }
        case SyntaxKind::StringToken:
        {
            SyntaxToken literal_token = next_token();
            return new LiteralExpressionSyntax(new String(literal_token.get_text()), literal_token.get_pos());
        }
        case SyntaxKind::DoubleToken:
        {
            SyntaxToken literal_token = next_token();
            std::istringstream is(literal_token.get_text());
            long double x = 0.0;
            if (is >> x) return new LiteralExpressionSyntax(new Double(x), literal_token.get_pos());
            
            return new LiteralExpressionSyntax(new None(), literal_token.get_pos());
        }
        case SyntaxKind::TrueKeyword:
        case SyntaxKind::FalseKeyword:
        {
            SyntaxToken keyword = next_token();
            bool value = keyword.kind() == SyntaxKind::TrueKeyword;
            return new LiteralExpressionSyntax(new Boolean(value), keyword.get_pos());
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
            Position start = current().get_pos();
            next_token();
            std::vector<SyntaxNode*> elements;
            if (current().kind() == SyntaxKind::RSquareToken)
            {
                SyntaxToken rsq = next_token();
                return new SequenceExpressionSyntax(elements, Position(start.ln, start.col, start.start, rsq.get_pos().end), true);
            }

            SyntaxNode* expression = parse_expression();
            elements.push_back(expression);

            while(current().kind() == SyntaxKind::CommaToken)
            {
                next_token();
                if (current().kind() == SyntaxKind::RSquareToken)
                    break; // allow trailing comma in list literal
                expression = parse_expression();
                elements.push_back(expression);
            }
            SyntaxToken rsq = match_token(SyntaxKind::RSquareToken);
            return new SequenceExpressionSyntax(elements, Position(start.ln, start.col, start.start, rsq.get_pos().end), true);
        }
        default:
        {
            Position start = current().get_pos();
            SyntaxToken identifier;
            if (SyntaxFacts::is_builtin_function(current().kind()) || current().kind() == SyntaxKind::IdentifierToken)
                identifier = next_token();
            else
                identifier = match_token(SyntaxKind::IdentifierToken);

            if (current().kind() == SyntaxKind::LParenToken)
            {
                next_token();
                std::vector<SyntaxNode*> args;
                if (current().kind() == SyntaxKind::RParenToken)
                {
                    SyntaxToken rp = next_token();
                    return new FuncCallExpressionSyntax(identifier, args,
                        Position(start.ln, start.col, start.start, rp.get_pos().end));
                }

                SyntaxNode* expression = parse_expression();
                args.push_back(expression);

                while(current().kind() == SyntaxKind::CommaToken)
                {
                    next_token();
                    expression = parse_expression();
                    args.push_back(expression);
                }
                SyntaxToken rp = match_token(SyntaxKind::RParenToken);
                return new FuncCallExpressionSyntax(identifier, args,
                    Position(start.ln, start.col, start.start, rp.get_pos().end));
            }
            return new VarAccessExpressionSyntax(identifier, identifier.get_pos());
        }
    }
}
