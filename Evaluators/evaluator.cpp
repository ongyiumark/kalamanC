#include "evaluator.h"
#include "builtin-functions.h"

#include <iostream>

using namespace Syntax;
using namespace Contexts;
using namespace Diagnostics;
using namespace Objects;
using namespace Evaluators;

// These help in evaluation. 
bool Evaluator::to_continue = false;
bool Evaluator::to_break = false;
Object* Evaluator::return_value = nullptr;

void Evaluator::clear()
{
    to_continue = false;
    to_break = false;
    return_value = nullptr;
}

// This signals the evaluator to stop propagating values.
bool Evaluator::should_return()
{
    return DiagnosticBag::size() || to_break || to_continue || return_value;
}

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
            return new None();    
        default:
            break;   
    }

    DiagnosticBag::report_unknown_syntax(kind_to_string(node->kind()), node->get_pos());
    return new None();
}

// Literally just return the object.
Object* Evaluator::evaluate_literal(Context& context, LiteralExpressionSyntax* node)
{
    return node->get_object()->copy();
}

// Unary operations.
Object* Evaluator::evaluate_unary(Context& context, UnaryExpressionSyntax* node)
{
    Object* result = nullptr;
    Object* operand = evaluate(context, node->get_operand());
    if (should_return()) 
    {
        delete operand;
        return new None();
    }

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

    if (result == nullptr || result->type() == Type::NONE)
    {
        DiagnosticBag::report_illegal_unary_operation(kind_to_string(node->get_op_token()->kind()),
            type_to_string(operand->type()), node->get_pos());
    } 

    delete operand;
    if (result != nullptr) return result;
    else return new None();
}

// Binary operations.
Object* Evaluator::evaluate_binary(Context& context, BinaryExpressionSyntax* node)
{
    Object* left = evaluate(context, node->get_left());

    if (should_return()) 
    {
        delete left;
        return new None();
    }

    Object* right = evaluate(context, node->get_right());
    
    if (should_return()) 
    {
        delete left;
        delete right;
        return new None();
    }

    Object* result = nullptr;
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

    if (result == nullptr || result->type() == Type::NONE)
    {
        DiagnosticBag::report_illegal_binary_operation(type_to_string(left->type()),
            kind_to_string(node->get_op_token()->kind()), type_to_string(right->type()), node->get_pos());
    } 

    delete left;
    delete right;
    if (result != nullptr) return result;
    else return new None();
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
            Object* obj = evaluate(context, node->get_node(i));
            elements.push_back(obj->copy());
            delete obj;
            if (should_return()) 
            {
                for (auto &o : elements)
                    delete o;
                return new None();
            }
        }
        List* list_res = new List(elements);
        return list_res;
    }

    // Sequence expression.
    int n = node->get_nodes_size();
    for (int i = 0; i < n; i++)
    {
        Object* obj = evaluate(context, node->get_node(i));
        delete obj;
        if (should_return()) return new None();
    }
    return new None();
}

// Index a list or a string.
Object* Evaluator::evaluate_index(Context& context, IndexExpressionSyntax* node)
{
    Object* left = evaluate(context, node->get_to_access());
    if (should_return()) 
    {
        delete left;
        return new None();    
    }
  
    Object* right = evaluate(context, node->get_indexer());
    if (should_return())
    {
        delete left;
        delete right;
        return new None();
    }

    Object* result = left->accessed_by(right);

    if (result->type() == Type::NONE)
    {
        DiagnosticBag::report_illegal_binary_operation(type_to_string(left->type()),
            kind_to_string(SyntaxKind::IndexExpression), type_to_string(right->type()), node->get_pos());
    } 

    delete left;
    delete right;
    return result;
}

// Declares a variable, assigns a default value.
Object* Evaluator::evaluate_var_declare(Context& context, VarDeclareExpressionSyntax* node)
{
    Type type = SyntaxFacts::get_keyword_type(node->get_var_keyword()->kind());

    Object* value = nullptr;
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
            value = new Double(0);
            break;
        }
        case Type::STRING:
        {
            value = new String("");
            break;
        }
        case Type::LIST:
        {
            std::vector<Object*> elems;
            value = new List(elems);
            break;
        }
        case Type::FUNCTION:
        {
            std::vector<std::string> arg_names;
            value = new Function("<uninitialized>", arg_names, nullptr);
            break;
        }
        default:
        {
            DiagnosticBag::report_unreachable_code("invalid type declaration", node->get_pos());
            return new None();
        }
    }

    Object* orig_value = context.get_symbol_table()->get_object(node->get_identifier()->get_text()).object;
    delete orig_value;
    context.get_symbol_table()->set_object(node->get_identifier()->get_text(), value);
    return value->copy();
}

// Assigns a value to a variable.
Object* Evaluator::evaluate_var_assign(Context& context, VarAssignExpressionSyntax* node)
{
    Object* value = evaluate(context, node->get_value());
    if (should_return()) 
    {
        delete value;
        return new None();
    }

    ObjectSymbol obj_sym = context.get_symbol_table()->get_object(node->get_identifier()->get_text());

    Object* orig_value = obj_sym.object;
    if (orig_value->type() != value->type())
    {
        DiagnosticBag::report_invalid_assign(type_to_string(value->type()), type_to_string(orig_value->type()),
            node->get_pos());
        delete value;
        return new None();
    }

    delete orig_value;
    obj_sym.symbol->set_object(node->get_identifier()->get_text(), value);
    return value->copy();
}

// Accesses a variable.
Object* Evaluator::evaluate_var_access(Context& context, VarAccessExpressionSyntax* node)
{
    Object* result = context.get_symbol_table()->get_object(node->get_identifier()->get_text()).object->copy();
    if (result->type() == Type::NONE)
    {
        DiagnosticBag::report_undeclared_identifier(node->get_identifier()->get_text(),
            node->get_identifier()->get_pos());
        return result;
    }

    return result;
}

// While statement.
Object* Evaluator::evaluate_while(Context& context, WhileExpressionSyntax* node)
{
    Context exec_ctx = Context("while-loop", &context, SymbolTable(context.get_symbol_table()));
    while(true)
    {
        Object* condition = evaluate(context, node->get_condition());
        if (should_return()) 
        {
            delete condition;
            return new None();
        }

        if (condition->type() != Type::BOOLEAN)
        {
            DiagnosticBag::report_unexpected_type(type_to_string(condition->type()), type_to_string(Type::BOOLEAN),
                node->get_condition()->get_pos());
            delete condition;
            return new None();
        }

        if (!((Boolean*)condition)->get_value())
        {
            delete condition;
            break;
        }
        
        Object* body_obj = evaluate(exec_ctx, node->get_body());
        delete body_obj;
        delete condition;

        if (should_return() && !(to_break || to_continue)) 
            return new None();
        
        if (to_break)
        {
            to_break = false;
            break;
        }

        if (to_continue)
        {
            to_continue = false;
            continue;
        }
    }
    return new None();
}

// Conditional staement.
Object* Evaluator::evaluate_if(Context& context, IfExpressionSyntax* node)
{
    int n = node->get_size();
    for (int i = 0; i < n; i++)
    {
        Object* condition = evaluate(context,node->get_condition(i));
        if (should_return()) 
        {
            delete condition;
            return new None();
        }

        if (condition->type() != Type::BOOLEAN)
        {
            DiagnosticBag::report_unexpected_type(type_to_string(condition->type()), type_to_string(Type::BOOLEAN),
                node->get_condition(i)->get_pos());
            delete condition;
            return new None();
        }

        if (((Boolean*)condition)->get_value())
        {
            Context exec_ctx = Context("if-statement", &context, SymbolTable(context.get_symbol_table()));
            Object* value = evaluate(exec_ctx, node->get_body(i));
            delete condition;
            if (should_return()) 
            {
                delete value;
                return new None();
            }
            return value;
        }
        delete condition;
    }

    if (node->get_else_body())
    {
        Context exec_ctx = Context("if-statement", &context, SymbolTable(context.get_symbol_table()));
        Object* value = evaluate(exec_ctx, node->get_else_body());
        if (should_return()) 
        {
            delete value;
            return new None();
        }
        return value;       
    }

    return new None();
}

// For statement.
Object* Evaluator::evaluate_for(Context& context, ForExpressionSyntax* node)
{
    Context exec_ctx = Context("for-loop", &context, SymbolTable(context.get_symbol_table()));
    Object* init_obj = evaluate(exec_ctx, node->get_init());
    delete init_obj;
    if (should_return()) return new None();

    while(true)
    {
        Object* condition = evaluate(exec_ctx, node->get_condition());
        if (should_return()) 
        {
            delete condition;
            return new None();
        }
        if (condition->type() != Type::BOOLEAN)
        {
            DiagnosticBag::report_unexpected_type(type_to_string(condition->type()), type_to_string(Type::BOOLEAN),
                node->get_condition()->get_pos());
            delete condition;
            return new None();
        }

        if (!((Boolean*)condition)->get_value())
        {
            delete condition;
            break;
        }

        Object* body_obj = evaluate(exec_ctx, node->get_body());
        delete body_obj;
        delete condition;

        if (should_return() && !(to_break || to_continue)) 
            return new None();

        if (to_break)
        {
            to_break = false;
            break;
        }

        if (to_continue) to_continue = false;

        Object* update_obj = evaluate(exec_ctx, node->get_update());
        delete update_obj;
        if (should_return()) return new None();
    }
    return new None();
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
    Object* orig_val = context.get_symbol_table()->get_object(node->get_identifier()->get_text()).object;
    delete orig_val;

    context.get_symbol_table()->set_object(node->get_identifier()->get_text(), val);
    return val->copy();
}

// Calls a function.
Object* Evaluator::evaluate_function_call(Context& context, FuncCallExpressionSyntax* node)
{
    Object* obj = context.get_symbol_table()->get_object(node->get_identifier()->get_text()).object->copy();
    if (obj->type() != Type::FUNCTION)
    {
        DiagnosticBag::report_unexpected_type(type_to_string(obj->type()), type_to_string(Type::FUNCTION),
            node->get_identifier()->get_pos());
        delete obj;
        return new None();
    }
    
    Function* func = (Function*)obj;

    // Generate context
    Context exec_ctx = Context(func->get_name(), &context, SymbolTable(context.get_symbol_table()));

    // Check arguments
    int n = func->get_argument_size();
    int m = node->get_arg_size();
    if (n != m)
    {
        Position arg_pos = Position();
        if (m > 0) 
        {
            Position first_arg = node->get_arg(0)->get_pos();
            Position last_arg = node->get_arg(m-1)->get_pos();
            arg_pos = Position(first_arg.ln, first_arg.col, first_arg.start, last_arg.end);
        }
        DiagnosticBag::report_illegal_arguments(m, n, func->get_name(), arg_pos);
        delete func;
        return new None();
    }
    
    // Evaluate arguments
    std::vector<Object*> args;
    for (int i = 0; i < n; i++)
    {
        args.push_back(evaluate(context, node->get_arg(i)));
        if (should_return()) 
        {
            for (auto &o : args)
                delete o;
            return new None();
        }

    }

    // Populate arguments
    for (int i = 0; i < n; i++)
        exec_ctx.get_symbol_table()->set_object(func->get_argument_name(i), args[i]);

    Object* result = nullptr;
    if (!func->is_built_in())
    {
        
        if (func->get_body() == nullptr)
        {
            for (auto &o : args)
                delete o;
            delete func;
            return new None();
        }

        // I had to cast here because I used a void*.
        Object* body_obj = evaluate(exec_ctx, (SyntaxNode*)(func->get_body()));
        delete body_obj;
        delete func;

        if (should_return() && !return_value) 
        {
            for (auto &o : args) delete o;
            return new None();
        }

        if (return_value)
        {
            result = return_value;
            return_value = nullptr;
        }
        
        if (result != nullptr) return result;
        else return new None();
    }

    std::string func_name = func->get_name();
    delete func;
    switch (SyntaxFacts::get_keyword_kind(func_name))
    {
        case SyntaxKind::PrintFunction:
            return BuiltInFunctions::BI_PRINT(exec_ctx);
        case SyntaxKind::InputFunction:
            return BuiltInFunctions::BI_INPUT(exec_ctx);
        case SyntaxKind::ToIntFunction:
            return BuiltInFunctions::BI_TO_INT(exec_ctx);
        default:
        {
            DiagnosticBag::report_unreachable_code("invalid builtin function", node->get_identifier()->get_pos());
            for (auto &o : args)
                delete o;
            return new None();
        }
    }

    for (auto &o : args)
        delete o;
    return new None();
}

// Return expression.
Object* Evaluator::evaluate_return(Context& context, ReturnExpressionSyntax* node)
{
    Object* result = nullptr;
    if (node->get_to_return())
    {
        result = evaluate(context, node->get_to_return());
        if (should_return()) 
        {
            delete result;
            return new None();
        }

        return_value = result;
        return new None();
    }

    return new None();
}

// Continue expression.
Object* Evaluator::evaluate_continue(Context& context, ContinueExpressionSyntax* node)
{
    to_continue = true;
    return new None();
}

// Break expression.
Object* Evaluator::evaluate_break(Context& context, BreakExpressionSyntax* node)
{
    to_break = true;
    return new None();
}
