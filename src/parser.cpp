#include "calc.hpp"
using namespace std;
using namespace GiNaC;

namespace Parser
{
	Runtime * runtime_ptr = nullptr;
	std::regex numeric_rgx("^[+-]?([0-9]*[.])?[0-9]+([eE][+-]?[0-9]+)?$");
	std::regex cmd_rgx("^[a-zA-Z]+$");
	std::regex open_delim_rgx("^(\\(|\\[|\\{|<<)$");
	std::regex explicit_symbol_rgx("^'[a-zA-Z]+'?$");
	std::regex unsigned_int_rgx("^[0-9]+$");

	Node * collect_cmd_ops (Node * n, Stack & stack)
	{
		HangingOpenDelim d = stack.get_next_hanging_open_delim();
		if (d.d_type == DELIM_OPEN_PAREN)
		{
			stack.take_at(d.level)->set_type(TYPE_DELETABLE);
			unsigned int i = d.level - 1;
			if (i < 1) { return n; }
			Node * prev_op = stack.take_at(i);
			n->links.down = prev_op;
			--i;
			Node * current_op;
			while (i > 0)
			{
				current_op = stack.take_at(i);
				prev_op->links.next = current_op;
				prev_op = current_op;
				--i;
			}
		} else {
			for (unsigned int i = 1; i != 3; ++i)
				{ if (stack.get_level_ref(i)->data.type == TYPE_DELIM) { return nullptr; } }
			if (stack.depth() < 2) { return n; }
			n->links.down = stack.get_level_ref(2);
			n->links.down->links.next = stack.take();
			stack.take();
		}
		return n;
	}
	// // we're phasing out the whole function pointer as a symbol thing.
	// bool parse_native_cmd_to(const string & s, Stack & stack)
	// {
	// 	//this syntax is really starting to get on my nerves.
	// 	Node *(*ptr)(Node *, NodeContainer*){NativeCMD::get_cmd_ptr(s)};
	// 	if (ptr == nullptr) { return false; }
	// 	Node * n = stack.reserve_node()->set_cmd(ptr);
	// 	n->data.cmd_symbol = NativeCMD::get_cmd_symbol(s);
	// 	collect_cmd_ops(n, stack);
	// 	if (!n->links.down) { return false; }
	// 	stack.push_node(n);
	// 	return true;
	// }
	bool parse_native_cmd_to(const string & s, Stack & stack)
	{
		cmdSymbol cmd_symbol = NativeCMD::get_cmd_symbol(s);
		if (cmd_symbol == CMD_NULL) { return false; }
		Node * n = stack.reserve_node()->set_cmd(cmd_symbol);
		collect_cmd_ops(n, stack);
		if (!n->links.down) { stack.delete_all_from_root(n); return false; }
		stack.push_node(n);
		return true;
	}
	

	bool parse_word_to(const string & s, Stack & stack)
	{
		if (s == "eval")
		{
			stack.eval();
			return true;
		}

		if(regex_match(s,numeric_rgx))
		{
			stack.push_new()->set_numeric(new numeric {s.c_str()});
			return true;
		}
		if (s == "swap") { stack.swap(); return true; }
		if (s == "drop") { stack.drop(); return true; }
		if (s == "rolln") { stack.rolln(); return true; }
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
	void set_runtime(Runtime & r) { runtime_ptr = &r; }
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