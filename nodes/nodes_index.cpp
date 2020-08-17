#include "nodes.h"

////////////////////////////////////
// INDEX NODE CLASS
////////////////////////////////////
IndexNode::IndexNode(Node* to_idx, Node* idx, Position s, Position e)
	: Node(NodeType::INDEXNODE, s, e), to_index(to_idx), index(idx) {}

void IndexNode::print(std::ostream& os) const 
{
	os << "{INDEX:";
	os << *to_index << " <- ";
	os << *index;
	os << "}";
}

const RTResult IndexNode::visit(const Context* context) const
{
	RTResult result = RTResult();
	Value* val = result.register_value(to_index->visit(context));
	if (result.should_return()) return result;

	ValueType type = val->get_type();
	if (type != ValueType::STRINGTYPE && type != ValueType::LISTTYPE)
	{
		result.failure(new RuntimeError("Cannot index " + VALUETYPES[val->get_type()],
			get_start(), get_end(), context));
		return result;
	}

	Value* idx = result.register_value(index->visit(context));
	if (result.should_return()) return result;

	if(idx->get_type() != ValueType::INTEGERTYPE)
	{
		result.failure(new RuntimeError("Index must be " + VALUETYPES[ValueType::INTEGERTYPE],
		get_start(), get_end(), context));
		return result;
	}

	int n = val->get_size();
	int i = idx->get_int_value();
	if (i < 0) i += n;
	if (i < 0 || i >= n)
	{		
		result.failure(new RuntimeError("Invalid index, expected " 
			+ std::to_string(-n) + " to " + std::to_string(n-1),
		get_start(), get_end(), context));
		return result;
	}

	Value* res_val = val->index(i);

	result.success(res_val);
	return result;
}
