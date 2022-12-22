#include "calc.hpp"
using namespace std;
using namespace GiNaC;

Node::Node() { set_links_null(); }
Node::Node(numeric * number) { set_numeric(number); set_links_null(); }
Node::Node(Node & old_node)
{
	links = old_node.links;
	data = old_node.data;
	if (old_node.data.type == TYPE_NUMERIC) { data.data_numeric = new numeric {*old_node.data.data_numeric}; }
	else if (old_node.data.type == TYPE_SYMBOL) { data.data_symbol = new symbol {*old_node.data.data_symbol}; }
	else if (old_node.data.type == TYPE_EX) { data.data_ex = new ex {*old_node.data.data_ex}; }
}
Node::~Node()
{
	if (data.type == TYPE_NUMERIC) { delete data.data_numeric; }
	else if (data.type == TYPE_SYMBOL) { delete data.data_symbol; }
	else if (data.type == TYPE_EX) { delete data.data_ex; }
}
void Node::set_links_null()
{
	links.down = nullptr;
	links.up = nullptr;
	links.next = nullptr;
	links.prev = nullptr;
}
nodeDataType Node::get_type()
{
	if (data.deletable) { return TYPE_DELETABLE; }
	return data.type;
}
Node * Node::set_numeric(numeric * number)
{
	data.type = TYPE_NUMERIC;
	data.data_numeric = number;
	return this;
}
Node * Node::set_cmd(Node * (*cmd)(Node *, NodeContainer *))
{
	data.type = TYPE_CMD;
	data.cmd = cmd;
	return this;
}

Node * Node::set_delim(const string & delim)
{
	data.type = TYPE_DELIM;
	data.cmd = NativeCMD::get_cmd_ptr(delim);
	return this;
}
Node * Node::set_symbol(const string & name)
{
	data.type = TYPE_DELIM;
	data.data = new symbol;
	return this;
}
Node * Node::set_ex(ex * expr)
{
	data.type = TYPE_EX;
	data.data = expr;
	return this;
}

Node * Node::set_type(const nodeDataType type)
{
	if (type == data.type) { return this; }
	if (type == TYPE_DELETABLE) { data.deletable = true; return this; }
	data.type = type;
	if (type == TYPE_NUMERIC) { data.data = new numeric; }
	return this;
}
void Node::set_type_all(const nodeDataType type)
{
	if (links.down) { links.down->set_type_all(type); }
	if (links.next) { links.next->set_type_all(type); }
	set_type(type);
}
void Node::mark_empty_delete_data()
{
	if (data.type == TYPE_NUMERIC) { delete data.data_numeric; }
	if (data.type == TYPE_SYMBOL) { delete data.data_symbol; }
	if (data.type == TYPE_EX) { delete data.data_ex; }
	data.type = TYPE_EMPTY;
}

ostringstream & Node::get_this_data_str(ostringstream & s)
{
	if (data.type == TYPE_CMD) {
		s << NativeCMD::get_cmd_str(data.cmd);
		return s;
	}
	// if (data.type == TYPE_NUMERIC) { s << dflt << *(numeric *) data.data; return s; }
	if (data.type == TYPE_NUMERIC) { s << dflt << *data.data_numeric; return s; }
	if (data.type == TYPE_DELIM) {
		s << NativeCMD::get_cmd_str(data.cmd);
		return s;
	}
	s << ERROR_MSG; return s;
}
ostringstream & Node::get_data_str(ostringstream & s)
{
	if (data.type < TYPE_NUMERIC) { s << ERROR_MSG; return s; }
	get_this_data_str(s);
	if (data.type == TYPE_CMD)
	{
		s << "(";
		if (links.down)
		{
			Node * next_op = links.down;
			next_op->get_data_str(s);
			while(next_op->links.next)
			{
				next_op = next_op->links.next;
				s << ","; next_op->get_data_str(s);
			}
		}
		s << ")";
	}
	return s;
}
string Node::get_this_data_str()
{
	ostringstream s;
	return get_this_data_str(s).str();
}
string Node::get_data_str()
{
	ostringstream s;
	return get_data_str(s).str();
}

Node * inject_fn (Node * result, Node * op, cmdSymbol cmd)
{
	if (cmd == CMD_PLUS) { inject_addto(result, op); }
	if (cmd == CMD_MINUS) { inject_minusto(result, op); }
	// if (cmd == CMD_TIMES) { *result = *result * *op; }
	// if (cmd == CMD_DIVIDE) { *result = *result / *op; }
	return result;
}

//inject: interesting FP concept that applies a function to an accumulator, and each element in a list, and returns the accumulator
Node * Node::inject_to(Node * result)
{
	if (data.type!=TYPE_CMD || links.down == nullptr) { return this; }
	Node * next_op {links.down};
	*result = *next_op;
	cmdSymbol this_cmd { data.cmd_symbol };
	while ((next_op = next_op->links.next)) {
		if (next_op->data.type == TYPE_CMD)
		{
			Node int_result;
			next_op->inject_to(&int_result);
		} else {
			inject_fn(result, next_op, this_cmd);
		}
	}
	return result;
}

inline bool compare_types_to (nodeDataType cmp, nodeDataType t1, nodeDataType t2, nodeDataType t3)
{
	return (t1 == cmp || t2 == cmp || t3 == cmp);
}

// TODO: there needs to be a case in here for when there are multiple symbols to set type to ex
nodeDataType Node::get_eval_type()
{
	if (!links.down) { return data.type; }
	nodeDataType down_type { links.down->get_eval_type() };
	nodeDataType next_type;
	if (links.next) { next_type = links.next->get_eval_type(); } else { next_type = TYPE_EMPTY; } 
	if (compare_types_to(TYPE_ERROR, data.type, down_type, next_type)) { return TYPE_ERROR; }
	if (compare_types_to(TYPE_EX, data.type, down_type, next_type)) { return TYPE_EX; }
	if (compare_types_to(TYPE_NUMERIC, data.type, down_type, next_type)) { return TYPE_NUMERIC; }
	if (compare_types_to(TYPE_SYMBOL, data.type, down_type, next_type)) { return TYPE_SYMBOL; }
	if (compare_types_to(TYPE_HOLD, data.type, down_type, next_type)) { return TYPE_HOLD; }
	if (compare_types_to(TYPE_DELIM, data.type, down_type, next_type)) { return TYPE_DELIM; }
	if (compare_types_to(TYPE_CMD, data.type, down_type, next_type)) { return TYPE_CMD; }
	if (compare_types_to(TYPE_EMPTY, data.type, down_type, next_type)) { return TYPE_EMPTY; }
	return TYPE_ERROR;
}
Node * Node::eval(NodeContainer * c)
{
	if (data.type != TYPE_CMD || links.down == nullptr) { return this; }
	return data.cmd(this, c);
}

NodeContainer::NodeContainer (cunt size) { initialize(size); }
NodeContainer::~NodeContainer() { delete[] container; }

void NodeContainer::initialize (cunt size)
{
	free_space_remaining = size;
	container_size = size;
	container = new Node[size];
	startptr = container;
	set_cut_card(startptr);
}
bool NodeContainer::set_cut_card (Node * position)
{
	if (position - container > container_size) { return false; }
	position->data.type = TYPE_CUTCARD;
	return true;
}

Node * NodeContainer::reserve_node()
{
	if (startptr == nullptr || free_space_remaining < 1) { return nullptr; }
	Node * reserved_node = startptr;
	if (startptr->data.type == TYPE_CUTCARD)
	{
		if (!set_cut_card(++startptr)) { startptr = nullptr; }
	} else {
		startptr = startptr->links.next;
	}
	reserved_node->set_links_null();
	--free_space_remaining;
	return reserved_node;
}
Node * NodeContainer::reserve_node(numeric * number)
{
	Node * n = reserve_node();
	n->data.data = number;
	n->set_type(TYPE_NUMERIC);
	return n;
}
void NodeContainer::delete_all_from_root(Node * root_node)
{
	if(root_node->links.down) { delete_all_from_root(root_node->links.down); }
	if(root_node->links.next) { delete_all_from_root(root_node->links.next); }
	root_node->mark_empty_delete_data();
	root_node->links.next = startptr;
	startptr = root_node;
	++free_space_remaining;
}