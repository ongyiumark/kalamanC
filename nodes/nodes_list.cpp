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