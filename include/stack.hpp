#ifndef STACK_H
#define STACK_H

class Stack
{
	NodeContainer container;
	std::vector<Node *> root_node_list;

public:
	Stack(cunt size);

	inline unsigned int depth () { return root_node_list.size(); }
	Node * get_level_ref (cunt level);

	Node * reserve_node();
	inline void push_node (Node * node) { root_node_list.push_back(node); }

	unsigned int min_free_space;
};

#endif