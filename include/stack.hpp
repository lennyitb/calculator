#ifndef STACK_H
#define STACK_H

struct HangingOpenDelim
{
	const delimType d_type;
	const unsigned int level;
	HangingOpenDelim() : d_type {DELIM_NULL}, level {0} {}
	HangingOpenDelim(const delimType d, const unsigned int l) : d_type {d}, level {l} {}
	
	//TODO change this to operator bool
	inline bool does_exist() { return level ? true : false; }
};



class Stack
{
	NodeContainer container;
	std::vector<Node *> root_node_list;
	std::vector<std::vector<Node *>> history_table;	
	// std::list<SymbolRecord> symbol_table;
public:
	Stack(cunt size);

	inline unsigned int depth () { return root_node_list.size(); }
	Node * get_level_ref (cunt level);

	// SymbolRecord * get_or_register_symbol (string & symbolname);
	// HangingOpenDelim get_next_hanging_open_delim_s_type ();
	HangingOpenDelim get_next_hanging_open_delim();

	Node * reserve_node();
	Node * reserve_node(GiNaC::numeric * number);
	void delete_all_from_root(Node * n);
	inline void push_node (Node * node) { root_node_list.push_back(node); }

	Node * push_new();

	Node * take();
	Node * take_at(cunt level); //not working

	unsigned int min_free_space;

	void record_history();

	void eval();


	bool swap();
	bool drop();
	//TODO make these return some kind of error later if n is out of bounds
	cunt get_cunt();
	bool rolln();
	bool unrolln();
};

#endif