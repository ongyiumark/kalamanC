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

	std::vector<Value*> vals;
	while(1)
	{
		Value* cond_value = result.register_value(condition->visit(context));
		if (result.should_return() && (result.get_continue() || result.get_break())) return result;

		if (!cond_value->is_true())
			break;

		if (result.get_continue() || result.get_break())
		{
			std::string details = "Cannot put " + KT_RETURN + " or " + KT_BREAK;
			details += " on the head of the loop";
			result.failure(new RuntimeError(details, get_start(), get_end(), context));
			return result;
		}

		Value* body_val = result.register_value(body->visit(context));
		if (result.should_return() && (result.get_continue() || result.get_break())) return result;

		if (result.get_break())
			break;

		result.register_value(update->visit(context));
		if (result.should_return() && (result.get_continue() || result.get_break())) return result;
		if (result.get_continue() || result.get_break())
		{
			std::string details = "Cannot put " + KT_RETURN + " or " + KT_BREAK;
			details += " on the head of the loop";
			result.failure(new RuntimeError(details, get_start(), get_end(), context));
			return result;
		}

		if (result.get_continue())
			continue;

		if (body_val->get_type() != ValueType::NULLTYPE)
			vals.push_back(body_val);
	}

	Value* res_val = new List(vals);
	res_val->set_position(get_start(), get_end());
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

	std::vector<Value*> vals;
	while(1)
	{
		Value* cond_value = result.register_value(condition->visit(context));
		if (result.should_return() && (result.get_continue() || result.get_break())) return result;

		if (!cond_value->is_true())
			break;

		if (result.get_continue() || result.get_break())
		{
			std::string details = "Cannot put " + KT_RETURN + " or " + KT_BREAK;
			details += " on the head of the loop";
			result.failure(new RuntimeError(details, get_start(), get_end(), context));
			return result;
		}

		Value* body_val = result.register_value(body->visit(context));
		if (result.should_return() && (result.get_continue() || result.get_break())) return result;

		if (result.get_break())
			break;

		if (result.get_continue())
			continue;

		if (body_val->get_type() != ValueType::NULLTYPE)
			vals.push_back(body_val);
	}

	Value* res_val = new List(vals);
	res_val->set_position(get_start(), get_end());
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


