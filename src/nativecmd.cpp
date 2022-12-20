#include "calc.hpp"
#include "cmdlookup.hpp"

using namespace std;
using namespace GiNaC;
using namespace Dispatcher;

namespace NativeCMD
{
	string get_cmd_str (void * cmd)
	{
		for (unsigned int i = 0; i < command_list.size(); ++i)
		{
			if (command_list[i].cmd == cmd) { return command_list[i].str; }
		}
		return ERROR_MSG;
	}
	void * get_cmd_ptr (const string & s)
	{
		for (unsigned int i = 0; i < command_list.size(); ++i)
		{
			if (command_list[i].str == s.c_str()) { return command_list[i].cmd; }
		}
		return nullptr;
	}
	
	Node * plus (Node * n)
	{
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
}