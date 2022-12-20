#include "calc.hpp"
using namespace GiNaC;

Stack::Stack(cunt size) : min_free_space(size / 10) {container.initialize(size); }

Node * Stack::get_level_ref(cunt level)
{
	if (root_node_list.size() < level)
		{return nullptr;}
	return *(root_node_list.end() - level);
}

HangingOpenDelim Stack::get_next_hanging_open_delim()
{
	char c[2];
	for (unsigned int i = root_node_list.size(); i > 0; --i)
	{
		if (get_level_ref(i)->data.type == TYPE_DELIM)
		{
			*c = *NativeCMD::get_cmd_str(get_level_ref(i)->data.data).c_str();
			HangingOpenDelim d {c, i};
			return d;
		}
	}
	HangingOpenDelim d; return d;
}

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