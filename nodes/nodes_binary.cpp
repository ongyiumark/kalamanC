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

RTResult BinOpNode::visit(Context* context)
{
	RTResult result = RTResult();

	Value* left_val = result.register_value(left->visit(context));
	if (result.should_return()) return result;


	Value* right_val = result.register_value(right->visit(context));
	if (result.should_return()) return result;

	TokenType type = op->get_type();
	Value* val = new Null();

	switch (type)
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
		case TokenType::EQUALS:
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
			result.failure(new RuntimeError("Unknow Operation " + op->as_string(), 
				get_start(), get_end(), context));
			return result;
	}

	if (val->get_type() == ValueType::NULLTYPE)
	{
		result.failure(new RuntimeError("Illegal Operation or Division by zero", 
				get_start(), get_end(), context));
		return result;
	}
	val->set_position(get_start(), get_end());
	result.success(val);
	return result;
}