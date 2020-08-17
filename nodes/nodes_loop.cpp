#include "nodes.h"

#include "../constants.h"

////////////////////////////////////
// FOR LOOP NODE CLASS
////////////////////////////////////
ForLoopNode::ForLoopNode(Node* i, Node * c, Node* u, Node* b, Position s, Position e)
	: Node(NodeType::FORNODE, s, e), init(i), condition(c), update(u), body(b) {}

void ForLoopNode::print(std::ostream& os) const 
{
	os << "{FOR(";
	os << *init << ", " << *condition << ", " << *update << ") -> ";
	os << *body;
	os << "}";
}

RTResult ForLoopNode::visit(Context* context)
{
	RTResult result = RTResult();

	result.register_value(init->visit(context));
	if (result.should_return()) return result;
	while(1)
	{
		Value* cond_value = result.register_value(condition->visit(context));
		if (result.should_return()) return result;

		if (!cond_value->is_true())
			break;

		Value* body_val = result.register_value(body->visit(context));
		if (result.should_return() && !(result.get_continue() || result.get_break())) return result;

		if (result.get_break())
			break;

		bool to_continue =  result.get_continue();

		result.register_value(update->visit(context));
		if (result.should_return()) return result;

		if (to_continue)
			continue;
	}

	Value* res_val = new Null();
	result.success(res_val);
	return result;
}

////////////////////////////////////
// WHILE LOOP NODE CLASS
////////////////////////////////////
WhileLoopNode::WhileLoopNode(Node * c, Node* b, Position s, Position e)
	: Node(NodeType::WHILENODE, s, e), condition(c), body(b) {}

void WhileLoopNode::print(std::ostream& os) const 
{
	os << "{WHILE(";
	os << *condition <<") -> ";
	os << *body;
	os << "}";
}

RTResult WhileLoopNode::visit(Context* context)
{
	RTResult result = RTResult();

	while(1)
	{
		Value* cond_value = result.register_value(condition->visit(context));
		if (result.should_return()) return result;

		if (!cond_value->is_true())
			break;

		Value* body_val = result.register_value(body->visit(context));
		if (result.should_return() && !(result.get_continue() || result.get_break())) return result;

		if (result.get_break())
			break;

		if (result.get_continue())
			continue;
	}

	Value* res_val = new Null();
	result.success(res_val);
	return result;
}

////////////////////////////////////
// BREAK NODE CLASS
////////////////////////////////////
BreakNode::BreakNode(Position s, Position e)
	: Node(NodeType::BREAKNODE, s, e) {}

void BreakNode::print(std::ostream& os) const 
{
	os << "(BREAK)";
}

RTResult BreakNode::visit(Context* context)
{
	RTResult result = RTResult();
	result.success_break();
	return result;
}

////////////////////////////////////
// CONTINUE NODE CLASS
////////////////////////////////////
ContinueNode::ContinueNode(Position s, Position e)
	: Node(NodeType::CONTNODE, s, e) {}

void ContinueNode::print(std::ostream& os) const 
{
	os << "(CONTINUE)";
}

RTResult ContinueNode::visit(Context* context)
{
	RTResult result = RTResult();
	result.success_continue();
	return result;
}


