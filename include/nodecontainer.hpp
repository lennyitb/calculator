#ifndef NODECONTAINER_H
#define NODECONTAINER_H

class Node; struct NodeLinks
{
	Node * up;
	Node * down;
	Node * prev;
	Node * next;
};

enum nodeDataType { TYPE_EMPTY, TYPE_DELETABLE, TYPE_CUTCARD, TYPE_NUMERIC, TYPE_SYMBOL, TYPE_CMD };

struct NodeData
{
	nodeDataType type;
	bool deletable;
	void * data;
};
class Node
{
	// friend class NodeContainer;

	std::string get_this_data_str ();
	std::ostringstream & get_this_data_str (std::ostringstream & s);
public:
	Node();
	NodeLinks links;
	NodeData data;
	
	void set_links_null();
	nodeDataType get_type();
	inline void set_deletable(bool state) { data.deletable = state; }
	void set_type (const nodeDataType type);
	void set_type_all (const nodeDataType type);

	void mark_empty_delete_data();

	std::string get_data_str();
	std::ostringstream & get_data_str (std::ostringstream & s);
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
	void delete_all_from_root (Node * root_node);
};

#endif