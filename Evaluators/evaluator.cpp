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
    delete return_value;
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

    DiagnosticBag::report_unknown_syntax(kind_to_display_string(node->kind()), node->get_pos());
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
            Integer minus_one(-1);
            result = operand->multiplied_by(&minus_one);
            break;
        }
        case SyntaxKind::PlusToken:
        {
            Integer one(1);
            result = operand->multiplied_by(&one);
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
        DiagnosticBag::report_illegal_unary_operation(kind_to_display_string(node->get_op_token()->kind()),
            type_to_string(operand->type()), node->get_pos());
    } 

    delete operand;
    if (result != nullptr) return result;
    else return new None();
}

// Binary operations (with short-circuit evaluation for && and ||).
Object* Evaluator::evaluate_binary(Context& context, BinaryExpressionSyntax* node)
{
    Object* left = evaluate(context, node->get_left());

    if (should_return()) 
    {
        delete left;
        return new None();
    }

    SyntaxKind op_kind = node->get_op_token()->kind();

    // Short-circuit logical AND and OR when left is a boolean
    if ((op_kind == SyntaxKind::AndKeyword || op_kind == SyntaxKind::DAmpersandToken) && left->type() == Type::BOOLEAN)
    {
        if (!((Boolean*)left)->get_value())
            return left; // false && anything -> false (without evaluating right!)
    }
    else if ((op_kind == SyntaxKind::OrKeyword || op_kind == SyntaxKind::DPipeToken) && left->type() == Type::BOOLEAN)
    {
        if (((Boolean*)left)->get_value())
            return left; // true || anything -> true (without evaluating right!)
    }

    Object* right = evaluate(context, node->get_right());
    
    if (should_return()) 
    {
        delete left;
        delete right;
        return new None();
    }

    Object* result = nullptr;
    switch (op_kind)
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
        case SyntaxKind::XorKeyword:
            result = left->xor_with(right);
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
        if ((op_kind == SyntaxKind::SlashToken || op_kind == SyntaxKind::ModuloToken) &&
            ((right->type() == Type::INTEGER && ((Integer*)right)->get_value() == 0) ||
             (right->type() == Type::DOUBLE && ((Double*)right)->get_value() == 0)))
        {
            DiagnosticBag::report_runtime_error("division by zero", node->get_pos());
        }
        else
        {
            DiagnosticBag::report_illegal_binary_operation(type_to_string(left->type()),
                kind_to_display_string(op_kind), type_to_string(right->type()), node->get_pos());
        }
    } 

    delete left;
    delete right;
    if (result != nullptr) return result;
    else return new None();
}

// Sequence/List expressions.
Object* Evaluator::evaluate_sequence(Context& context, SequenceExpressionSyntax* node)
{
    // List expression ([a, b, c]).
    if (node->get_to_return())
    {   
        int n = node->get_nodes_size();
        std::vector<Object*> elements;
        elements.reserve(n);
        for (int i = 0; i < n; i++)
        {
            Object* obj = evaluate(context, node->get_node(i));
            elements.push_back(obj);
            if (should_return()) 
            {
                for (auto &o : elements)
                    delete o;
                return new None();
            }
        }
        return new List(elements);
    }

    // Sequence of statements (returns the result of the last executed statement).
    int n = node->get_nodes_size();
    Object* last_obj = nullptr;
    for (int i = 0; i < n; i++)
    {
        delete last_obj;
        last_obj = evaluate(context, node->get_node(i));
        if (should_return())
        {
            delete last_obj;
            return new None();
        }
    }
    if (last_obj != nullptr) return last_obj;
    return new None();
}

// Index or slice a list or a string.
Object* Evaluator::evaluate_index(Context& context, IndexExpressionSyntax* node)
{
    Object* left = evaluate(context, node->get_to_access());
    if (should_return()) 
    {
        delete left;
        return new None();    
    }

    if (node->is_slice())
    {
        bool has_start = (node->get_indexer() != nullptr);
        bool has_end = (node->get_slice_end() != nullptr);
        long long start_val = 0;
        long long end_val = 0;

        if (has_start)
        {
            Object* start_obj = evaluate(context, node->get_indexer());
            if (should_return())
            {
                delete left;
                delete start_obj;
                return new None();
            }
            if (start_obj->type() != Type::INTEGER)
            {
                DiagnosticBag::report_unexpected_type(type_to_string(start_obj->type()), type_to_string(Type::INTEGER),
                    node->get_indexer()->get_pos());
                delete left;
                delete start_obj;
                return new None();
            }
            start_val = ((Integer*)start_obj)->get_value();
            delete start_obj;
        }

        if (has_end)
        {
            Object* end_obj = evaluate(context, node->get_slice_end());
            if (should_return())
            {
                delete left;
                delete end_obj;
                return new None();
            }
            if (end_obj->type() != Type::INTEGER)
            {
                DiagnosticBag::report_unexpected_type(type_to_string(end_obj->type()), type_to_string(Type::INTEGER),
                    node->get_slice_end()->get_pos());
                delete left;
                delete end_obj;
                return new None();
            }
            end_val = ((Integer*)end_obj)->get_value();
            delete end_obj;
        }

        Object* result = nullptr;
        if (left->type() == Type::LIST)
            result = ((List*)left)->slice(start_val, end_val, has_start, has_end);
        else if (left->type() == Type::STRING)
            result = ((String*)left)->slice(start_val, end_val, has_start, has_end);
        else
        {
            DiagnosticBag::report_illegal_binary_operation(type_to_string(left->type()),
                "[:]", type_to_string(Type::INTEGER), node->get_pos());
            result = new None();
        }
        delete left;
        return result;
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
        if ((left->type() == Type::LIST || left->type() == Type::STRING) && right->type() == Type::INTEGER)
        {
            DiagnosticBag::report_runtime_error("index out of range", node->get_pos());
        }
        else
        {
            DiagnosticBag::report_illegal_binary_operation(type_to_string(left->type()),
                kind_to_display_string(SyntaxKind::IndexExpression), type_to_string(right->type()), node->get_pos());
        }
    } 

    delete left;
    delete right;
    return result;
}

// Declares a variable in the current (local) symbol table and assigns a default value.
Object* Evaluator::evaluate_var_declare(Context& context, VarDeclareExpressionSyntax* node)
{
    Type type = SyntaxFacts::get_keyword_type(node->get_var_keyword()->kind());

    Object* value = nullptr;
    switch(type)
    {
        case Type::BOOLEAN:
            value = new Boolean(false);
            break;
        case Type::INTEGER:
            value = new Integer(0);
            break;
        case Type::DOUBLE:
            value = new Double(0);
            break;
        case Type::STRING:
            value = new String("");
            break;
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

    context.get_symbol_table()->set_object(node->get_identifier()->get_text(), value);
    return value->copy();
}

static Object* apply_compound_op(Object* orig, SyntaxKind op_kind, Object* rhs, Position pos)
{
    Object* result = nullptr;
    switch (op_kind)
    {
        case SyntaxKind::PlusPlusToken:
        {
            Integer one(1);
            result = orig->added_by(&one);
            break;
        }
        case SyntaxKind::MinusMinusToken:
        {
            Integer one(1);
            result = orig->subtracted_by(&one);
            break;
        }
        case SyntaxKind::PlusEqualsToken:
            result = orig->added_by(rhs);
            break;
        case SyntaxKind::MinusEqualsToken:
            result = orig->subtracted_by(rhs);
            break;
        case SyntaxKind::StarEqualsToken:
            result = orig->multiplied_by(rhs);
            break;
        case SyntaxKind::SlashEqualsToken:
            result = orig->divided_by(rhs);
            break;
        case SyntaxKind::ModuloEqualsToken:
            result = orig->modded_by(rhs);
            break;
        case SyntaxKind::PowerEqualsToken:
            result = orig->powered_by(rhs);
            break;
        default:
            break;
    }

    if (result == nullptr || result->type() == Type::NONE)
    {
        if (rhs == nullptr)
        {
            DiagnosticBag::report_illegal_unary_operation(kind_to_display_string(op_kind),
                type_to_string(orig->type()), pos);
        }
        else if ((op_kind == SyntaxKind::SlashEqualsToken || op_kind == SyntaxKind::ModuloEqualsToken) &&
                 ((rhs->type() == Type::INTEGER && ((Integer*)rhs)->get_value() == 0) ||
                  (rhs->type() == Type::DOUBLE && ((Double*)rhs)->get_value() == 0)))
        {
            DiagnosticBag::report_runtime_error("division by zero", pos);
        }
        else
        {
            DiagnosticBag::report_illegal_binary_operation(type_to_string(orig->type()),
                kind_to_display_string(op_kind), type_to_string(rhs->type()), pos);
        }
        delete result;
        return nullptr;
    }
    return result;
}

// Assigns or compound-assigns a value to a variable or list element.
Object* Evaluator::evaluate_var_assign(Context& context, VarAssignExpressionSyntax* node)
{
    SyntaxKind op_kind = node->get_op_token()->kind();

    // Case 1: Indexed assignment (e.g. a[i] = val, a[i][j] += val, a[i]++)
    if (node->get_target() != nullptr && node->get_target()->kind() == SyntaxKind::IndexExpression)
    {
        std::vector<IndexExpressionSyntax*> index_chain;
        SyntaxNode* curr = node->get_target();
        while (curr->kind() == SyntaxKind::IndexExpression)
        {
            IndexExpressionSyntax* idx_node = (IndexExpressionSyntax*)curr;
            if (idx_node->is_slice())
            {
                DiagnosticBag::report_runtime_error("cannot assign to a slice", idx_node->get_pos());
                return new None();
            }
            index_chain.push_back(idx_node);
            curr = idx_node->get_to_access();
        }

        if (curr->kind() != SyntaxKind::VarAccessExpression)
        {
            DiagnosticBag::report_runtime_error("invalid target for indexed assignment", node->get_pos());
            return new None();
        }

        VarAccessExpressionSyntax* base_var = (VarAccessExpressionSyntax*)curr;
        ObjectSymbol obj_sym = context.get_symbol_table()->get_object(base_var->get_identifier()->get_text());
        if (obj_sym.object == nullptr)
        {
            DiagnosticBag::report_undeclared_identifier(base_var->get_identifier()->get_text(),
                base_var->get_identifier()->get_pos());
            return new None();
        }

        Object* container = obj_sym.object;
        for (int k = (int)index_chain.size() - 1; k >= 0; k--)
        {
            IndexExpressionSyntax* step = index_chain[k];
            Object* idx_obj = evaluate(context, step->get_indexer());
            if (should_return())
            {
                delete idx_obj;
                return new None();
            }
            if (container->type() != Type::LIST || idx_obj->type() != Type::INTEGER)
            {
                DiagnosticBag::report_illegal_binary_operation(type_to_string(container->type()),
                    "[]", type_to_string(idx_obj->type()), step->get_pos());
                delete idx_obj;
                return new None();
            }

            List* list = (List*)container;
            long long idx = ((Integer*)idx_obj)->get_value();
            delete idx_obj;
            int n = list->get_size();
            if (idx < 0) idx += n;
            if (idx < 0 || idx >= n)
            {
                DiagnosticBag::report_runtime_error("list assignment index out of range", step->get_pos());
                return new None();
            }

            if (k == 0)
            {
                Object* new_val = nullptr;
                if (op_kind == SyntaxKind::EqualsToken)
                {
                    new_val = evaluate(context, node->get_value());
                    if (should_return())
                    {
                        delete new_val;
                        return new None();
                    }
                }
                else
                {
                    Object* rhs = nullptr;
                    if (node->get_value() != nullptr)
                    {
                        rhs = evaluate(context, node->get_value());
                        if (should_return())
                        {
                            delete rhs;
                            return new None();
                        }
                    }
                    new_val = apply_compound_op(list->get_value((int)idx), op_kind, rhs, node->get_pos());
                    delete rhs;
                    if (new_val == nullptr)
                        return new None();
                }

                list->set_value((int)idx, new_val);
                return new_val->copy();
            }
            else
            {
                container = list->get_value((int)idx);
            }
        }
        return new None();
    }

    // Case 2: Standard or compound variable assignment (x = val, x += val, x++, x--)
    const std::string& var_name = node->get_identifier()->get_text();
    ObjectSymbol obj_sym = context.get_symbol_table()->get_object(var_name);
    if (obj_sym.object == nullptr)
    {
        DiagnosticBag::report_undeclared_identifier(var_name, node->get_identifier()->get_pos());
        return new None();
    }

    Object* orig_value = obj_sym.object;

    // Fast path for ++ and -- on integer variables
    if (orig_value->type() == Type::INTEGER &&
        (op_kind == SyntaxKind::PlusPlusToken || op_kind == SyntaxKind::MinusMinusToken))
    {
        long long v = ((Integer*)orig_value)->get_value() + (op_kind == SyntaxKind::PlusPlusToken ? 1 : -1);
        ((Integer*)orig_value)->set_value(v);
        return new Integer(v);
    }

    Object* value = nullptr;

    if (op_kind == SyntaxKind::EqualsToken)
    {
        value = evaluate(context, node->get_value());
        if (should_return()) 
        {
            delete value;
            return new None();
        }
        // Re-fetch in case evaluating RHS mutated the symbol table
        obj_sym = context.get_symbol_table()->get_object(var_name);
        orig_value = obj_sym.object;
    }
    else
    {
        Object* rhs = nullptr;
        if (node->get_value() != nullptr)
        {
            rhs = evaluate(context, node->get_value());
            if (should_return())
            {
                delete rhs;
                return new None();
            }
            obj_sym = context.get_symbol_table()->get_object(var_name);
            orig_value = obj_sym.object;
        }
        value = apply_compound_op(orig_value, op_kind, rhs, node->get_pos());
        delete rhs;
        if (value == nullptr)
            return new None();
    }

    if (orig_value->type() != value->type())
    {
        DiagnosticBag::report_invalid_assign(type_to_string(value->type()), type_to_string(orig_value->type()),
            node->get_pos());
        delete value;
        return new None();
    }

    if (orig_value->type() == Type::INTEGER)
    {
        ((Integer*)orig_value)->set_value(((Integer*)value)->get_value());
        return value;
    }

    obj_sym.symbol->set_object(var_name, value);
    return value->copy();
}

// Accesses a variable.
Object* Evaluator::evaluate_var_access(Context& context, VarAccessExpressionSyntax* node)
{
    ObjectSymbol sym = context.get_symbol_table()->get_object(node->get_identifier()->get_text());
    if (sym.object == nullptr || sym.object->type() == Type::NONE)
    {
        DiagnosticBag::report_undeclared_identifier(node->get_identifier()->get_text(),
            node->get_identifier()->get_pos());
        return new None();
    }

    return sym.object->copy();
}

// While statement.
Object* Evaluator::evaluate_while(Context& context, WhileExpressionSyntax* node)
{
    Context exec_ctx("while-loop", &context, context.get_symbol_table());
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

// Conditional statement.
Object* Evaluator::evaluate_if(Context& context, IfExpressionSyntax* node)
{
    int n = node->get_size();
    for (int i = 0; i < n; i++)
    {
        Object* condition = evaluate(context, node->get_condition(i));
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
            Context exec_ctx("if-statement", &context, context.get_symbol_table());
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
        Context exec_ctx("if-statement", &context, context.get_symbol_table());
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

// For statement (supports both C-style for(init; cond; update) and foreach for(x in iterable)).
Object* Evaluator::evaluate_for(Context& context, ForExpressionSyntax* node)
{
    Context exec_ctx("for-loop", &context, context.get_symbol_table());

    if (node->is_foreach())
    {
        bool is_typed_decl = (node->get_init()->kind() == SyntaxKind::VarDeclareExpression);
        Type expected_type = Type::NONE;
        std::string var_name;
        if (is_typed_decl)
        {
            VarDeclareExpressionSyntax* decl = (VarDeclareExpressionSyntax*)node->get_init();
            var_name = decl->get_identifier()->get_text();
            expected_type = SyntaxFacts::get_keyword_type(decl->get_var_keyword()->kind());
            Object* init_obj = evaluate(exec_ctx, decl);
            delete init_obj;
            if (should_return()) return new None();
        }
        else
        {
            VarAccessExpressionSyntax* acc = (VarAccessExpressionSyntax*)node->get_init();
            var_name = acc->get_identifier()->get_text();
        }

        Object* iterable = evaluate(exec_ctx, node->get_condition());
        if (should_return())
        {
            delete iterable;
            return new None();
        }

        std::vector<Object*> items;
        if (iterable->type() == Type::LIST)
        {
            List* list = (List*)iterable;
            int n = list->get_size();
            items.reserve(n);
            for (int i = 0; i < n; i++)
                items.push_back(list->get_value(i)->copy());
        }
        else if (iterable->type() == Type::STRING)
        {
            const std::string& s = ((String*)iterable)->get_value();
            items.reserve(s.size());
            for (char c : s)
                items.push_back(new String(std::string(1, c)));
        }
        else
        {
            DiagnosticBag::report_runtime_error("cannot iterate over <" + type_to_string(iterable->type()) + ">",
                node->get_condition()->get_pos());
            delete iterable;
            return new None();
        }
        delete iterable;

        ObjectSymbol sym = exec_ctx.get_symbol_table()->get_object(var_name);
        SymbolTable* target_table = (sym.symbol != nullptr) ? sym.symbol : exec_ctx.get_symbol_table();
        if (!is_typed_decl && sym.object != nullptr)
            expected_type = sym.object->type();

        for (size_t i = 0; i < items.size(); i++)
        {
            Object* item = items[i];
            if (expected_type != Type::NONE && expected_type != item->type())
            {
                DiagnosticBag::report_invalid_assign(type_to_string(item->type()),
                    type_to_string(expected_type), node->get_init()->get_pos());
                for (size_t j = i; j < items.size(); j++)
                    delete items[j];
                return new None();
            }

            target_table->set_object(var_name, item);

            Object* body_obj = evaluate(exec_ctx, node->get_body());
            delete body_obj;

            if (should_return() && !(to_break || to_continue))
            {
                for (size_t j = i + 1; j < items.size(); j++)
                    delete items[j];
                return new None();
            }

            if (to_break)
            {
                to_break = false;
                for (size_t j = i + 1; j < items.size(); j++)
                    delete items[j];
                break;
            }

            if (to_continue)
                to_continue = false;
        }
        return new None();
    }

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
    const std::string& name = node->get_identifier()->get_text();
    std::vector<std::string> arg_names; 

    // Grabs the arg_names from the identifier tokens.
    int n = node->get_arg_size();
    arg_names.reserve(n);
    for (int i = 0; i < n; i++)
        arg_names.push_back(node->get_arg_name(i)->get_text());
    
    Object* val = new Function(name, arg_names, node->get_body_shared());
    context.get_symbol_table()->set_object(name, val);
    return val->copy();
}

// Calls a function.
Object* Evaluator::evaluate_function_call(Context& context, FuncCallExpressionSyntax* node)
{
    ObjectSymbol sym = context.get_symbol_table()->get_object(node->get_identifier()->get_text());
    if (sym.object == nullptr || sym.object->type() == Type::NONE)
    {
        DiagnosticBag::report_undeclared_identifier(node->get_identifier()->get_text(),
            node->get_identifier()->get_pos());
        return new None();
    }

    if (sym.object->type() != Type::FUNCTION)
    {
        DiagnosticBag::report_unexpected_type(type_to_string(sym.object->type()), type_to_string(Type::FUNCTION),
            node->get_identifier()->get_pos());
        return new None();
    }
    
    Function* func = (Function*)sym.object->copy();

    // Check arguments
    int min_args = func->get_min_args();
    int max_args = func->get_max_args();
    int m = node->get_arg_size();
    if (m < min_args || (max_args != -1 && m > max_args))
    {
        Position arg_pos = node->get_pos();
        if (m > 0) 
        {
            Position first_arg = node->get_arg(0)->get_pos();
            Position last_arg = node->get_arg(m - 1)->get_pos();
            arg_pos = Position(first_arg.ln, first_arg.col, first_arg.start, last_arg.end);
        }
        DiagnosticBag::report_illegal_arguments_range(m, min_args, max_args, func->get_name(), arg_pos);
        delete func;
        return new None();
    }
    
    // Evaluate arguments
    std::vector<Object*> args;
    args.reserve(m);
    for (int i = 0; i < m; i++)
    {
        args.push_back(evaluate(context, node->get_arg(i)));
        if (should_return()) 
        {
            for (auto &o : args)
                delete o;
            delete func;
            return new None();
        }
    }

    // Handle user-defined functions
    if (!func->is_built_in())
    {
        if (func->get_body() == nullptr)
        {
            DiagnosticBag::report_runtime_error("cannot call uninitialized function '" + node->get_identifier()->get_text() + "'",
                node->get_pos());
            for (auto &o : args)
                delete o;
            delete func;
            return new None();
        }

        Context exec_ctx(func->get_name(), &context, context.get_symbol_table());
        for (int i = 0; i < m; i++)
            exec_ctx.get_symbol_table()->set_object(func->get_argument_name(i), args[i]);

        DiagnosticBag::push_call(func->get_name(), node->get_pos());
        Object* body_obj = evaluate(exec_ctx, func->get_body());
        DiagnosticBag::pop_call();

        delete body_obj;
        delete func;

        if (should_return() && !return_value) 
            return new None();

        Object* result = nullptr;
        if (return_value)
        {
            result = return_value;
            return_value = nullptr;
        }
        
        if (result != nullptr) return result;
        else return new None();
    }

    // Handle built-in functions directly without allocating a temporary Context/SymbolTable
    std::string func_name = func->get_name();
    delete func;
    Position call_pos = node->get_pos();
    SyntaxNode* first_arg_node = (m > 0) ? node->get_arg(0) : nullptr;
    Object* result = nullptr;

    switch (SyntaxFacts::get_keyword_kind(func_name))
    {
        case SyntaxKind::PrintFunction:
            result = BuiltInFunctions::PRINT(args, call_pos);
            break;
        case SyntaxKind::InputFunction:
            result = BuiltInFunctions::INPUT(args, call_pos);
            break;
        case SyntaxKind::SplitFunction:
            result = BuiltInFunctions::SPLIT(args, call_pos);
            break;
        case SyntaxKind::SizeFunction:
            result = BuiltInFunctions::SIZE(args, call_pos);
            break;
        case SyntaxKind::TypeFunction:
            result = BuiltInFunctions::TYPE(args, call_pos);
            break;
        case SyntaxKind::ToBoolFunction:
            result = BuiltInFunctions::TO_BOOL(args, call_pos);
            break;
        case SyntaxKind::ToIntFunction:
            result = BuiltInFunctions::TO_INT(args, call_pos);
            break;
        case SyntaxKind::ToDoubleFunction:
            result = BuiltInFunctions::TO_DOUBLE(args, call_pos);
            break;
        case SyntaxKind::ToStringFunction:
            result = BuiltInFunctions::TO_STRING(args, call_pos);
            break;
        case SyntaxKind::SetIndexFunction:
            result = BuiltInFunctions::SET_INDEX(args, call_pos);
            break;
        case SyntaxKind::PushFunction:
            result = BuiltInFunctions::PUSH(context, args, first_arg_node, call_pos);
            break;
        case SyntaxKind::PopFunction:
            result = BuiltInFunctions::POP(context, args, first_arg_node, call_pos);
            break;
        case SyntaxKind::RangeFunction:
            result = BuiltInFunctions::RANGE(args, call_pos);
            break;
        case SyntaxKind::SortFunction:
            result = BuiltInFunctions::SORT(args, call_pos);
            break;
        case SyntaxKind::ReverseFunction:
            result = BuiltInFunctions::REVERSE(args, call_pos);
            break;
        case SyntaxKind::JoinFunction:
            result = BuiltInFunctions::JOIN(args, call_pos);
            break;
        case SyntaxKind::AbsFunction:
            result = BuiltInFunctions::ABS(args, call_pos);
            break;
        case SyntaxKind::MinFunction:
            result = BuiltInFunctions::MIN(args, call_pos);
            break;
        case SyntaxKind::MaxFunction:
            result = BuiltInFunctions::MAX(args, call_pos);
            break;
        case SyntaxKind::SqrtFunction:
            result = BuiltInFunctions::SQRT(args, call_pos);
            break;
        case SyntaxKind::PowModFunction:
            result = BuiltInFunctions::POW_MOD(args, call_pos);
            break;
        case SyntaxKind::SetModFunction:
            result = BuiltInFunctions::SET_MOD(args, call_pos);
            break;
        case SyntaxKind::ReadFileFunction:
            result = BuiltInFunctions::READ_FILE(args, call_pos);
            break;
        case SyntaxKind::WriteFileFunction:
            result = BuiltInFunctions::WRITE_FILE(args, call_pos);
            break;
        case SyntaxKind::ClockFunction:
            result = BuiltInFunctions::CLOCK(args, call_pos);
            break;
        case SyntaxKind::ExitFunction:
            result = BuiltInFunctions::EXIT(args, call_pos);
            break;
        default:
        {
            DiagnosticBag::report_unreachable_code("invalid builtin function", node->get_identifier()->get_pos());
            result = new None();
            break;
        }
    }

    for (auto &o : args)
        delete o;
    return result;
}

// Return expression.
Object* Evaluator::evaluate_return(Context& context, ReturnExpressionSyntax* node)
{
    if (node->get_to_return())
    {
        Object* result = evaluate(context, node->get_to_return());
        if (should_return()) 
        {
            delete result;
            return new None();
        }

        if (return_value != nullptr) delete return_value;
        return_value = result;
        return new None();
    }

    if (return_value != nullptr) delete return_value;
    return_value = new None();
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
