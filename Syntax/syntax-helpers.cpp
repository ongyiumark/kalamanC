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
    return ('a' <= c && c <= 'z')||('A' <= c && c <='Z');
}

// Check if a character is a letter, digit, or underscore.
bool Syntax::is_valid_identifier(char c)
{
    return is_digit(c) || is_letter(c) || c == '_';
}

// Helper function to convert the kind to a string.
std::string Syntax::kind_to_string(SyntaxKind kind)
{
    const char* s = 0;
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
        PROCESS_VAL(SyntaxKind::EqualsToken);
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
        PROCESS_VAL(SyntaxKind::SemicolonToken);
        PROCESS_VAL(SyntaxKind::IdentifierToken);

        //Keywords
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
        PROCESS_VAL(SyntaxKind::SizeFunction);  
        PROCESS_VAL(SyntaxKind::TypeFunction);  
        PROCESS_VAL(SyntaxKind::ToBoolFunction);  
        PROCESS_VAL(SyntaxKind::ToIntFunction);  
        PROCESS_VAL(SyntaxKind::ToDoubleFunction);  
        PROCESS_VAL(SyntaxKind::ToStringFunction);  
    }
#undef PROCESS_VAL
    return s;
}

// Prints the parse tree in a pretty way. This is purely for debugging purposes.
void Syntax::pretty_print(SyntaxNode* node, std::string indent, bool is_last)
{
    std::string marker = is_last ? "|--" : "|--";

    std::cout << indent << marker;
    indent += is_last ? "   " : "|  ";

    std::cout << kind_to_string(node->kind());
    
    std::vector<SyntaxNode*> children;
    switch(node->kind())
    {
        case SyntaxKind::LiteralExpression:
        {
            LiteralExpressionSyntax* t = (LiteralExpressionSyntax*)node;
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
            children = {t->get_init(), t->get_condition(), t->get_update(), t->get_body()};
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
            SyntaxToken if_token = SyntaxToken(SyntaxKind::IfKeyword, Diagnostics::Position(), KT_IF);
            SyntaxToken elif_token = SyntaxToken(SyntaxKind::ElifKeyword, Diagnostics::Position(), KT_ELIF);
            SyntaxToken else_token = SyntaxToken(SyntaxKind::ElseKeyword, Diagnostics::Position(), KT_ELSE);
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
            children = {t->get_to_access(), t->get_indexer()};
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
            children = {t->get_identifier(), t->get_value()};
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
        pretty_print(children[i], indent, i == n-1);
}