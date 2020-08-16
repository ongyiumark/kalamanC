#include "nodes.h"

////////////////////////////////////
// STRING NODE CLASS
////////////////////////////////////
StringNode::StringNode(Token* tok, Position s, Position e) : Node(NodeType::STRINGNODE, s, e), value(tok) {}

void StringNode::print(std::ostream& os) const
{
	os << "(" << NODETYPES[get_type()] << ":" << *value << ")"; 
}

RTResult StringNode::visit(Context* context)
{
	RTResult result = RTResult();
	Value *val = new String(value->get_value());

	val->set_position(get_start(), get_end());
	result.success(val);
	return result;
}