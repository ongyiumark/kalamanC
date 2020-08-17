#include "nodes.h"

////////////////////////////////////
// CONDITION NODE CLASS
////////////////////////////////////
ConditionNode::ConditionNode(std::vector<Node*>& cs, std::vector<Node*>& stms, Node* ecs, Position s, Position e) 
	: Node(NodeType::CONDITION, s, e), cases(cs), statements(stms), else_case(ecs)  {}

void ConditionNode::print(std::ostream& os) const 
{
	os << "{";
	int n = cases.size();
	for (int i = 0; i < n; i++)
	{
		os << "(";
		os << *cases[i];
		os << "-> ";
		os << *statements[i];
		os << ")";
		os << ", ";
	}
	if (else_case) os << *else_case;
	os << "}";
}

RTResult ConditionNode::visit(Context* context) const
{
	RTResult result = RTResult();
	int n = cases.size();

	for (int i = 0; i < n; i++){
		Value* condition = result.register_value(cases[i]->visit(context));
		if (result.should_return()) return result;

		if (condition->is_true())
		{
			Value* stm = result.register_value(statements[i]->visit(context));
			if (result.should_return()) return result;
			result.success(stm);
			return result;
		}
	}

	if (else_case)
	{
		Value* stm = result.register_value(else_case->visit(context));
		if (result.should_return()) return result;
		result.success(stm);
		return result;
	}

	Value* res_val = new Null();
	result.success(res_val);
	return result;
}