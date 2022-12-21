#ifndef CMDLOOKUP_H
#define CMDLOOKUP_H

namespace CMDLookup
{
	struct Command {
		std::string str;
		Node *(*cmd)(Node *);
	};
	const std::vector<Command> command_list {
		{"plus", &NativeCMD::plus},
		{"minus", &NativeCMD::minus},
		{"times", &NativeCMD::times},
		{"{", &NativeCMD::open_delim_curly},
		{"[", &NativeCMD::open_delim_square},
		{"(", &NativeCMD::open_delim_round},
		{"<<", &NativeCMD::open_delim_angle}
	};
}

#endif