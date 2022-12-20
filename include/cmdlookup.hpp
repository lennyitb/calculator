#ifndef CMDLOOKUP_H
#define CMDLOOKUP_H

namespace CMDLookup
{
	struct Command {
		std::string str;
		void * cmd;
	};
	const std::vector<Command> command_list {
		{"plus", (void *) &NativeCMD::plus},
		{"minus", (void *) &NativeCMD::minus},
		{"times", (void *) &NativeCMD::times},
		{"{", (void *) &NativeCMD::open_delim_curly},
		{"[", (void *) &NativeCMD::open_delim_square},
		{"(", (void *) &NativeCMD::open_delim_round},
		{"<<", (void *) &NativeCMD::open_delim_angle}
	};
}

#endif