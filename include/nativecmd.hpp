#ifndef NATIVECMD_H
#define NATIVECMD_H

typedef Node * (*native_cmd)(Node *);

namespace NativeCMD
{
	std::string get_cmd_str (void * cmd);
	void * get_cmd_ptr (const std::string & s);

	Node * plus (Node * n);
	Node * minus (Node * n);
}

#endif