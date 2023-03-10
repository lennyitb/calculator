#include "calc.hpp"
using namespace std;
using namespace GiNaC;
using namespace NodeInjectOperator;

Node::Node() { set_links_null(); }
Node::Node(numeric * number) { set_numeric(number); set_links_null(); }
Node::Node(Node & old_node)
{
	links = old_node.links;
	data = old_node.data;
	if (old_node.data.type == TYPE_NUMERIC) { data.data_numeric = new numeric {*old_node.data.data_numeric}; }
	else if (old_node.data.type == TYPE_SYMBOL) { data.data_symbol_record = old_node.data.data_symbol_record; }
	else if (old_node.data.type == TYPE_EX) { data.data_ex = new ex {*old_node.data.data_ex}; }
}
Node::~Node()
{
	if (data.type == TYPE_NUMERIC) { delete data.data_numeric; }
	// else if (data.type == TYPE_SYMBOL) { delete data.data_symbol; }
	else if (data.type == TYPE_EX) { delete data.data_ex; }
}
void Node::set_links_null()
{
	links.down = nullptr;
	links.up = nullptr;
	links.next = nullptr;
	links.prev = nullptr;
}

Node * Node::copy (Node * old_node)
{
	links = old_node->links;
	data = old_node->data;
	if (old_node->data.type == TYPE_NUMERIC) { data.data_numeric = new numeric {*old_node->data.data_numeric}; }
	else if (old_node->data.type == TYPE_SYMBOL) { data.data_symbol_record = old_node->data.data_symbol_record; }
	else if (old_node->data.type == TYPE_EX) { data.data_ex = new ex {*old_node->data.data_ex}; }
	return this;
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
Node * Node::set_cmd(cmdSymbol cmd)
{
	data.type = TYPE_CMD;
	data.cmd_symbol = cmd;
	return this;
}

inline delimType delim_lookup(const string & delim)
{
	if (delim == "(") { return DELIM_OPEN_PAREN; }
	if (delim == ")") { return DELIM_CLOSE_PAREN; }
	if (delim == "{") { return DELIM_OPEN_CURLY; }
	if (delim == "}") { return DELIM_CLOSE_CURLY; }
	if (delim == "[") { return DELIM_OPEN_SQUARE; }
	if (delim == "]") { return DELIM_CLOSE_SQUARE; }
	if (delim == "<<") { return DELIM_OPEN_ANGLE; }
	if (delim == ">>") { return DELIM_CLOSE_ANGLE; }
	return DELIM_NULL;
}
inline const string delim_lookup(const delimType delim)
{
	if (delim == DELIM_OPEN_PAREN) { return "("; }
	if (delim == DELIM_CLOSE_PAREN) { return ")"; }
	if (delim == DELIM_OPEN_CURLY) { return "{"; }
	if (delim == DELIM_CLOSE_CURLY) { return "}"; }
	if (delim == DELIM_OPEN_SQUARE) { return "["; }
	if (delim == DELIM_CLOSE_SQUARE) { return "]"; }
	if (delim == DELIM_OPEN_ANGLE) { return "<<"; }
	if (delim == DELIM_CLOSE_ANGLE) { return ">>"; }
	return ERROR_MSG;
}
Node * Node::set_delim(const string & delim)
{
	data.type = TYPE_DELIM;
	data.delim = delim_lookup(delim);
	return this;
}
Node * Node::set_symbol(const string & name)
//get rid of this. it's wrong. symbols are now managed by symboltable in stack or maybe runtime
{
	data.type = TYPE_SYMBOL;
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
	if (type == TYPE_SYMBOL) { data.data = new symbol; }
	if (type == TYPE_EX) { data.data = new ex; }
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
	// symbol allocation is now managed by Runtime class.
	// if (data.type == TYPE_SYMBOL) { delete data.data_symbol; }
	if (data.type == TYPE_EX) { delete data.data_ex; }
	data.type = TYPE_EMPTY;
}

ostringstream & Node::get_this_data_str(ostringstream & s)
{
	if (data.type == TYPE_CMD) {
		s << NativeCMD::get_cmd_str(data.cmd_symbol);
		return s;
	}
	// if (data.type == TYPE_NUMERIC) { s << dflt << *(numeric *) data.data; return s; }
	if (data.type == TYPE_NUMERIC) { s << dflt << *data.data_numeric; return s; }
	if (data.type == TYPE_DELIM) {
		s << delim_lookup(data.delim);
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
	if (cmd == CMD_TIMES) { inject_timesto(result, op); }
	if (cmd == CMD_DIVIDE) { inject_divideto(result, op); }
	return result;
}

//inject: interesting FP concept that applies a function to an accumulator, and each element in a list, then returns the accumulator
Node * Node::inject_to(Node * result)
//current headache is that result isn't being set to the first operand
//i need to find a way to set result to the first element evaluation
{
	// if it's not a command it's a leaf, and is considered already evaluated
	// but it needs to be copied into result for memory management
	if (data.type!=TYPE_CMD || links.down == nullptr)
	{
		result->copy(this);
		set_deletable(true);
		return result;
	}
	Node * next_op { links.down };
	nodeDataType result_type { get_eval_type() };
	result->set_type(result_type);
	//evaluate first operand, then set result to that
	next_op->inject_to(result);
	//iterate over every other operand, evaluating it and applying the function to the result
	while ((next_op = next_op->links.next)) {
		if (next_op->data.type == TYPE_CMD)
		{
			Node int_result;
			int_result.set_type(result_type);
			next_op->inject_to(&int_result);
			inject_fn(result, &int_result, data.cmd_symbol);
		} else {
			inject_fn(result, next_op, data.cmd_symbol);
		}
	}
	return result;
}

Node * Node::new_inject_to (Node * result)
{
	// copy self into result and quit if i'm not a command
	if (data.type!=TYPE_CMD || links.down == nullptr)
	{
		result->copy(this);
		set_deletable(true);
		return result;
	}
	nodeDataType result_type { get_eval_type() };
	result->set_type(result_type);
	// go down and walk over every next to see if there's a command
	// set result to down->inject_to()
	Node * next_op { links.down };
	next_op->inject_to(result);
	// call inject_fn on result and every down->next
	// while ((next_op = next_op->links.next))
	// {

	// }
	return result;
}

inline bool compare_types_to (nodeDataType cmp, nodeDataType t1, nodeDataType t2, nodeDataType t3)
{
	return (t1 == cmp || t2 == cmp || t3 == cmp);
}

nodeDataType Node::get_eval_type()
{
	if (!links.down) { return data.type; }
	nodeDataType down_type { links.down->get_eval_type() };
	nodeDataType next_type;
	if (links.next) { next_type = links.next->get_eval_type(); } else { next_type = TYPE_EMPTY; }

	if (compare_types_to(TYPE_ERROR, data.type, down_type, next_type)) { return TYPE_ERROR; }
	// multiple symbols should return an ex
	if (compare_types_to(TYPE_EX, data.type, down_type, next_type) || (next_type == TYPE_SYMBOL && down_type == TYPE_SYMBOL)) { return TYPE_EX; }
	if (compare_types_to(TYPE_NUMERIC, data.type, down_type, next_type)) { return TYPE_NUMERIC; }
	if (compare_types_to(TYPE_SYMBOL, data.type, down_type, next_type)) { return TYPE_SYMBOL; }
	if (compare_types_to(TYPE_HOLD, data.type, down_type, next_type)) { return TYPE_HOLD; }
	if (compare_types_to(TYPE_DELIM, data.type, down_type, next_type)) { return TYPE_DELIM; }
	if (compare_types_to(TYPE_CMD, data.type, down_type, next_type)) { return TYPE_CMD; }
	if (compare_types_to(TYPE_EMPTY, data.type, down_type, next_type)) { return TYPE_EMPTY; }

	return TYPE_ERROR;
}
// deprecated
Node * Node::eval(NodeContainer * c)
{
	if (data.type != TYPE_CMD || links.down == nullptr) { return this; }
	return data.cmd(this, c);
}

NodeContainer::NodeContainer (cunt size) { initialize(size); }
NodeContainer::~NodeContainer()
{
	// // // OKAY on second thought I already have a Node deconstructor defined so I will omit this pending evidence nodes aren't being deleted properly
	// // this is a pretty low priority function anywho as a nodecontainer should exist for the entire lifecycle of the program in the typical use case

	// // free all pointers stored by nodes within the container
	// for (Node * i = container; i < container + container_size; ++i)
	// {
	// 	if (i->get_type() == TYPE_CUTCARD) { break; } //assume there will be no data after the cutcard is encountered
	// 	i->mark_empty_delete_data(); //it's actually unnecessary to mark the nodes empty but I don't have a method to only delete the data
	// }

	// //finally free the container itself
	delete[] container;
}

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
	//seems like a silly check but the code will actually attempt to push the cutcard past the end of the container
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
