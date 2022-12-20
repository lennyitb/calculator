#include "calc.hpp"
using namespace std;
using namespace GiNaC;

namespace Parser
{
	Node * collect_cmd_ops (Node * n, Stack & stack)
	{
		if (stack.depth() < 2) { return n; }
		n->links.down = stack.get_level_ref(2);
		n->links.down->links.next = stack.take();
		stack.take();
		return n;
	}
	bool parse_native_cmd_to(const string & s, Stack & stack)
	{
		if (s == "plus")
		{
			Node * n = stack.reserve_node()->set_cmd((void *) NativeCMD::plus);
			collect_cmd_ops(n, stack);
			stack.push_node(n);
			return true;
		}
		void * ptr = NativeCMD::get_cmd_ptr(s);
		if (ptr == nullptr) { return false; }
		Node * n = stack.reserve_node()->set_cmd(ptr);
		collect_cmd_ops(n, stack);
		stack.push_node(n);
		return true;
	}
	bool parse_word_to(const string & s, Stack & stack)
	{
		regex numeric_rgx("^[+-]?([0-9]*[.])?[0-9]+([eE][+-]?[0-9]+)?$");
		regex cmd_rgx("^[a-zA-Z]+$");
		regex open_delim_rgx("^(\\(|\\[|\\{|<<)$");

		if(regex_match(s,numeric_rgx))
		{
			stack.push_new()->set_numeric(new numeric {s.c_str()});
			return true;
		}
		if(regex_match(s,cmd_rgx))
		{
			return parse_native_cmd_to(s, stack);
		}
		if (regex_match(s, open_delim_rgx))
		{
			stack.push_new()->set_delim(s);
			return true;
		}
		return false;
	}
	bool parse_to(istream & is, Stack & stack)
	{
		string s;
		do {
			is >> s;
			if (!parse_word_to(s, stack)) {return false;}
		} while (is.peek() != PARSE_EOF);
		return true;
	}
}