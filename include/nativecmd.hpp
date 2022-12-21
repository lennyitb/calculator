#ifndef NATIVECMD_H
#define NATIVECMD_H

typedef Node * (*native_cmd)(Node *);

namespace NativeCMD
{
	std::string get_cmd_str (Node * (*cmd)(Node *));
	native_cmd get_cmd_ptr (const std::string & s);

	Node * open_delim_curly (Node * n);
	Node * open_delim_square (Node * n);
	Node * open_delim_round (Node * n);
	Node * open_delim_angle (Node * n);

	Node * plus (Node * n);
	Node * minus (Node * n);
	Node * times (Node * n);
}

#endif