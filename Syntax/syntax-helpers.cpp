#include "Expressions/syntax-expressions.h"
#include "../constants.h"
#include <iostream>

// Checks if a character is a digit. 
bool Syntax::is_digit(char c)
{
    return '0' <= c && c <= '9';
}

// Check if a character is a letter.
bool Syntax::is_letter(char c)
{
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

// Check if a character is a letter, digit, or underscore.
bool Syntax::is_valid_identifier(char c)
{
    return is_digit(c) || is_letter(c) || c == '_';
}

// Helper function to convert the kind to a string (used by parse tree printer).
std::string Syntax::kind_to_string(SyntaxKind kind)
{
    const char* s = "SyntaxKind::Unknown";
#define PROCESS_VAL(p) case(p): s = #p; break;
    switch(kind)
    {
        // Tokens
        PROCESS_VAL(SyntaxKind::BadToken);
        PROCESS_VAL(SyntaxKind::EndOfFileToken);
        PROCESS_VAL(SyntaxKind::WhitespaceToken);
        PROCESS_VAL(SyntaxKind::CommentToken);
        PROCESS_VAL(SyntaxKind::IntegerToken);
        PROCESS_VAL(SyntaxKind::DoubleToken);
        PROCESS_VAL(SyntaxKind::StringToken);
        PROCESS_VAL(SyntaxKind::PlusToken);
        PROCESS_VAL(SyntaxKind::MinusToken);
        PROCESS_VAL(SyntaxKind::StarToken);
        PROCESS_VAL(SyntaxKind::SlashToken);
        PROCESS_VAL(SyntaxKind::PowerToken);
        PROCESS_VAL(SyntaxKind::ModuloToken);
        PROCESS_VAL(SyntaxKind::PlusPlusToken);
        PROCESS_VAL(SyntaxKind::MinusMinusToken);
        PROCESS_VAL(SyntaxKind::EqualsToken);
        PROCESS_VAL(SyntaxKind::PlusEqualsToken);
        PROCESS_VAL(SyntaxKind::MinusEqualsToken);
        PROCESS_VAL(SyntaxKind::StarEqualsToken);
        PROCESS_VAL(SyntaxKind::SlashEqualsToken);
        PROCESS_VAL(SyntaxKind::ModuloEqualsToken);
        PROCESS_VAL(SyntaxKind::PowerEqualsToken);
        PROCESS_VAL(SyntaxKind::BangToken);
        PROCESS_VAL(SyntaxKind::DAmpersandToken);
        PROCESS_VAL(SyntaxKind::DPipeToken);
        PROCESS_VAL(SyntaxKind::DEqualsToken);
        PROCESS_VAL(SyntaxKind::BangEqualsToken);
        PROCESS_VAL(SyntaxKind::LessThanToken);
        PROCESS_VAL(SyntaxKind::LessEqualsToken);
        PROCESS_VAL(SyntaxKind::GreaterThanToken);
        PROCESS_VAL(SyntaxKind::GreaterEqualsToken);
        PROCESS_VAL(SyntaxKind::LParenToken);
        PROCESS_VAL(SyntaxKind::RParenToken);
        PROCESS_VAL(SyntaxKind::LSquareToken);
        PROCESS_VAL(SyntaxKind::RSquareToken);
        PROCESS_VAL(SyntaxKind::LCurlyToken);
        PROCESS_VAL(SyntaxKind::RCurlyToken);
        PROCESS_VAL(SyntaxKind::CommaToken);
        PROCESS_VAL(SyntaxKind::ColonToken);
        PROCESS_VAL(SyntaxKind::SemicolonToken);
        PROCESS_VAL(SyntaxKind::IdentifierToken);

        // Keywords
        PROCESS_VAL(SyntaxKind::TrueKeyword);
        PROCESS_VAL(SyntaxKind::FalseKeyword);
        PROCESS_VAL(SyntaxKind::BooleanKeyword);
        PROCESS_VAL(SyntaxKind::IntegerKeyword);
        PROCESS_VAL(SyntaxKind::DoubleKeyword);
        PROCESS_VAL(SyntaxKind::StringKeyword);
        PROCESS_VAL(SyntaxKind::ListKeyword);
        PROCESS_VAL(SyntaxKind::FunctionKeyword);
        PROCESS_VAL(SyntaxKind::DefineFunctionKeyword);
        PROCESS_VAL(SyntaxKind::IfKeyword);
        PROCESS_VAL(SyntaxKind::ElifKeyword);
        PROCESS_VAL(SyntaxKind::ElseKeyword);
        PROCESS_VAL(SyntaxKind::WhileKeyword);
        PROCESS_VAL(SyntaxKind::ForKeyword);
        PROCESS_VAL(SyntaxKind::InKeyword);
        PROCESS_VAL(SyntaxKind::AndKeyword);
        PROCESS_VAL(SyntaxKind::OrKeyword);
        PROCESS_VAL(SyntaxKind::XorKeyword);
        PROCESS_VAL(SyntaxKind::NotKeyword);
        PROCESS_VAL(SyntaxKind::ReturnKeyword);
        PROCESS_VAL(SyntaxKind::BreakKeyword);
        PROCESS_VAL(SyntaxKind::ContinueKeyword);
        
        // Expressions
        PROCESS_VAL(SyntaxKind::LiteralExpression);
        PROCESS_VAL(SyntaxKind::UnaryExpression);
        PROCESS_VAL(SyntaxKind::BinaryExpression);
        PROCESS_VAL(SyntaxKind::VarDeclareExpression);
        PROCESS_VAL(SyntaxKind::VarAssignExpression);
        PROCESS_VAL(SyntaxKind::VarAccessExpression);
        PROCESS_VAL(SyntaxKind::SequenceExpression);
        PROCESS_VAL(SyntaxKind::IfExpression);
        PROCESS_VAL(SyntaxKind::WhileExpression);
        PROCESS_VAL(SyntaxKind::ForExpression);
        PROCESS_VAL(SyntaxKind::FuncDefineExpression);
        PROCESS_VAL(SyntaxKind::FuncCallExpression);
        PROCESS_VAL(SyntaxKind::IndexExpression);
        PROCESS_VAL(SyntaxKind::ReturnExpression);
        PROCESS_VAL(SyntaxKind::BreakExpression);
        PROCESS_VAL(SyntaxKind::ContinueExpression);
        PROCESS_VAL(SyntaxKind::NoneExpression);  

        // Builtin Functions   
        PROCESS_VAL(SyntaxKind::PrintFunction);  
        PROCESS_VAL(SyntaxKind::InputFunction);  
        PROCESS_VAL(SyntaxKind::SplitFunction);
        PROCESS_VAL(SyntaxKind::SizeFunction);  
        PROCESS_VAL(SyntaxKind::TypeFunction);  
        PROCESS_VAL(SyntaxKind::ToBoolFunction);  
        PROCESS_VAL(SyntaxKind::ToIntFunction);  
        PROCESS_VAL(SyntaxKind::ToDoubleFunction);  
        PROCESS_VAL(SyntaxKind::ToStringFunction);  
        PROCESS_VAL(SyntaxKind::SetIndexFunction);
        PROCESS_VAL(SyntaxKind::PushFunction);
        PROCESS_VAL(SyntaxKind::PopFunction);
        PROCESS_VAL(SyntaxKind::RangeFunction);
        PROCESS_VAL(SyntaxKind::SortFunction);
        PROCESS_VAL(SyntaxKind::ReverseFunction);
        PROCESS_VAL(SyntaxKind::JoinFunction);
        PROCESS_VAL(SyntaxKind::AbsFunction);
        PROCESS_VAL(SyntaxKind::MinFunction);
        PROCESS_VAL(SyntaxKind::MaxFunction);
        PROCESS_VAL(SyntaxKind::SqrtFunction);
        PROCESS_VAL(SyntaxKind::PowModFunction);
        PROCESS_VAL(SyntaxKind::SetModFunction);
        PROCESS_VAL(SyntaxKind::ReadFileFunction);
        PROCESS_VAL(SyntaxKind::WriteFileFunction);
        PROCESS_VAL(SyntaxKind::ClockFunction);
        PROCESS_VAL(SyntaxKind::ExitFunction);
    }
#undef PROCESS_VAL
    return s;
}

std::string Syntax::kind_to_display_string(SyntaxKind kind)
{
    switch (kind)
    {
        case SyntaxKind::EndOfFileToken:    return "end of file";
        case SyntaxKind::IntegerToken:      return "int literal";
        case SyntaxKind::DoubleToken:       return "double literal";
        case SyntaxKind::StringToken:       return "string literal";
        case SyntaxKind::PlusToken:         return "+";
        case SyntaxKind::MinusToken:        return "-";
        case SyntaxKind::StarToken:         return "*";
        case SyntaxKind::SlashToken:        return "/";
        case SyntaxKind::PowerToken:        return "^";
        case SyntaxKind::ModuloToken:       return "%";
        case SyntaxKind::PlusPlusToken:     return "++";
        case SyntaxKind::MinusMinusToken:   return "--";
        case SyntaxKind::EqualsToken:       return "=";
        case SyntaxKind::PlusEqualsToken:   return "+=";
        case SyntaxKind::MinusEqualsToken:  return "-=";
        case SyntaxKind::StarEqualsToken:   return "*=";
        case SyntaxKind::SlashEqualsToken:  return "/=";
        case SyntaxKind::ModuloEqualsToken: return "%=";
        case SyntaxKind::PowerEqualsToken:  return "^=";
        case SyntaxKind::BangToken:         return "!";
        case SyntaxKind::DAmpersandToken:   return "&&";
        case SyntaxKind::DPipeToken:        return "||";
        case SyntaxKind::DEqualsToken:      return "==";
        case SyntaxKind::BangEqualsToken:   return "!=";
        case SyntaxKind::LessThanToken:     return "<";
        case SyntaxKind::LessEqualsToken:   return "<=";
        case SyntaxKind::GreaterThanToken:  return ">";
        case SyntaxKind::GreaterEqualsToken:return ">=";
        case SyntaxKind::LParenToken:       return "(";
        case SyntaxKind::RParenToken:       return ")";
        case SyntaxKind::LSquareToken:      return "[";
        case SyntaxKind::RSquareToken:      return "]";
        case SyntaxKind::LCurlyToken:       return "{";
        case SyntaxKind::RCurlyToken:       return "}";
        case SyntaxKind::CommaToken:        return ",";
        case SyntaxKind::ColonToken:        return ":";
        case SyntaxKind::SemicolonToken:    return ";";
        case SyntaxKind::IdentifierToken:   return "identifier";
        case SyntaxKind::AndKeyword:        return "and";
        case SyntaxKind::OrKeyword:         return "or";
        case SyntaxKind::XorKeyword:        return "xor";
        case SyntaxKind::NotKeyword:        return "not";
        case SyntaxKind::IndexExpression:   return "[]";
        default:                            return kind_to_string(kind);
    }
}

// Prints the parse tree in a pretty way. This is purely for debugging purposes.
void Syntax::pretty_print(SyntaxNode* node, std::string indent, bool is_last)
{
    if (!node) return;
    std::string marker = "|--";

    std::cout << indent << marker;
    indent += is_last ? "   " : "|  ";

    std::cout << kind_to_string(node->kind());
    
    // Keep synthetic tokens in function scope so their addresses remain valid during child traversal!
    SyntaxToken if_token(SyntaxKind::IfKeyword, Diagnostics::Position(), KT_IF);
    SyntaxToken elif_token(SyntaxKind::ElifKeyword, Diagnostics::Position(), KT_ELIF);
    SyntaxToken else_token(SyntaxKind::ElseKeyword, Diagnostics::Position(), KT_ELSE);

    std::vector<SyntaxNode*> children;
    switch(node->kind())
    {
        case SyntaxKind::LiteralExpression:
        {
            LiteralExpressionSyntax* t = (LiteralExpressionSyntax*)node;
            if (t->get_object())
                std::cout << " " << t->get_object()->to_string();
            break;
        }
        case SyntaxKind::UnaryExpression:
        {
            UnaryExpressionSyntax* t = (UnaryExpressionSyntax*)node;
            children = {t->get_op_token(), t->get_operand()};
            break;
        }
        case SyntaxKind::BinaryExpression:
        {
            BinaryExpressionSyntax* t = (BinaryExpressionSyntax*)node;
            children = {t->get_left(), t->get_op_token(), t->get_right()};
            break;
        }
        case SyntaxKind::ForExpression:
        {
            ForExpressionSyntax* t = (ForExpressionSyntax*)node;
            if (t->get_init()) children.push_back(t->get_init());
            if (t->get_condition()) children.push_back(t->get_condition());
            if (t->get_update()) children.push_back(t->get_update());
            if (t->get_body()) children.push_back(t->get_body());
            break;
        }
        case SyntaxKind::WhileExpression:
        {
            WhileExpressionSyntax* t = (WhileExpressionSyntax*)node;
            children = {t->get_condition(), t->get_body()};
            break;
        }
        case SyntaxKind::FuncCallExpression:
        {
            FuncCallExpressionSyntax* t = (FuncCallExpressionSyntax*)node;
            children = {t->get_identifier()};
            int m = t->get_arg_size();
            for (int i = 0; i < m; i++) children.push_back(t->get_arg(i));
            break;
        }
        case SyntaxKind::FuncDefineExpression:
        {
            FuncDefineExpressionSyntax* t = (FuncDefineExpressionSyntax*)node;
            children = {t->get_identifier()};
            int m = t->get_arg_size();
            for (int i = 0; i < m; i++) children.push_back(t->get_arg_name(i));
            children.push_back(t->get_body());
            break;
        }
        case SyntaxKind::IfExpression:
        {
            IfExpressionSyntax* t = (IfExpressionSyntax*)node;
            int m = t->get_size();
            for (int i = 0; i < m; i++)
            {
                if (i == 0) children.push_back(&if_token);
                else children.push_back(&elif_token);
                children.push_back(t->get_condition(i));
                children.push_back(t->get_body(i));
            }

            if (t->get_else_body())
            {
                children.push_back(&else_token);
                children.push_back(t->get_else_body());
            }
            break;
        }
        case SyntaxKind::IndexExpression:
        {
            IndexExpressionSyntax* t = (IndexExpressionSyntax*)node;
            if (t->get_to_access()) children.push_back(t->get_to_access());
            if (t->get_indexer()) children.push_back(t->get_indexer());
            if (t->get_slice_end()) children.push_back(t->get_slice_end());
            break;
        }
        case SyntaxKind::ReturnExpression:
        {
            ReturnExpressionSyntax* t = (ReturnExpressionSyntax*)node;
            if (t->get_to_return()) children = {t->get_to_return()};
            break;
        }
        case SyntaxKind::SequenceExpression:
        {
            SequenceExpressionSyntax* t = (SequenceExpressionSyntax*)node;
            children = t->get_nodes();
            break;
        }
        case SyntaxKind::VarAccessExpression:
        {
            VarAccessExpressionSyntax* t = (VarAccessExpressionSyntax*)node;
            children = {t->get_identifier()};
            break;
        }
        case SyntaxKind::VarAssignExpression:
        {
            VarAssignExpressionSyntax* t = (VarAssignExpressionSyntax*)node;
            if (t->get_target()) children.push_back(t->get_target());
            else children.push_back(t->get_identifier());
            if (t->get_value()) children.push_back(t->get_value());
            break;
        }
        case SyntaxKind::VarDeclareExpression:
        {
            VarDeclareExpressionSyntax* t = (VarDeclareExpressionSyntax*)node;
            children = {t->get_var_keyword(), t->get_identifier()};
            break;
        }
        case SyntaxKind::NoneExpression:
        case SyntaxKind::BreakExpression:
        case SyntaxKind::ContinueExpression:
            break;
        default:
        {
            std::cout << " ";
            SyntaxToken* token = (SyntaxToken*)node;
            std::cout << "'" << token->get_text() << "'";
            break;
        }
    }

    std::cout << std::endl;

    int n = children.size();
    for (int i = 0; i < n; i++)
        pretty_print(children[i], indent, i == n - 1);
}
