#include "nodes.h"
#include "../constants.h"
#include "../interpreter/builtin_functions.h"

////////////////////////////////////
// FUNCTION DEFINE NODE CLASS
////////////////////////////////////
FuncDefNode::FuncDefNode(Token* id, std::vector<Token*>& arg_n, Node * b,  Position s, Position e)
	: Node(NodeType::FUNCDEF, s, e), identifier(id), arg_names(arg_n), body(b) {}

void FuncDefNode::print(std::ostream& os) const 
{
	os << "{FUNCDEF:" << identifier->get_value() << " -> ";
	int n = arg_names.size();
	os << "(";
	for (int i = 0; i < n; i++)
	{
		os << arg_names[i]->get_value();
		if (i != n-1) os << ", ";
	}
	os << ")}";
}

const RTResult FuncDefNode::visit(const Context* context) const
{
	RTResult result = RTResult();
	
	std::vector<std::string> args;
	for (Token* t : arg_names)
		args.push_back(t->get_value());

	Value* val = new Function(identifier->get_value(), body, args);
	context->get_table()->set_value(identifier->get_value(), val);
	result.success(new Null());
	return result;
}

////////////////////////////////////
// FUNCTION CALL NODE CLASS
////////////////////////////////////
FuncCallNode::FuncCallNode(Node* node, std::vector<Node*>& arg, Position s, Position e)
	: Node(NodeType::FUNCDEF, s, e), to_call(node), args(arg) {}

void FuncCallNode::print(std::ostream& os) const 
{
	os << "{FUNCALL:" << *to_call << " <- ";
	int n = args.size();
	os << "(";
	for (int i = 0; i < n; i++)
	{
		os << *args[i];
		if (i != n-1) os << ", ";
	}
	os << ")}";
}

const RTResult FuncCallNode::visit(const Context* context) const
{
	RTResult result = RTResult();
	Value* func = result.register_value(to_call->visit(context));
	if (result.should_return()) return result;

	if (func->get_type() != ValueType::FUNCTIONTYPE)
	{
		result.failure(new RuntimeError("Cannot call " + VALUETYPES[func->get_type()],
			get_start(), get_end(), context));
		return result;
	}

	if (!func->get_func_body() && !func->is_bifunc())
	{
		result.failure(new RuntimeError("Cannot call uninitialized function",
			get_start(), get_end(), context));
		return result;
	}

	std::vector<std::string> arg_names = func->get_func_argnames();

	std::vector<Value*> arg_vals;
	for (Node* n : args)
	{
		Value* val = result.register_value(n->visit(context));
		if (result.should_return()) return result;
		arg_vals.push_back(val);
	}
	
	// Generate context
	Context* exec_ctx = new Context(func->get_func_name(), context->copy(), get_start(), 
		new SymbolTable(context->get_table()));

	// Check arguments
	int n = arg_names.size();
	int m = arg_vals.size();
	if (n != m)
	{
		result.failure(new RuntimeError("Expected " + std::to_string(n) + ", provided with " + std::to_string(m),
			get_start(), get_end(), context));
		return result;
	}

	// Populate arguments
	for (int i = 0; i < n; i++)
		exec_ctx->get_table()->set_value(arg_names[i], arg_vals[i]);

	if (!func->is_bifunc())
	{
		result.register_value(func->get_func_body()->visit(exec_ctx));
		if (result.should_return() && !result.get_return_value()) return result;

		Value* ret = new Null();
		if (result.get_return_value()) ret = result.get_return_value();
		result.success(ret);
		return result;
	}

	BuiltInName bi_name = func->get_bifunc_name();
	switch (bi_name)
	{
		case BuiltInName::PRINT:
			result.register_value(BFunctions::PRINT(exec_ctx));
			return result;
		case BuiltInName::INPUTSTR:
			result.register_value(BFunctions::INPUTSTR(exec_ctx));
			return result;
		default:
			result.failure(new RuntimeError("Undefined Builtin Function", get_start(), get_end(), context));
			return result;
	}
}

////////////////////////////////////
// RETURN NODE CLASS
////////////////////////////////////
ReturnNode::ReturnNode(Node* ret, Position s, Position e) 
	: Node(NodeType::RETURNNODE, s, e), return_node(ret) {}

void ReturnNode::print(std::ostream& os) const 
{
	os << "(RETURN ";
	if (return_node) os << *return_node;
	os << ")";
}

const RTResult ReturnNode::visit(const Context* context) const
{
	RTResult result = RTResult();
	result.success_return(new Null());
	if (return_node) 
	{
		Value* val = result.register_value(return_node->visit(context));
		if (result.should_return()) return result;

		result.success_return(val);
	}
	return result;
}