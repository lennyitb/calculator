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

// struct SymbolRecord
// {
// 	std::string name;
// 	union realOrComplex {
// 		GiNaC::realsymbol value;
// 		GiNaC::symbol cvalue;
// 	} value;
// 	enum symbolType {REAL, COMPLEX} type;
// 	SymbolRecord(std::string s, realOrComplex v, symbolType st) : name(name), value(v), type(st) {}
// };

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

	void eval();
};

#endif