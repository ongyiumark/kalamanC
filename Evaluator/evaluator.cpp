#include "evaluator.h"
#include "builtin-functions.h"

#include <iostream>

using namespace Syntax;
using namespace Contexts;
using namespace Diagnostics;
using namespace Objects;

// Picks the right function and casts the node appropriately.
Object* Evaluator::evaluate(Context& context, SyntaxNode* node)
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
        case SyntaxKind::NoneExpression:  
            return Object::none_result;    
        default:
            break;   
    }

    DiagnosticBag::report_unknown_syntax(kind_to_string(node->kind()));
    return Object::none_result;
}

// Literally just return the object.
Object* Evaluator::evaluate_literal(Context& context, LiteralExpressionSyntax* node)
{
    return node->get_object();
}

// Unary operations.
Object* Evaluator::evaluate_unary(Context& context, UnaryExpressionSyntax* node)
{
    Object* result = Object::none_result;
    Object* operand = evaluate(context, node->get_operand());
    if (DiagnosticBag::should_return()) return result;

    switch (node->get_op_token()->kind())
    {
        case SyntaxKind::MinusToken:
        {
            Integer* minus_one = new Integer(-1);
            result = operand->multiplied_by(minus_one);
            delete minus_one;
            break;
        }
        case SyntaxKind::PlusToken:
        {
            Integer* one = new Integer(1);
            result = operand->multiplied_by(one);
            delete one;
            break;
        }
        case SyntaxKind::NotKeyword:
        case SyntaxKind::BangToken:
            result = operand->notted();    
            break;
        default:
            break;
    }

    if (result->type() == Type::NONE)
    {
        DiagnosticBag::report_illegal_unary_operation(kind_to_string(node->get_op_token()->kind()),
            type_to_string(operand->type()));
    }
    else DiagnosticBag::add_object(result);

    return result;
}

// Binary operations.
Object* Evaluator::evaluate_binary(Context& context, BinaryExpressionSyntax* node)
{
    Object* result = Object::none_result;
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
        default:
            break;
    }

    if (result->type() == Type::NONE)
    {
        DiagnosticBag::report_illegal_binary_operation(type_to_string(left->type()),
            kind_to_string(node->get_op_token()->kind()), type_to_string(right->type()));
    } else DiagnosticBag::add_object(result);
    return result;
}

// Sequence/List expressions.
Object* Evaluator::evaluate_sequence(Context& context, SequenceExpressionSyntax* node)
{
    // List expression.
    if (node->get_to_return())
    {   
        std::vector<Object*> elements;
        int n = node->get_nodes_size();
        for (int i = 0; i < n; i++)
        {
            elements.push_back(evaluate(context, node->get_node(i)));
            if (DiagnosticBag::should_return()) return Object::none_result;
        }
        List* list_res = new List(elements);
        DiagnosticBag::add_object(list_res);
        return list_res;
    }

    // Sequence expression.
    int n = node->get_nodes_size();
    for (int i = 0; i < n; i++)
    {
        evaluate(context, node->get_node(i));
        if (DiagnosticBag::should_return()) return Object::none_result;
    }
    return Object::none_result;
}

// Index a list or a string.
Object* Evaluator::evaluate_index(Context& context, IndexExpressionSyntax* node)
{
    Object* result = Object::none_result;
    Object* left = evaluate(context, node->get_to_access());
    if (DiagnosticBag::should_return()) return result;
  
    Object* right = evaluate(context, node->get_indexer());
    if (DiagnosticBag::should_return()) return result;

    result = left->accessed_by(right);

    if (result->type() == Type::NONE)
    {
        DiagnosticBag::report_illegal_binary_operation(type_to_string(left->type()),
            kind_to_string(SyntaxKind::IndexExpression), type_to_string(right->type()));
    } else DiagnosticBag::add_object(result);
    return result;
}

// Declares a variable, assigns a default value.
Object* Evaluator::evaluate_var_declare(Context& context, VarDeclareExpressionSyntax* node)
{
    Type type = SyntaxFacts::get_keyword_type(node->get_var_keyword()->kind());

    Object* value = Object::none_result;
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
            value = new Objects::Function("<uninitialized>", arg_names, nullptr);
            break;
        }
        default:
        {
            DiagnosticBag::report_unreachable_code("invalid type declaration");
            return Object::none_result;
        }
    }
    DiagnosticBag::add_object(value);
    context.get_symbol_table()->set_object(node->get_identifier()->get_text(), value);
    return value;
}

// Assigns a value to a variable.
Object* Evaluator::evaluate_var_assign(Context& context, VarAssignExpressionSyntax* node)
{
    Object* result = Object::none_result;
    Object* value = evaluate(context, node->get_value());
    if (DiagnosticBag::should_return()) return result;

    ObjectSymbol obj_sym = context.get_symbol_table()->get_object(node->get_identifier()->get_text());

    Object* orig_value = obj_sym.object;
    if (orig_value->type() != value->type())
    {
        DiagnosticBag::report_invalid_assign(type_to_string(value->type()), type_to_string(orig_value->type()));
        return result;
    }

    obj_sym.symbol->set_object(node->get_identifier()->get_text(), value);
    return value;
}

// Accesses a variable.
Object* Evaluator::evaluate_var_access(Context& context, VarAccessExpressionSyntax* node)
{
    Object* result = context.get_symbol_table()->get_object(node->get_identifier()->get_text()).object;
    if (result->type() == Type::NONE)
    {
        DiagnosticBag::report_undeclared_identifier(node->get_identifier()->get_text());
        return result;
    }

    return result;
}

// While statement.
Object* Evaluator::evaluate_while(Context& context, WhileExpressionSyntax* node)
{
    Object* result = Object::none_result;
    Context exec_ctx = Context("while-loop", &context, SymbolTable(context.get_symbol_table()));
    while(true)
    {
        Object* condition = evaluate(context, node->get_condition());
        if (DiagnosticBag::should_return()) return result;
        
        if (condition->type() != Type::BOOLEAN)
        {
            DiagnosticBag::report_unexpected_type(type_to_string(condition->type()), type_to_string(Type::BOOLEAN));
            return result;
        }
        if (!((Boolean*)condition)->get_value())
            break;
        
        evaluate(exec_ctx, node->get_body());
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

// Conditional staement.
Object* Evaluator::evaluate_if(Context& context, IfExpressionSyntax* node)
{
    Object* result = Object::none_result;
    int n = node->get_size();
    for (int i = 0; i < n; i++)
    {
        Object* condition = evaluate(context,node->get_condition(i));
        if (DiagnosticBag::should_return()) return result;
        if (condition->type() != Type::BOOLEAN)
        {
            DiagnosticBag::report_unexpected_type(type_to_string(condition->type()), type_to_string(Type::BOOLEAN));
            return result;
        }

        if (((Boolean*)condition)->get_value())
        {
            Context exec_ctx = Context("if-statement", &context, SymbolTable(context.get_symbol_table()));
            Object* value = evaluate(exec_ctx, node->get_body(i));
            if (DiagnosticBag::should_return()) return result;
            return value;
        }
    }

    if (node->get_else_body())
    {
        Context exec_ctx = Context("if-statement", &context, SymbolTable(context.get_symbol_table()));
        Object* value = evaluate(exec_ctx, node->get_else_body());
        if (DiagnosticBag::should_return()) return result;
        return value;       
    }

    return result;
}

// For statement.
Object* Evaluator::evaluate_for(Context& context, ForExpressionSyntax* node)
{
    Context exec_ctx = Context("for-loop", &context, SymbolTable(context.get_symbol_table()));
    Object* result = Object::none_result;
    evaluate(exec_ctx, node->get_init());
    if (DiagnosticBag::should_return()) return result;

    while(true)
    {
        Object* condition = evaluate(exec_ctx, node->get_condition());
        if (DiagnosticBag::should_return()) return result;
        if (condition->type() != Type::BOOLEAN)
        {
            DiagnosticBag::report_unexpected_type(type_to_string(condition->type()), type_to_string(Type::BOOLEAN));
            return result;
        }

        if (!((Boolean*)condition)->get_value())
            break;

        evaluate(exec_ctx, node->get_body());
        if (DiagnosticBag::should_return() && !(DiagnosticBag::to_break || DiagnosticBag::to_continue)) return result;

        if (DiagnosticBag::to_break)
        {
            DiagnosticBag::to_break = false;
            break;
        }

        if (DiagnosticBag::to_continue) DiagnosticBag::to_continue = false;

        evaluate(exec_ctx, node->get_update());
        if (DiagnosticBag::should_return()) return result;
    }
    return result;
}

// Defines a function.
Object* Evaluator::evaluate_function_define(Context& context, FuncDefineExpressionSyntax* node)
{
    std::string name = node->get_identifier()->get_text();
    std::vector<std::string> arg_names; 

    // Grabs the arg_names from the identifier tokens.
    int n = node->get_arg_size();
    for (int i = 0; i < n; i++)
        arg_names.push_back(node->get_arg_name(i)->get_text());
    
    Object* val = new Function(name, arg_names, node->get_body());
    DiagnosticBag::add_object(val);
    context.get_symbol_table()->set_object(node->get_identifier()->get_text(), val);
    return val;
}

// Calls a function.
Object* Evaluator::evaluate_function_call(Context& context, FuncCallExpressionSyntax* node)
{
    Object* result = Object::none_result;
    Object* obj = context.get_symbol_table()->get_object(node->get_identifier()->get_text()).object;
    if (obj->type() != Type::FUNCTION)
    {
        DiagnosticBag::report_unexpected_type(type_to_string(obj->type()), type_to_string(Type::FUNCTION));
        return result;
    }
    
    Function* func = (Function*)obj;

    // Generate context
    Context exec_ctx = Context(func->get_name(), &context, SymbolTable(context.get_symbol_table()));

    // Check arguments
    int n = func->get_argument_size();
    int m = node->get_arg_size();
    if (n != m)
    {
        DiagnosticBag::report_illegal_arguments(n, m);
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
        exec_ctx.get_symbol_table()->set_object(func->get_argument_name(i), args[i]);

    if (!func->is_built_in())
    {
        // I had to cast here because I used a void*.
        if (func->get_body() == nullptr)
        {
            DiagnosticBag::report_uninitialized_function();
            return result;
        }
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
        case SyntaxKind::ToIntFunction:
            return BuiltInFunctions::BI_TO_INT(exec_ctx);
        default:
        {
            DiagnosticBag::report_unreachable_code("invalid builtin function");
            return Object::none_result;
        }
    }

    return result;
}

// Return expression.
Object* Evaluator::evaluate_return(Context& context, ReturnExpressionSyntax* node)
{
    Object* result = Object::none_result;
    if (node->get_to_return())
    {
        evaluate(context, node->get_to_return());
        if (DiagnosticBag::should_return()) return result;

        DiagnosticBag::return_value = evaluate(context, node->get_to_return());
        return result;
    }

    return result;
}

// Continue expression.
Object* Evaluator::evaluate_continue(Context& context, ContinueExpressionSyntax* node)
{
    DiagnosticBag::to_continue = true;
    return Object::none_result;
}

// Break expression.
Object* Evaluator::evaluate_break(Context& context, BreakExpressionSyntax* node)
{
    DiagnosticBag::to_break = true;
    return Object::none_result;
}
