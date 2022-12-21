#include "calc.hpp"
#include "cmdlookup.hpp"

using namespace std;
using namespace GiNaC;
using namespace CMDLookup;

namespace NativeCMD
{
	string get_cmd_str (Node * (*cmd)(Node *, NodeContainer *))
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
		nodeDataType it_type;
		while (it)
		{
			it_type = it->get_type();
			if (it_type == TYPE_EX || it_type == TYPE_SYMBOL || it_type == TYPE_HOLD) { return TYPE_EX; }
			it = it->links.next;
		}
		return ret;
	}

	Node * open_delim_curly (Node * n, NodeContainer * c) { return n; }
	Node * open_delim_square (Node * n, NodeContainer * c) { return n; }
	Node * open_delim_round (Node * n, NodeContainer * c) { return n; }
	Node * open_delim_angle (Node * n, NodeContainer * c) { return n; }
	
	template <class T> Node * plus_itr(Node * result, Node * n, NodeContainer * c)
	{
		n = n->links.down;
		while (n)
		{
			if (n->data.type == TYPE_CMD) { n = n->eval(c); }
			*(T *) result->data.data += *(T *) n->data.data;
			n->set_deletable(true);
			n = n->links.next;
		}
		return result;
	}
	Node * plus (Node * n, NodeContainer * c)
	{
		nodeDataType ret_type = get_return_type(n);
		Node * result = c->reserve_node();
		if (ret_type == TYPE_NUMERIC)
			{ result->set_numeric(new numeric); return plus_itr<numeric>(result, n, c); }
		else
			{ result->set_ex(new ex); return plus_itr<ex>(result, n, c); }
	}

	template <class T> Node * minus_itr(Node * result, Node * n, NodeContainer * c)
	{
		n = n->links.down;	
		if (n->data.type == TYPE_CMD) { n = n->eval(c); }
		*(T *) result->data.data = *(T *) n->data.data;
		n->set_deletable(true);
		n = n->links.next;
		while (n)
		{
			if (n->data.type == TYPE_CMD) { n = n->eval(c); }
			*(T *) result->data.data -= *(T *) n->data.data;
			n->set_deletable(true);
			n = n->links.next;
		}
		return result;
	}
	Node * minus (Node * n, NodeContainer * c) //copilot
	{
		nodeDataType ret_type = get_return_type(n);
		Node * result = c->reserve_node();
		if (ret_type == TYPE_NUMERIC)
			{ result->set_numeric(new numeric); return minus_itr<numeric>(result, n, c); }
		else
			{ result->set_ex(new ex); return minus_itr<ex>(result, n, c); }
	}

	template <class T> Node * times_itr(Node * result, Node * n, NodeContainer * c)
	{
		n = n->links.down;
		if (n->data.type == TYPE_CMD) { n = n->eval(c); }
		*(T *) result->data.data = *(T *) n->data.data;
		n->set_deletable(true);
		n = n->links.next;
		while (n)
		{
			if (n->data.type == TYPE_CMD) { n = n->eval(c); }
			*(T *) result->data.data *= *(T *) n->data.data;
			n->set_deletable(true);
			n = n->links.next;
		}
		return result;
	}
	Node * times (Node * n, NodeContainer * c) //copilot
	{
		nodeDataType ret_type = get_return_type(n);
		Node * result = c->reserve_node();
		if (ret_type == TYPE_NUMERIC)
			{ result->set_numeric(new numeric); return times_itr<numeric>(result, n, c); }
		else

			{ result->set_ex(new ex); return times_itr<ex>(result, n, c); }
	}

	template <class T> Node * divide_itr (Node * n, NodeContainer * c)
	{
		Node * result = c->reserve_node();
		result->set_numeric(new numeric);
		*(T *) result->data.data = *(T *) n->links.down->data.data;
		n = n->links.down->links.next;
		while (n)
		{
			*(T *) result->data.data /= *(T *) n->data.data;
			n = n->links.next;
		}
		return result;
	}
	Node * divide (Node * n, NodeContainer * c) //copilot
	{
		nodeDataType ret_type = get_return_type(n);
		if (ret_type == TYPE_NUMERIC)
			{ return divide_itr<numeric>(n, c); }
		else
			{ return divide_itr<ex>(n, c); }
	}
}