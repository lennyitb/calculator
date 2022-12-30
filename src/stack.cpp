#include "calc.hpp"
using namespace std;
using namespace GiNaC;

regex unsigned_int_rgx("^[0-9]+$");

Stack::Stack(cunt size) : min_free_space(size / 10) {container.initialize(size); }

Node * Stack::get_level_ref(cunt level)
{
	if (root_node_list.size() < level)
		{return nullptr;}
	return *(root_node_list.end() - level);
}

HangingOpenDelim Stack::get_next_hanging_open_delim()
{
	for (unsigned int i = 1; i <= root_node_list.size(); ++i)
	{
		if (get_level_ref(i)->data.type == TYPE_DELIM)
		{
			HangingOpenDelim d {get_level_ref(i)->data.delim, i};
			return d;
		}
	}
	HangingOpenDelim d; return d;
}
// // not getting rid of this commented code. i really want for it to exist
// SymbolRecord * Stack::get_or_register_symbol(string & symbolname)
// {
// 	for (list<SymbolRecord>::iterator i = symbol_table.begin(); i != symbol_table.end(); ++i)
// 	{
// 		if (i->name == symbolname)
// 		{
// 			return &*i;
// 		}
// 	}
// 	symbol s{"x"};
// 	s.

// }

Node * Stack::reserve_node() { return container.reserve_node(); }
Node * Stack::reserve_node(numeric * number) { return container.reserve_node(number); }
void Stack::delete_all_from_root(Node * n) { container.delete_all_from_root(n); }

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
Node * Stack::take_at(cunt level) //not working
{
	Node * n = get_level_ref(level);
	root_node_list.erase(root_node_list.end() - level);
	// n->set_type_all(TYPE_DELETABLE);
	return n;
}
// void Stack::drop()
// {
// 	root_node_list.back()->set_type_all(TYPE_DELETABLE);
// 	root_node_list.pop_back();
// }

void Stack::record_history()
{
	history_table.push_back(root_node_list);
}

void Stack::eval()
{
	Node * level = root_node_list.back(); //record root node of level 1
	// Node * result = level->eval(&container);
	Node * result = reserve_node();
	level->inject_to(result); //apply inject function to level and use result as an accumulator
	if (level != result) //replace level with result but only go to the trouble if they're different
	{
		root_node_list.back() = result;
		level->set_type_all(TYPE_DELETABLE);
	} else {
		container.delete_all_from_root(result);
	}
}

bool Stack::swap()
{
	if (depth() < 2) { return false; }
	record_history();
	vector<Node *>::iterator it { (root_node_list.end() - 2) };
	Node * n { *it };
	root_node_list.erase(it);
	root_node_list.push_back(n);
	return true;
}
bool Stack::drop()
{
	if (depth() < 1) { return false; }
	record_history();
	root_node_list.back()->set_type_all(TYPE_DELETABLE);
	root_node_list.pop_back();
	return true;
}
cunt Stack::get_cunt()
{
	const string num_s { take()->get_data_str() };
	if (regex_match(num_s, unsigned_int_rgx)) {
		return stoi(num_s);
	}	return 0;
}
bool Stack::rolln()
{
	if (depth() < 1) { return false; }
	cunt my_cunt = get_cunt();
	if (depth() < my_cunt) { return false; }
	record_history();
	Node * n { root_node_list.back() };
	n->set_type_all(TYPE_DELETABLE);
	root_node_list.pop_back();
	root_node_list.insert(root_node_list.end() - my_cunt, n);
	return true;
}
bool Stack::unrolln()
{
	if (depth() < 1) { return false; }
	cunt my_cunt = get_cunt();
	if (depth() < my_cunt) { return false; }
	record_history();
	vector<Node *>::iterator it { root_node_list.end() - my_cunt };
	Node * n { *it };
	n->set_type_all(TYPE_DELETABLE);
	root_node_list.erase(it);
	root_node_list.push_back(n);
	return true;
}