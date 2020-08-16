#include "nodes.h"

////////////////////////////////////
// NO OP NODE CLASS
////////////////////////////////////
NoOpNode::NoOpNode(Position s, Position e) 
	: Node(NodeType::NOOP, s, e) {}

void NoOpNode::print(std::ostream& os) const 
{
	os << "(" << NODETYPES[NodeType::NOOP] << ")";
}

RTResult NoOpNode::visit(Context* context) 
{
	RTResult result = RTResult();
	result.success(new Null());
	return result;
}