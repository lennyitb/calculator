#include "calc.hpp"

Stack::Stack(cuint size) : min_free_space(size / 10) {container.initialize(size); }

Node * Stack::get_level_ref(cuint level)
{
	if (root_node_list.size() < level) {return nullptr;}
	return *(root_node_list.end() - level);
}

Node * Stack::reserve_node() { return container.reserve_node(); }