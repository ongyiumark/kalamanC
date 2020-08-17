#include "nodes.h"

#include "../constants.h"

////////////////////////////////////
// BINARY OPERATION CLASS
////////////////////////////////////
BinOpNode::BinOpNode(Token* tok, Node* l, Node* r, Position s, Position e) 
	: Node(NodeType::BINOP, s, e), op(tok), left(l), right(r) {}

void BinOpNode::print(std::ostream& os) const
{
	os << "(" << *left << ", ";
	os << *op << ", ";
	os << *right << ")"; 
}

const RTResult BinOpNode::visit(const Context* context) const
{
	RTResult result = RTResult();

	Value* left_val = result.register_value(left->visit(context));
	if (result.should_return()) return result;

	Value* right_val = result.register_value(right->visit(context));
	if (result.should_return()) return result;

	Value* val = new Null();

	switch (op->get_type())
	{
		case TokenType::PLUS:
			val = left_val->add(right_val);
			break;
		case TokenType::MINUS:
			val = left_val->subtract(right_val);
			break;
		case TokenType::MUL:
			val = left_val->multiply(right_val);
			break;
		case TokenType::DIV:
			val = left_val->divide(right_val);
			break;
		case TokenType::MOD:
			val = left_val->modulo(right_val);
			break;
		case TokenType::POW:
			val = left_val->power(right_val);
			break;
		case TokenType::DEQUALS:
			val = left_val->equals(right_val);
			break;
		case TokenType::NEQUALS:
			val = left_val->not_equals(right_val);
			break;
		case TokenType::LESS:
			val = left_val->less_than(right_val);
			break;
		case TokenType::GREATER:
			val = left_val->greater_than(right_val);
			break;
		case TokenType::GEQUALS:
			val = left_val->greater_equals(right_val);
			break;
		case TokenType::LEQUALS:
			val = left_val->less_equals(right_val);
			break;
		case TokenType::KEYWORD:
		{
			std::string keyword = op->get_value();
			if (keyword == KT_AND)
			{
				val = left_val->and_with(right_val);
				break;
			}
			else if (keyword == KT_OR) 
			{
				val = left_val->or_with(right_val);
				break;
			}
			else if (keyword == KT_XOR)
			{
				val = left_val->xor_with(right_val);
				break;
			}
		}
		default:
			std::string details = "Unknow Binary Operation " + op->as_string() + " for ";
			details += VALUETYPES[left_val->get_type()] + " and " + VALUETYPES[right_val->get_type()];
			result.failure(new RuntimeError(details, get_start(), get_end(), context));
			return result;
	}

	if (val->get_type() == ValueType::NULLTYPE)
	{
		std::string details = "Division by zero or ";
		details += "Unknow Binary Operation: " + op->as_string() + " for ";
		details += VALUETYPES[left_val->get_type()] + " and " + VALUETYPES[right_val->get_type()];

		result.failure(new RuntimeError(details, get_start(), get_end(), context));
			return result;
	}

	result.success(val);
	return result;
}

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

const RTResult UnaryOpNode::visit(const Context* context) const
{
	RTResult result = RTResult();

	Value* child_val = result.register_value(child->visit(context));
	if (result.should_return()) return result;

	Value* val = new Null();
	switch(op->get_type())
	{
		case TokenType::MINUS:
			val = child_val->multiply(new Integer(-1));
			break;
		case TokenType::PLUS:
		{
			switch(child_val->get_type())
			{
				case ValueType::INTEGERTYPE:
				case ValueType::DOUBLETYPE:
					val = child_val;
					break;
				default:
					val = new Null();
			}
			break;
		}
		case TokenType::KEYWORD:
			if (op->get_value() == KT_NOT) val = child_val->notted();
			break;
	}

	if (val->get_type() == ValueType::NULLTYPE)
	{
		std::string details = "Unknown Unary Operation: " + op->as_string() + " for " + VALUETYPES[child_val->get_type()];
		result.failure(new RuntimeError(details, get_start(), get_end(), context));
		return result;
	}

	result.success(val);
	return result;
}

////////////////////////////////////
// NO OPERATION NODE CLASS
////////////////////////////////////
NoOpNode::NoOpNode(Position s, Position e) 
	: Node(NodeType::NOOP, s, e) {}

void NoOpNode::print(std::ostream& os) const 
{
	os << "(" << NODETYPES[NodeType::NOOP] << ")";
}

const RTResult NoOpNode::visit(const Context* context) const
{
	RTResult result = RTResult();
	result.success(new Null());
	return result;
}