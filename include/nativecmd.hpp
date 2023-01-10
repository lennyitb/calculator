#ifndef NATIVECMD_H
#define NATIVECMD_H

// i previously had the idea to use function pointers as symbols to represent commands and delims
// i thought that was neat because nodes kinda just evaluate their own data
// the syntax is ridiculous. the standards compliance is dubious. the performance is questionable.
// most of this is already deprecated and will soon be removed

typedef Node * (*native_cmd)(Node *, NodeContainer *);

namespace NativeCMD
{
	std::string get_cmd_str (Node * (*cmd)(Node *, NodeContainer *)); //deprecated and will soon be removed
	std::string get_cmd_str (cmdSymbol cmd);
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