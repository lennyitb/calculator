#ifndef NODECONTAINER_H
#define NODECONTAINER_H

class Node;
class NodeContainer;
struct NodeLinks
{
	Node * up;
	Node * down;
	Node * prev;
	Node * next;
};

enum nodeDataType { TYPE_EMPTY, TYPE_DELETABLE, TYPE_CUTCARD, TYPE_NUMERIC, TYPE_SYMBOL, TYPE_EX, TYPE_HOLD, TYPE_CMD, TYPE_DELIM };

struct NodeData
{
	nodeDataType type;
	bool deletable {false};
	union {
		void * data;
		Node * (*cmd)(Node *, NodeContainer *);
	};
};
class Node
{
	// friend class NodeContainer;

	std::string get_this_data_str ();
	std::ostringstream & get_this_data_str (std::ostringstream & s);
public:
	Node();
	Node(GiNaC::numeric * number);
	NodeLinks links;
	NodeData data;

	NodeContainer * get_container();
	
	void set_links_null();
	nodeDataType get_type();
	inline void set_deletable(bool state) { data.deletable = state; }

	Node * set_numeric (GiNaC::numeric * number);
	Node * set_cmd (Node * (*cmd)(Node *, NodeContainer *));
	Node * set_delim (const std::string & delim);
	Node * set_symbol (const std::string & name);
	Node * set_ex (GiNaC::ex * expr);

	void set_type (const nodeDataType type);
	void set_type_all (const nodeDataType type);

	void mark_empty_delete_data();

	std::string get_data_str();
	std::ostringstream & get_data_str (std::ostringstream & s);

	Node * eval(NodeContainer * c);
};

class NodeContainer
{
	Node * container;
	Node * startptr;
	unsigned int container_size;
	bool set_cut_card (Node * position);
public:
	NodeContainer () {}
	NodeContainer (cunt size);
	~NodeContainer();
	void initialize (cunt size);

	unsigned int free_space_remaining;

	Node * reserve_node ();
	Node * reserve_node (GiNaC::numeric * number);
	void delete_all_from_root (Node * root_node);
};

#endif