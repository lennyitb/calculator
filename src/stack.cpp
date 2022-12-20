#include "calc.hpp"
using namespace GiNaC;

Stack::Stack(cunt size) : min_free_space(size / 10) {container.initialize(size); }

Node * Stack::get_level_ref(cunt level)
{
	if (root_node_list.size() < level) {return nullptr;}
	return *(root_node_list.end() - level);
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
void Stack::drop()
{
	root_node_list.back()->set_type_all(TYPE_DELETABLE);
	root_node_list.pop_back();
}

void Stack::record_history()
{
	history_table.push_back(root_node_list);
}