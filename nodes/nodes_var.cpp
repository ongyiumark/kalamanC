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

RTResult VarDeclareNode::visit(Context* context)
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
		default:
			val = new Null();
	}

	val->set_position(get_start(), get_end());
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

RTResult VarAssignNode::visit(Context* context)
{
	RTResult result = RTResult();
	
	Value* val = result.register_value(child->visit(context));
	Value* orig_val = context->get_table()->get_value(identifier->get_value());

	if (orig_val->get_type() != val->get_type())
	{
		result.failure(new RuntimeError(
			"Unmatching Types: " + VALUETYPES[orig_val->get_type()] + " and " + VALUETYPES[val->get_type()],
			get_start(), get_end(), context));
		return result;
	}

	val->set_position(get_start(), get_end());
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

RTResult VarAccessNode::visit(Context* context)
{
	RTResult result = RTResult();
	
	Value* val = context->get_table()->get_value(identifier->get_value());
	if (val->get_type() == ValueType::NULLTYPE)
	{
		result.failure(new RuntimeError(
			"Undeclared variable " + identifier->get_value(),
			get_start(), get_end(), context));
		return result;
	}

	val->set_position(get_start(), get_end());
	result.success(val);
	return result;
}