#pragma once

#include "../Objects/object.h"
#include "../Syntax/Expressions/syntax-expressions.h"
#include "../Contexts/context.h"
#include "../Diagnostics/diagnostic.h"

// Refer to evaluator.cpp.
class Evaluator final
{
private:
    static Objects::Object* evaluate_literal(Contexts::Context& context, Syntax::LiteralExpressionSyntax* node);
    static Objects::Object* evaluate_unary(Contexts::Context& context, Syntax::UnaryExpressionSyntax* node);
    static Objects::Object* evaluate_binary(Contexts::Context& context, Syntax::BinaryExpressionSyntax* node);
    static Objects::Object* evaluate_sequence(Contexts::Context& context, Syntax::SequenceExpressionSyntax* node);
    static Objects::Object* evaluate_index(Contexts::Context& context, Syntax::IndexExpressionSyntax* node);
    static Objects::Object* evaluate_var_declare(Contexts::Context& context, Syntax::VarDeclareExpressionSyntax* node);
    static Objects::Object* evaluate_var_assign(Contexts::Context& context, Syntax::VarAssignExpressionSyntax* node);
    static Objects::Object* evaluate_var_access(Contexts::Context& context, Syntax::VarAccessExpressionSyntax* node);
    static Objects::Object* evaluate_while(Contexts::Context& context, Syntax::WhileExpressionSyntax* node);
    static Objects::Object* evaluate_for(Contexts::Context& context, Syntax::ForExpressionSyntax* node);
    static Objects::Object* evaluate_if(Contexts::Context& context, Syntax::IfExpressionSyntax* node);
    static Objects::Object* evaluate_return(Contexts::Context& context, Syntax::ReturnExpressionSyntax* node);
    static Objects::Object* evaluate_continue(Contexts::Context& context, Syntax::ContinueExpressionSyntax* node);
    static Objects::Object* evaluate_break(Contexts::Context& context, Syntax::BreakExpressionSyntax* node);
    static Objects::Object* evaluate_function_define(Contexts::Context& context, Syntax::FuncDefineExpressionSyntax* node);
    static Objects::Object* evaluate_function_call(Contexts::Context& context, Syntax::FuncCallExpressionSyntax* node);
public:
    static Objects::Object* evaluate(Contexts::Context& context, Syntax::SyntaxNode* node);
};