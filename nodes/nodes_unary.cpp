#include "nodes.h"

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