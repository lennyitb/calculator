#include "calc.hpp"
#include "cmdlookup.hpp"

// i previously had the idea to use function pointers as symbols to represent commands and delims
// i thought that was neat because nodes kinda just evaluate their own data
// the syntax is ridiculous. the standards compliance is dubious. the performance is questionable.
// the compiler has no fuck*** clue what i'm trying to do
// most of this is already depricated and will soon be removed

using namespace std;
using namespace GiNaC;
using namespace CMDLookup;

namespace NativeCMD
{
	string get_cmd_str (Node * (*cmd)(Node *, NodeContainer *)) // deprecated and will soon be removed
	{
		for (unsigned int i = 0; i < command_list.size(); ++i)
		{
			if (command_list[i].cmd == cmd) { return command_list[i].str; }
		}
		return ERROR_MSG;
	}
	string get_cmd_str (cmdSymbol cmd)
	{
		for (unsigned int i = 0; i < command_list.size(); ++i)
		{
			if (command_list[i].cmd_symbol == cmd) { return command_list[i].str; }
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
	cmdSymbol get_cmd_symbol (const string & s)
	{
		for (unsigned int i = 0; i < command_list.size(); ++i)
		{
			if (command_list[i].str == s) { return command_list[i].cmd_symbol; }
		}
		return CMD_NULL;
	}

	template<class T> inline T inject_plus (T * op1, T * op2)
	{
		return T(*op1 + *op2);
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
	Node * cmd_inject (Node * n, NodeContainer * c, cmdSymbol * cmd)
	{
		// nodeDataType ret_type {get_return_type(n)};
		n->set_deletable(true);
		n = n->links.down;
		// Node * acc/*umulator*/ {c->reserve_node()->set_type(ret_type)};
		return n; //lint
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

	template <class T> Node * divide_itr (Node * result, Node * n, NodeContainer * c)
	{
		result->set_numeric(new numeric);
		n = n->links.down;
		if (n->data.type == TYPE_CMD) { n = n->eval(c); }
		*(T *) result->data.data = *(T *) n->data.data;
		n->set_deletable(true);
		n = n->links.down->links.next;
		while (n)
		{
			if (n->data.type == TYPE_CMD) { n = n->eval(c); }
			*(T *) result->data.data /= *(T *) n->data.data;
			n->set_deletable(true);
			n = n->links.next;
		}
		return result;
	}
	Node * divide (Node * n, NodeContainer * c) //copilot
	{
		Node * result = c->reserve_node();
		nodeDataType ret_type = get_return_type(n);
		if (ret_type == TYPE_NUMERIC)
			{ return divide_itr<numeric>(result, n, c); }
		else
			{ return divide_itr<ex>(result, n, c); }
	}
}
