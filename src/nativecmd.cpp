#include "calc.hpp"
#include "cmdlookup.hpp"

using namespace std;
using namespace GiNaC;
using namespace CMDLookup;

namespace NativeCMD
{
	string get_cmd_str (Node * (*cmd)(Node *))
	{
		for (unsigned int i = 0; i < command_list.size(); ++i)
		{
			if (command_list[i].cmd == cmd) { return command_list[i].str; }
		}
		return ERROR_MSG;
	}
	native_cmd get_cmd_ptr (const string & s)
	{
		for (unsigned int i = 0; i < command_list.size(); ++i)
		{
			if (command_list[i].str == s) { return command_list[i].cmd; }
		}
		return nullptr;
	}
	nodeDataType get_return_type (Node * n)
	{
		nodeDataType ret = TYPE_NUMERIC;
		Node * it = n->links.down;
		while (it)
		{
			if (it->get_type() == TYPE_SYMBOL) { ret = TYPE_SYMBOL; }
			if (it->get_type() == TYPE_EX) { return TYPE_EX; }
			it = it->links.next;
		}
		return ret;
	}

	Node * open_delim_curly (Node * n) { return n; }
	Node * open_delim_square (Node * n) { return n; }
	Node * open_delim_round (Node * n) { return n; }
	Node * open_delim_angle (Node * n) { return n; }
	
	Node * new_plus (Nade * n)
	{
		nodeDataType ret_type = get_return_type(n);
	}
	
	Node * plus (Node * n)
	{
		// nodeDataType ret_type = get_return_type(n);
		n = n->links.down;
		numeric * result {new numeric {*(numeric *) n->data.data}};
		while (n)
		{
			*result += *(numeric *) n->data.data;
			n = n->links.next;
		}
		return new Node {result};
	}
	Node * minus (Node * n) //copilot
	{
		n = n->links.down;
		numeric * result {new numeric {*(numeric *) n->data.data}};
		while (n)
		{
			*result -= *(numeric *) n->data.data;
			n = n->links.next;
		}
		return new Node {result};
	}
	Node * times (Node * n) //copilot
	{
		n = n->links.down;
		numeric * result {new numeric {*(numeric *) n->data.data}};
		while (n)
		{
			*result *= *(numeric *) n->data.data;
			n = n->links.next;
		}
		return new Node {result};
	}
}