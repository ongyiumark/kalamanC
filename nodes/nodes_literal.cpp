#include "nodes.h"
#include <limits.h>

////////////////////////////////////
// NUMBER NODE CLASS
////////////////////////////////////
NumberNode::NumberNode(Token* tok, Position s, Position e) : Node(NodeType::NUMBER, s, e), number(tok) {}

void NumberNode::print(std::ostream& os) const
{
	os << "(" << NODETYPES[get_type()] << ":" << *number << ")"; 
}

bool is_valid(const std::string& s)
{
	std::string highest = std::to_string(LLONG_MAX);
	int n = (s[0] == '-' ? s.size()-1 : s.size());
	if (n < highest.size()) return true;
	if (n > highest.size()) return false;

	if (highest.compare((s[0] == '-' ? s.substr(1) : s)) < 0) return false;
	return true;
}

RTResult NumberNode::visit(Context* context) const
{
	RTResult result = RTResult();
	Value *val = new Null();

	std::string num_str = number->get_value();
	switch(number->get_type())
	{
		case TokenType::INTEGER:
		{
			if (!is_valid(num_str))
			{
				result.failure(new RuntimeError("Integer is too large, maximum is " + std::to_string(LLONG_MAX),
					get_start(), get_end(), context));
				return result;
			}
			val = new Integer(std::stoll(num_str));
			break;
		}
		case TokenType::DOUBLE:
			val = new Double(std::stold(number->get_value()));
			break;
	}

	result.success(val);
	return result;
}

////////////////////////////////////
// STRING NODE CLASS
////////////////////////////////////
StringNode::StringNode(Token* tok, Position s, Position e) : Node(NodeType::STRINGNODE, s, e), value(tok) {}

void StringNode::print(std::ostream& os) const
{
	os << "(" << NODETYPES[get_type()] << ":" << *value << ")"; 
}

RTResult StringNode::visit(Context* context) const
{
	RTResult result = RTResult();
	Value *val = new String(value->get_value());

	result.success(val);
	return result;
}