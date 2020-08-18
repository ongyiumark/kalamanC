#include "nodes.h"

////////////////////////////////////
// VARIABLE DECLARE NODE CLASS
////////////////////////////////////
VarDeclareNode::VarDeclareNode(ValueType t, Token* id, Position s, Position e)
	: Node(NodeType::VARDECL, s, e), type(t), identifier(id) {}

void VarDeclareNode::print(std::ostream& os) const 
{
	os << "(DECLARE:" << VALUETYPES[type] << ":" << identifier->get_value() << ")";
}

const RTResult VarDeclareNode::visit(const Context* context) const
{
	RTResult result = RTResult();
	
	Value* val = new Null();
	switch (type)
	{
		case ValueType::INTEGERTYPE:
			val = new Integer(0);
			break;
		case ValueType::DOUBLETYPE:
			val = new Double(0);
			break;
		case ValueType::STRINGTYPE:
			val = new String("");
			break;
		case ValueType::LISTTYPE:
			val = new List(std::vector<Value*>());
			break;
		case ValueType::FUNCTIONTYPE:
		{
			std::vector<std::string> args;
			val = new Function("<uninitialized>", NULL, args);
			break;
		}
		default:
			val = new Null();
	}

	context->get_table()->set_value(identifier->get_value(), val);
	result.success(new Null());
	return result;
}

////////////////////////////////////
// VARIABLE ASSIGN NODE CLASS
////////////////////////////////////
VarAssignNode::VarAssignNode(Token* id, Node* c, Position s, Position e)
	: Node(NodeType::VARASSIGN, s, e), identifier(id), child(c) {}

void VarAssignNode::print(std::ostream& os) const 
{
	os << "(ASSIGN:" << identifier->get_value() << " <- " << *child << ")";
}

const RTResult VarAssignNode::visit(const Context* context) const
{
	RTResult result = RTResult();
	
	Value* val = result.register_value(child->visit(context));
	if (result.should_return()) return result;

	Value* orig_val = context->get_table()->get_value(identifier->get_value());

	if (orig_val->get_type() != val->get_type())
	{
		result.failure(new RuntimeError(
			"Unmatching Types: " + VALUETYPES[orig_val->get_type()] + " and " + VALUETYPES[val->get_type()],
			get_start(), get_end(), context));
		return result;
	}

	context->get_table()->set_value(identifier->get_value(), val);

	result.success(val);
	return result;
}

////////////////////////////////////
// VARIABLE ACCESS NODE CLASS
////////////////////////////////////
VarAccessNode::VarAccessNode(Token* id, Position s, Position e)
	: Node(NodeType::VARASSIGN, s, e), identifier(id) {}

void VarAccessNode::print(std::ostream& os) const 
{
	os << "(" << *identifier << ")";
}

const RTResult VarAccessNode::visit(const Context* context) const
{
	RTResult result = RTResult();
	
	Value* val = context->get_table()->get_value(identifier->get_value());
	if (val->get_type() == ValueType::NULLTYPE)
	{
		result.failure(new RuntimeError(
			"Undeclared identifier " + identifier->get_value(),
			get_start(), get_end(), context));
		return result;
	}

	result.success(val);
	return result;
}