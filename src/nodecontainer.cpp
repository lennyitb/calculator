#include "calc.hpp"
using namespace std;
using namespace GiNaC;

Node::Node() {} // set_links_null(); }
Node::Node(numeric * number) { set_numeric(number); }
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
	data.data = number;
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

void Node::set_type(const nodeDataType type)
{
	if (type == data.type) { return; }
	if (type == TYPE_DELETABLE) { data.deletable = true; return; }
	data.type = type;
	if (type == TYPE_NUMERIC) { data.data = new numeric; }
}
void Node::set_type_all(const nodeDataType type)
{
	if (links.down) { links.down->set_type_all(type); }
	if (links.next) { links.next->set_type_all(type); }
	set_type(type);
}
void Node::mark_empty_delete_data()
{
	if (data.type == TYPE_NUMERIC) { delete (numeric *) data.data; }
	data.type = TYPE_EMPTY;
}

ostringstream & Node::get_this_data_str(ostringstream & s)
{
	if (data.type == TYPE_CMD) {
		s << NativeCMD::get_cmd_str(data.cmd);
		return s;
	}
	if (data.type == TYPE_NUMERIC) { s << dflt << *(numeric *) data.data; return s; }
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