#include "nodes.h"

////////////////////////////////////
// NUMBER NODE CLASS
////////////////////////////////////
NumberNode::NumberNode(Token* tok, Position s, Position e) : Node(NodeType::NUMBER, s, e), number(tok) {}

void NumberNode::print(std::ostream& os) const
{
	os << "(" << NODETYPES[get_type()] << ":" << *number << ")"; 
}

RTResult NumberNode::visit(Context* context)
{
	RTResult result = RTResult();
	Value *val = new Null();
	if (number->matches(TokenType::INTEGER)) 
	{
		std::string num_str = number->get_value();
		val = new Integer(std::stoll(num_str));
	}
	else if (number->matches(TokenType::DOUBLE))
		val = new Double(std::stold(number->get_value()));

	val->set_position(get_start(), get_end());
	result.success(val);
	return result;
}