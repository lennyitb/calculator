#ifndef CMDLOOKUP_H
#define CMDLOOKUP_H

namespace CMDLookup
{
	struct Command {
		std::string str;
		std::string infx;
		Node *(*cmd)(Node *, NodeContainer *);
		bool infixable;
	};
	const std::vector<Command> command_list {
		{"plus", "+", &NativeCMD::plus, true},
		{"minus", "-", &NativeCMD::minus, true},
		{"times", "*", &NativeCMD::times, true},
		{"divide", "/", &NativeCMD::divide, true},
		{"{", " ", &NativeCMD::open_delim_curly, false},
		{"[", " ", &NativeCMD::open_delim_square, false},
		{"(", " ", &NativeCMD::open_delim_round, false},
		{"<<", " ", &NativeCMD::open_delim_angle, false}
	};
}

#endif