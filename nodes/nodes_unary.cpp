#include "nodes.h"

#include "../constants.h"

////////////////////////////////////
// UNARY OPERATION CLASS
////////////////////////////////////
UnaryOpNode::UnaryOpNode(Token* tok, Node* c, Position s, Position e) 
	: Node(NodeType::UNARYOP, s, e), op(tok), child(c) {}

void UnaryOpNode::print(std::ostream& os) const
{
	os << "(";
	os << *op << ", ";
	os << *child << ")"; 
}

RTResult UnaryOpNode::visit(Context* context)
{
	RTResult result = RTResult();

	Value* child_val = result.register_value(child->visit(context));
	if (result.should_return()) return result;

	Value* val = new Null();
	if (op->matches(TokenType::MINUS))
		val = child_val->multiply(new Integer(-1));
	else if (op->matches(TokenType::PLUS))
		val = child_val->multiply(new Integer(1));
	else if (op->matches(TokenType::KEYWORD, KT_NOT))
		val = child_val->notted();


	if (val->get_type() == ValueType::NULLTYPE)
	{
		result.failure(new RuntimeError("Unknown Operation: " + op->as_string(), get_start(), get_end(), context));
		return result;
	}

	val->set_position(get_start(), get_end());
	result.success(val);
	return result;
}