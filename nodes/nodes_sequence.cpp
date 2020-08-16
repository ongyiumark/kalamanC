#include "nodes.h"

////////////////////////////////////
// SEQUENCE NODE CLASS
////////////////////////////////////
SequenceNode::SequenceNode(std::vector<Node*>& elems, Position s, Position e) 
	: Node(NodeType::SEQUENCE, s, e), elements(elems) {}

void SequenceNode::print(std::ostream& os) const 
{
	os << "{";
	int n = elements.size();
	for (int i = 0; i < n; i++)
	{
		os << *elements[i];
		if (i != n-1) os << " -> ";
	}
	os << "}";
}

RTResult SequenceNode::visit(Context* context)
{
	RTResult result = RTResult();

	for(Node* n : elements)
	{
		result.register_value(n->visit(context));
		if (result.should_return()) return result;
	}

	Value* res_val = new Null();
	res_val->set_position(get_start(), get_end());
	result.success(res_val);
	return result;
}