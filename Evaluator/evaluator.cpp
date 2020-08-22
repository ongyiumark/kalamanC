#include "evaluator.h"
#include "builtin-functions.h"

#include <iostream>

using namespace Syntax;
using namespace Contexts;
using namespace Diagnostics;
using namespace Objects;

DiagnosticBag* Evaluator::_diagnostics;

Object* Evaluator::evaluate(Context* context, const SyntaxNode* node)
{
    switch (node->kind())
    {
        case SyntaxKind::LiteralExpression:
            return evaluate_literal(context, (LiteralExpressionSyntax*)node);
        case SyntaxKind::UnaryExpression:
            return evaluate_unary(context, (UnaryExpressionSyntax*)node);
        case SyntaxKind::BinaryExpression:
            return evaluate_binary(context, (BinaryExpressionSyntax*)node);
        case SyntaxKind::IndexExpression:
            return evaluate_index(context, (IndexExpressionSyntax*)node);
        case SyntaxKind::SequenceExpression:
            return evaluate_sequence(context, (SequenceExpressionSyntax*)node);
        case SyntaxKind::VarDeclareExpression:  
            return evaluate_var_declare(context, (VarDeclareExpressionSyntax*)node);
        case SyntaxKind::VarAssignExpression:
            return evaluate_var_assign(context, (VarAssignExpressionSyntax*)node);
        case SyntaxKind::VarAccessExpression:  
            return evaluate_var_access(context, (VarAccessExpressionSyntax*)node);
        case SyntaxKind::ReturnExpression:  
            return evaluate_return(context, (ReturnExpressionSyntax*)node);
        case SyntaxKind::BreakExpression:  
            return evaluate_break(context, (BreakExpressionSyntax*)node);
        case SyntaxKind::ContinueExpression:  
            return evaluate_continue(context, (ContinueExpressionSyntax*)node);
        case SyntaxKind::WhileExpression:
            return evaluate_while(context, (WhileExpressionSyntax*)node);
        case SyntaxKind::IfExpression:  
            return evaluate_if(context, (IfExpressionSyntax*)node);
        case SyntaxKind::ForExpression:
            return evaluate_for(context, (ForExpressionSyntax*)node);
        case SyntaxKind::FuncDefineExpression:
            return evaluate_function_define(context, (FuncDefineExpressionSyntax*)node);
        case SyntaxKind::FuncCallExpression:  
            return evaluate_function_call(context, (FuncCallExpressionSyntax*)node);        
    }

    _diagnostics->report_unknown_syntax(kind_to_string(node->kind()));
    return new None();
}

Object* Evaluator::evaluate_literal(Context* context, const LiteralExpressionSyntax* node)
{
    return node->get_object();
}

Object* Evaluator::evaluate_unary(Context* context, const UnaryExpressionSyntax* node)
{
    Object* result = new None();
    Object* operand = evaluate(context, node->get_operand());
    if (DiagnosticBag::should_return()) return result;
  
    switch (node->get_op_token()->kind())
    {
        case SyntaxKind::MinusToken:
            result = operand->multiplied_by(new Integer(-1));
            break;
        case SyntaxKind::PlusToken:
            result = operand->multiplied_by(new Integer(1));
            break;
        case SyntaxKind::NotKeyword:
        case SyntaxKind::BangToken:
            result = operand->notted();    
            break;
    }

    if (result->type() == Type::NONE)
    {
        _diagnostics->report_illegal_unary_operation(kind_to_string(node->get_op_token()->kind()),
            type_to_string(operand->type()));
    }
    return result;
}

Object* Evaluator::evaluate_binary(Context* context, const BinaryExpressionSyntax* node)
{
    Object* result = new None();
    Object* left = evaluate(context, node->get_left());
    if (DiagnosticBag::should_return()) return result;
  
    Object* right = evaluate(context, node->get_right());
    if (DiagnosticBag::should_return()) return result;

    switch (node->get_op_token()->kind())
    {
        case SyntaxKind::PlusToken:
            result = left->added_by(right);
            break;
        case SyntaxKind::MinusToken:
            result = left->subtracted_by(right);
            break;
        case SyntaxKind::StarToken:
            result = left->multiplied_by(right);
            break;
        case SyntaxKind::SlashToken:
            result = left->divided_by(right);
            break;
        case SyntaxKind::ModuloToken:
            result = left->modded_by(right);
            break;
        case SyntaxKind::PowerToken:
            result = left->powered_by(right);
            break;
        case SyntaxKind::AndKeyword:
        case SyntaxKind::DAmpersandToken:
            result = left->and_with(right);
            break;
        case SyntaxKind::OrKeyword:
        case SyntaxKind::DPipeToken:
            result = left->or_with(right);
            break;
        case SyntaxKind::LessThanToken:
            result = left->less_than(right);
            break;
        case SyntaxKind::GreaterThanToken:
            result = left->greater_than(right);
            break;
        case SyntaxKind::LessEqualsToken:
            result = left->less_equals(right);
            break;
        case SyntaxKind::GreaterEqualsToken:
            result = left->greater_equals(right);
            break;
        case SyntaxKind::DEqualsToken:
            result = left->equals(right);
            break;
        case SyntaxKind::BangEqualsToken:
            result = left->not_equals(right);
            break;
    }

    if (result->type() == Type::NONE)
    {
        _diagnostics->report_illegal_binary_operation(type_to_string(left->type()),
            kind_to_string(node->get_op_token()->kind()), type_to_string(right->type()));
    }
    return result;
}

Object* Evaluator::evaluate_sequence(Context* context, const SequenceExpressionSyntax* node)
{
    std::vector<Object*> elements;
    int n = node->get_nodes_size();
    for (int i = 0; i < n; i++)
    {
        elements.push_back(evaluate(context, node->get_node(i)));
        if (DiagnosticBag::should_return()) return new None();
    }

    return new List(elements);
}

Object* Evaluator::evaluate_index(Context* context, const IndexExpressionSyntax* node)
{
    Object* result = new None();
    Object* left = evaluate(context, node->get_to_access());
    if (DiagnosticBag::should_return()) return result;
  
    Object* right = evaluate(context, node->get_indexer());
    if (DiagnosticBag::should_return()) return result;

    result = left->accessed_by(right);

    if (result->type() == Type::NONE)
    {
        _diagnostics->report_illegal_binary_operation(type_to_string(left->type()),
            kind_to_string(SyntaxKind::IndexExpression), type_to_string(right->type()));
    }
    return result;
}

Object* Evaluator::evaluate_var_declare(Context* context, const VarDeclareExpressionSyntax* node)
{
    Type type = SyntaxFacts::get_keyword_type(node->get_var_keyword()->kind());

    Object* value = new None();
    switch(type)
    {
        case Type::BOOLEAN:
        {
            value = new Boolean(false);
            break;
        }
        case Type::INTEGER:
        {
            value = new Integer(0);
            break;
        }
        case Type::DOUBLE:
        {
            value = new Objects::Double(0);
            break;
        }
        case Objects::Type::STRING:
        {
            value = new Objects::String("");
            break;
        }
        case Objects::Type::LIST:
        {
            std::vector<Objects::Object*> elems;
            value = new Objects::List(elems);
            break;
        }
        case Objects::Type::FUNCTION:
        {
            std::vector<std::string> arg_names;
            value = new Objects::Function("<uninitialized>", arg_names, new NoneExpressionSyntax());
            break;
        }
    }

    context->get_symbol_table()->set_object(node->get_identifier()->get_text(), value);
    return value;
}

Object* Evaluator::evaluate_var_assign(Context* context, const VarAssignExpressionSyntax* node)
{
    Object* result = new None();
    Object* value = evaluate(context, node->get_value());
    if (DiagnosticBag::should_return()) return result;

    Object* orig_value = context->get_symbol_table()->get_object(node->get_identifier()->get_text());
    if (orig_value->type() != value->type())
    {
        _diagnostics->report_invalid_assign(type_to_string(value->type()), type_to_string(orig_value->type()));
        return result;
    }

    context->get_symbol_table()->set_object(node->get_identifier()->get_text(), value);
    return value;
}

Object* Evaluator::evaluate_var_access(Context* context, const VarAccessExpressionSyntax* node)
{
    Object* result = context->get_symbol_table()->get_object(node->get_identifier()->get_text());
    if (result->type() == Type::NONE)
    {
        _diagnostics->report_undeclared_identifier(node->get_identifier()->get_text());
        return result;
    }

    return result;
}

Object* Evaluator::evaluate_while(Context* context, const WhileExpressionSyntax* node)
{
    Object* result = new None();
    while(true)
    {
        Object* condition = evaluate(context, node->get_condition());
        if (DiagnosticBag::should_return()) return result;
        
        if (condition->type() != Type::BOOLEAN)
        {
            _diagnostics->report_unexpected_type(type_to_string(condition->type()), type_to_string(Type::BOOLEAN));
            return result;
        }
        if (!((Boolean*)condition)->get_value())
            break;
        
        Object* body_val = evaluate(context, node->get_body());
        if (DiagnosticBag::should_return() && !(DiagnosticBag::to_break || DiagnosticBag::to_continue)) return result;

        if (DiagnosticBag::to_break)
        {
            DiagnosticBag::to_break = false;
            break;
        }

        if (DiagnosticBag::to_continue)
        {
            DiagnosticBag::to_continue = false;
            continue;
        }
    }
    return result;
}

Object* Evaluator::evaluate_if(Context* context, const IfExpressionSyntax* node)
{
    Object* result = new None();
    int n = node->get_size();
    for (int i = 0; i < n; i++)
    {
        Object* condition = evaluate(context,node->get_condition(i));
        if (DiagnosticBag::should_return()) return result;
        if (condition->type() != Type::BOOLEAN)
        {
            _diagnostics->report_unexpected_type(type_to_string(condition->type()), type_to_string(Type::BOOLEAN));
            return result;
        }

        if (((Boolean*)condition)->get_value())
        {
            Object* value = evaluate(context, node->get_body(i));
            if (DiagnosticBag::should_return()) return result;
            return value;
        }
    }

    if (node->get_else_body())
    {
        Object* value = evaluate(context, node->get_else_body());
        if (DiagnosticBag::should_return()) return result;
        return value;       
    }

    return result;
}

Object* Evaluator::evaluate_for(Context* context, const ForExpressionSyntax* node)
{
    Object* result = new None();
    evaluate(context, node->get_init());
    if (DiagnosticBag::should_return()) return result;

    while(true)
    {
        Object* condition = evaluate(context, node->get_condition());
        if (DiagnosticBag::should_return()) return result;
        if (condition->type() != Type::BOOLEAN)
        {
            _diagnostics->report_unexpected_type(type_to_string(condition->type()), type_to_string(Type::BOOLEAN));
            return result;
        }

        if (!((Boolean*)condition)->get_value())
            break;
        evaluate(context, node->get_body());
        if (DiagnosticBag::should_return() && !(DiagnosticBag::to_break || DiagnosticBag::to_continue)) return result;

        if (DiagnosticBag::to_break)
        {
            DiagnosticBag::to_break = false;
            break;
        }

        if (DiagnosticBag::to_continue) DiagnosticBag::to_continue = false;

        evaluate(context, node->get_update());
        if (DiagnosticBag::should_return()) return result;
    }
    return result;
}

Object* Evaluator::evaluate_function_define(Context* context, const FuncDefineExpressionSyntax* node)
{
    std::string name = node->get_identifier()->get_text();
    std::vector<std::string> arg_names; 
    int n = node->get_arg_size();
    for (int i = 0; i < n; i++)
        arg_names.push_back(node->get_arg_name(i)->get_text());
    
    Object* val = new Function(name, arg_names, node->get_body());
    context->get_symbol_table()->set_object(node->get_identifier()->get_text(), val);
    return val;
}

Object* Evaluator::evaluate_function_call(Context* context, const FuncCallExpressionSyntax* node)
{
    Object* result = new None();
    Object* obj = context->get_symbol_table()->get_object(node->get_identifier()->get_text());
    if (obj->type() != Type::FUNCTION)
    {
        _diagnostics->report_unexpected_type(type_to_string(obj->type()), type_to_string(Type::FUNCTION));
        return result;
    }
    Function* func = (Function*)obj;

    // Generate context
    Context* exec_ctx = new Context(func->get_name(), context, new SymbolTable(context->get_symbol_table()));

    // Check arguments
    int n = func->get_argument_size();
    int m = node->get_arg_size();
    if (n != m)
    {
        _diagnostics->report_illegal_arguments(n, m);
        return result;
    }
    
    // Evaluate arguments
    std::vector<Object*> args;
    for (int i = 0; i < n; i++)
    {
        args.push_back(evaluate(context, node->get_arg(i)));
        if (DiagnosticBag::should_return()) return result;
    }

    // Populate arguments
    for (int i = 0; i < n; i++)
        exec_ctx->get_symbol_table()->set_object(func->get_argument_name(i), args[i]);

    if (!func->is_built_in())
    {
        evaluate(exec_ctx, (SyntaxNode*)func->get_body());
        if (DiagnosticBag::should_return() && !DiagnosticBag::return_value) return result;
        if (DiagnosticBag::return_value)
        {
            result = DiagnosticBag::return_value;
            DiagnosticBag::return_value = NULL;
        }
        return result;
    }

    switch (SyntaxFacts::get_keyword_kind(func->get_name()))
    {
        case SyntaxKind::PrintFunction:
            return BuiltInFunctions::BI_PRINT(exec_ctx);
        case SyntaxKind::InputFunction:
            return BuiltInFunctions::BI_INPUT(exec_ctx);
    }

    return result;
}


Object* Evaluator::evaluate_return(Context* context, const ReturnExpressionSyntax* node)
{
    Object* result = new None();
    if (node->get_to_return())
    {
        Object* value = evaluate(context, node->get_to_return());
        if (DiagnosticBag::should_return()) return result;

        DiagnosticBag::return_value = evaluate(context, node->get_to_return());
        return result;
    }

    return result;
}

Object* Evaluator::evaluate_continue(Context* context, const ContinueExpressionSyntax* node)
{
    DiagnosticBag::to_continue = true;
    return new None();
}

Object* Evaluator::evaluate_break(Context* context, const BreakExpressionSyntax* node)
{
    DiagnosticBag::to_break = true;
    return new None();
}
