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

enum nodeDataType { TYPE_EMPTY, TYPE_DELETABLE, TYPE_CUTCARD, TYPE_ERROR, TYPE_NUMERIC, TYPE_SYMBOL, TYPE_EX, TYPE_HOLD, TYPE_CMD, TYPE_DELIM, TYPE_LIST, TYPE_SEQ };
enum cmdSymbol { CMD_NULL, CMD_PLUS, CMD_MINUS, CMD_TIMES, CMD_DIVIDE };
enum delimType { DELIM_NULL, DELIM_OPEN_PAREN, DELIM_OPEN_CURLY, DELIM_OPEN_SQUARE, DELIM_OPEN_ANGLE, DELIM_CLOSE_PAREN, DELIM_CLOSE_CURLY, DELIM_CLOSE_SQUARE, DELIM_CLOSE_ANGLE };

struct NodeData
{
	nodeDataType type;
	bool deletable {false};
	union {
		void * data;
		Node * (*cmd)(Node *, NodeContainer *);
		delimType delim;
		GiNaC::numeric * data_numeric;
		GiNaC::symbol * data_symbol;
		GiNaC::ex * data_ex;
	};
	cmdSymbol cmd_symbol;
};
class Node
{
	// friend class NodeContainer;

	std::string get_this_data_str ();
	std::ostringstream & get_this_data_str (std::ostringstream & s);
	// Node * inject_fn (Node * result, Node * op, cmdSymbol cmd);
public:
	Node();
	Node(GiNaC::numeric * number);
	Node(Node & old_node);
	// Node operator= (Node & old_node);
	~Node();
	NodeLinks links;
	NodeData data;

	Node * copy(Node * old_node);

	void set_links_null();
	nodeDataType get_type();
	inline void set_deletable(bool state) { data.deletable = state; }

	Node * set_numeric (GiNaC::numeric * number);
	Node * set_cmd (Node * (*cmd)(Node *, NodeContainer *));
	Node * set_delim (const std::string & delim);
	Node * set_symbol (const std::string & name);
	Node * set_ex (GiNaC::ex * expr);

	Node * set_type (const nodeDataType type);
	void set_type_all (const nodeDataType type);

	void mark_empty_delete_data();

	std::string get_data_str();
	std::ostringstream & get_data_str (std::ostringstream & s);

	Node * inject_to (Node * result);

	nodeDataType get_eval_type();
	Node * eval(NodeContainer * c);
};

// I'm kind of pround of this. It's a container of nodes linked in a tree, but the empty nodees are all forward linked to eachother;
// making it a very fast operation to find an empty node, or delete a node.
// A key feature is that it makes the last empty node a 'cut card', like in a shoe blackjack game.
// When the cut card is reached, the deck is reshuffled. In this case, the cut card indicates that every node following is empty.
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