#include "calc.hpp"
using namespace std;
using namespace GiNaC;


Stack::Stack(cunt size) : min_free_space(size / 10) {container.initialize(size); }

Node * Stack::get_level_ref(cunt level)
{
	if (root_node_list.size() < level)
		{return nullptr;}
	return *(root_node_list.end() - level);
}

// HangingOpenDelim Stack::get_next_hanging_open_delim_s_type()
// {
// 	char c[2];
// 	for (unsigned int i = root_node_list.size(); i > 0; --i)
// 	{
// 		if (get_level_ref(i)->data.type == TYPE_DELIM)
// 		{
// 			*c = *NativeCMD::get_cmd_str(get_level_ref(i)->data.cmd).c_str();
// 			HangingOpenDelim d {c, i};
// 			return d;
// 		}
// 	}
// 	HangingOpenDelim d; return d;
// }
HangingOpenDelim Stack::get_next_hanging_open_delim()
{
	for (unsigned int i = 1; i <= root_node_list.size(); ++i)
	{
		if (get_level_ref(i)->data.type == TYPE_DELIM)
		{
			HangingOpenDelim d {get_level_ref(i)->data.delim, i};
			return d;
		}
	}
	HangingOpenDelim d; return d;
}
// SymbolRecord * Stack::get_or_register_symbol(string & symbolname)
// {
// 	for (list<SymbolRecord>::iterator i = symbol_table.begin(); i != symbol_table.end(); ++i)
// 	{
// 		if (i->name == symbolname)
// 		{
// 			return &*i;
// 		}
// 	}
// 	symbol s{"x"};
// 	s.

// }

Node * Stack::reserve_node() { return container.reserve_node(); }
Node * Stack::reserve_node(numeric * number) { return container.reserve_node(number); }

Node * Stack::push_new()
{
	Node * n {container.reserve_node()};
	root_node_list.push_back(n);
	return n;
}

Node * Stack::take()
{
	Node * n = root_node_list.back();
	root_node_list.pop_back();
	return n;
}
Node * Stack::take_at(cunt level) //not working
{
	Node * n = get_level_ref(level);
	root_node_list.erase(root_node_list.end() - level);
	// n->set_type_all(TYPE_DELETABLE);
	return n;
}
void Stack::drop()
{
	root_node_list.back()->set_type_all(TYPE_DELETABLE);
	root_node_list.pop_back();
}

void Stack::record_history()
{
	history_table.push_back(root_node_list);
}

void Stack::eval()
{
	Node * level = root_node_list.back(); //record root node of level 1
	// Node * result = level->eval(&container);
	Node * result = reserve_node();
	level->inject_to(result); //apply inject function to level and use result as an accumulator
	if (level != result) //replace level with result but only go to the trouble if they're different
	{
		root_node_list.back() = result;
		level->set_type_all(TYPE_DELETABLE);
	} else {
		container.delete_all_from_root(result);
	}
}