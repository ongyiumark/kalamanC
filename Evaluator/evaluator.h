#pragma once

#include "../Objects/object.h"
#include "../Syntax/Expressions/syntax-expressions.h"
#include "../Contexts/context.h"
#include "../Diagnostics/diagnostic.h"

class Evaluator final
{
private:
    static Diagnostics::DiagnosticBag* _diagnostics;
    static Objects::Object* evaluate_literal(Contexts::Context* context, const Syntax::LiteralExpressionSyntax* node);
    static Objects::Object* evaluate_unary(Contexts::Context* context, const Syntax::UnaryExpressionSyntax* node);
    static Objects::Object* evaluate_binary(Contexts::Context* context, const Syntax::BinaryExpressionSyntax* node);
    static Objects::Object* evaluate_sequence(Contexts::Context* context, const Syntax::SequenceExpressionSyntax* node);
    static Objects::Object* evaluate_index(Contexts::Context* context, const Syntax::IndexExpressionSyntax* node);
    static Objects::Object* evaluate_var_declare(Contexts::Context* context, const Syntax::VarDeclareExpressionSyntax* node);
    static Objects::Object* evaluate_var_assign(Contexts::Context* context, const Syntax::VarAssignExpressionSyntax* node);
    static Objects::Object* evaluate_var_access(Contexts::Context* context, const Syntax::VarAccessExpressionSyntax* node);
    static Objects::Object* evaluate_while(Contexts::Context* context, const Syntax::WhileExpressionSyntax* node);
    static Objects::Object* evaluate_for(Contexts::Context* context, const Syntax::ForExpressionSyntax* node);
    static Objects::Object* evaluate_if(Contexts::Context* context, const Syntax::IfExpressionSyntax* node);
    static Objects::Object* evaluate_return(Contexts::Context* context, const Syntax::ReturnExpressionSyntax* node);
    static Objects::Object* evaluate_continue(Contexts::Context* context, const Syntax::ContinueExpressionSyntax* node);
    static Objects::Object* evaluate_break(Contexts::Context* context, const Syntax::BreakExpressionSyntax* node);
    static Objects::Object* evaluate_function_define(Contexts::Context* context, const Syntax::FuncDefineExpressionSyntax* node);
    static Objects::Object* evaluate_function_call(Contexts::Context* context, const Syntax::FuncCallExpressionSyntax* node);
public:
    static Objects::Object* evaluate(Contexts::Context* context, const Syntax::SyntaxNode* node);
};