#include "nodes.h"

////////////////////////////////////
// LIST NODE CLASS
////////////////////////////////////
ListNode::ListNode(std::vector<Node*>& elems, Position s, Position e) 
	: Node(NodeType::LIST, s, e), elements(elems) {}

void ListNode::print(std::ostream& os) const 
{
	os << "[";
	int n = elements.size();
	for (int i = 0; i < n; i++)
	{
		os << *elements[i];
		if (i != n-1) os << ", ";
	}
	os << "]";
}

RTResult ListNode::visit(Context* context)
{
	RTResult result = RTResult();

	std::vector<Value*> values;
	for(Node* n : elements)
	{
		Value* val = result.register_value(n->visit(context));
		if (result.should_return()) return result;
		values.push_back(val);
	}
	Value* res_val = new List(values);
	res_val->set_position(get_start(), get_end());
	result.success(res_val);
	return result;
}