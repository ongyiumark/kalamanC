#include "syntax.h"
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
    
    switch(node->kind())
    {
        case SyntaxKind::IntegerToken:
        case SyntaxKind::StringToken:
        case SyntaxKind::DoubleToken:
        {
            std::cout << " ";
            SyntaxToken* token = (SyntaxToken*)node;
            if (token->get_object()) std::cout << token->get_object()->to_string();
            break;
        }
        case SyntaxKind::IdentifierToken:
        {
            std::cout << " ";
            SyntaxToken* token = (SyntaxToken*)node;
            std::cout << token->get_text();
            break;
        }
    }

    std::cout << std::endl;

    SyntaxNode* last_child = NULL;
    int n = node->children_size();
    if (n) last_child = node->child(n-1);

    for (int i = 0; i < n; i++)
        pretty_print(node->child(i), indent, last_child == node->child(i));
}