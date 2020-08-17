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

RTResult SequenceNode::visit(Context* context) const
{
	RTResult result = RTResult();

	for(Node* n : elements)
	{
		result.register_value(n->visit(context));
		if (result.should_return()) return result;
	}

	Value* res_val = new Null();
	result.success(res_val);
	return result;
}

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

RTResult ListNode::visit(Context* context) const
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
	result.success(res_val);
	return result;
}