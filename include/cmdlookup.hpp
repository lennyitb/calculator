#ifndef CMDLOOKUP_H
#define CMDLOOKUP_H

namespace CMDLookup
{
	struct Command {
		std::string str;
		std::string infx;
		Node *(*cmd)(Node *, NodeContainer *);
		cmdSymbol cmd_symbol;
		bool infixable;
	};
	const std::vector<Command> command_list {
		{"plus", "+", &NativeCMD::plus, CMD_PLUS, true},
		{"minus", "-", &NativeCMD::minus, CMD_MINUS, true},
		{"times", "*", &NativeCMD::times, CMD_TIMES, true},
		{"divide", "/", &NativeCMD::divide, CMD_DIVIDE, true},
		{"{", " ", &NativeCMD::open_delim_curly, CMD_NULL, false},
		{"[", " ", &NativeCMD::open_delim_square, CMD_NULL, false},
		{"(", " ", &NativeCMD::open_delim_round, CMD_NULL, false},
		{"<<", " ", &NativeCMD::open_delim_angle, CMD_NULL, false}
	};
}

#endif
