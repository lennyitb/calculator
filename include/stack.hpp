#ifndef STACK_H
#define STACK_H

struct HangingOpenDelim
{
	const char delim[2];
	const unsigned int level;
	HangingOpenDelim() : delim {'\0', '\0'}, level {0} {}
	HangingOpenDelim(const char * d, unsigned int l) : delim {d[0], d[1]}, level {l} {}
	inline bool does_exist() { return level ? true : false; }
};
class Stack
{
	NodeContainer container;
	std::vector<Node *> root_node_list;
	std::vector<std::vector<Node *>> history_table;	

public:
	Stack(cunt size);

	inline unsigned int depth () { return root_node_list.size(); }
	Node * get_level_ref (cunt level);

	HangingOpenDelim get_next_hanging_open_delim ();

	Node * reserve_node();
	Node * reserve_node(GiNaC::numeric * number);
	inline void push_node (Node * node) { root_node_list.push_back(node); }

	Node * push_new();

	Node * take();
	Node * take_at(cunt level); //not working
	void drop();

	unsigned int min_free_space;

	void record_history();
};

#endif