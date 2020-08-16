#include "nodes.h"

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
		default:
			result.failure(new RuntimeError("Unknow Operation " + TOKENTYPES[type], 
				get_start(), get_end(), context));
			return result;
	}

	result.success(val);
	return result;
}