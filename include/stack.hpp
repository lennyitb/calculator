#ifndef STACK_H
#define STACK_H

class Stack
{
	NodeContainer container;
	std::vector<Node *> root_node_list;
	std::vector<std::vector<Node *>> history_table;	

public:
	Stack(cunt size);

	inline unsigned int depth () { return root_node_list.size(); }
	Node * get_level_ref (cunt level);

	Node * reserve_node();
	Node * reserve_node(GiNaC::numeric * number);
	inline void push_node (Node * node) { root_node_list.push_back(node); }

	Node * push_new();

	Node * take();
	void drop();

	unsigned int min_free_space;

	void record_history();
};

#endif