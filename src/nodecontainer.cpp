#include "calc.hpp"
using namespace std;
using namespace GiNaC;

Node::Node() { set_links_null(); }
void Node::set_links_null()
{
	links.down = nullptr;
	links.up = nullptr;
	links.next = nullptr;
	links.prev = nullptr;
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

string Node::get_this_data_str()
{
	ostringstream s;
	switch (data.type)
	{
		case TYPE_CMD:
			return "plus";
			break;
		case TYPE_NUMERIC:
			s << dflt << *(numeric *) data.data;
			return s.str();
			break;
		default:
			return "error";
	}
}
string Node::get_data_str()
{
	if (data.type < TYPE_NUMERIC) { return ERROR_MSG; }
	string acc = get_this_data_str();
	if (data.type == TYPE_CMD)
	{
		Node * next_op = links.down;
		acc += "(" + next_op->get_data_str();
		while(next_op->links.next)
		{
			next_op = next_op->links.next;
			acc += "," + next_op->get_data_str();
		}
		acc += ")";
	}
	return acc;
}

NodeContainer::NodeContainer (cuint size) { initialize(size); }
NodeContainer::~NodeContainer() { delete[] container; }

void NodeContainer::initialize (cuint size)
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
	--free_space_remaining;
	return reserved_node;
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