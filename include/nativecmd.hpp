#ifndef NATIVECMD_H
#define NATIVECMD_H

typedef Node * (*native_cmd)(Node *, NodeContainer *);

namespace NativeCMD
{
	std::string get_cmd_str (Node * (*cmd)(Node *, NodeContainer *));
	native_cmd get_cmd_ptr (const std::string & s);
	cmdSymbol get_cmd_symbol (const std::string & s);

	Node * cmd_inject (Node * n, NodeContainer * c, cmdSymbol * cmd);

	Node * open_delim_curly (Node * n, NodeContainer * c);
	Node * open_delim_square (Node * n, NodeContainer * c);
	Node * open_delim_round (Node * n, NodeContainer * c);
	Node * open_delim_angle (Node * n, NodeContainer * c);

	Node * plus (Node * n, NodeContainer * c);
	Node * minus (Node * n, NodeContainer * c);
	Node * times (Node * n, NodeContainer * c);
	Node * divide (Node * n, NodeContainer * c);
}

#endif