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
	result.success(new Integer(std::stoll(number->get_value())));
	return result;
}

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

////////////////////////////////////
// UNARY OPERATION CLASS
////////////////////////////////////
UnaryOpNode::UnaryOpNode(Token* tok, Node* c, Position s, Position e) 
	: Node(NodeType::UNARYOP, s, e), op(tok), child(c) {}

void UnaryOpNode::print(std::ostream& os) const
{
	os << "(";
	os << *op << ", ";
	os << *child << ")"; 
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

////////////////////////////////////
// NO OP NODE CLASS
////////////////////////////////////
NoOpNode::NoOpNode(Position s, Position e) 
	: Node(NodeType::NOOP, s, e) {}

void NoOpNode::print(std::ostream& os) const {}


