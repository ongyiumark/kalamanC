#include "nodes.h"

////////////////////////////////////
// NODE BASE CLASS
////////////////////////////////////
Node::Node(NodeType n, Position s, Position e) : type(n), start(s), end(e) {} 

RTResult Node::visit(Context* context)
{
	RTResult result = RTResult();
	result.failure(new RuntimeError("Unknown Node: "+NODETYPES[type], start,end, context));
	return result;
}

NodeType Node::get_type() const
{
	return type;
}

Position Node::get_start() const
{
	return start;
}

Position Node::get_end() const
{
	return end;
}

std::ostream& operator<<(std::ostream& os, const Node &node)
{
	node.print(os);
	return os;
}


