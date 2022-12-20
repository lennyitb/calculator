#ifndef CMDLOOKUP_H
#define CMDLOOKUP_H

#include <vector>

namespace Dispatcher
{
	struct Command {
		char str[10];
		void * cmd;
	};
	const std::vector<Command> command_list {
		{"plus", (void *) &NativeCMD::plus},
		{"minus", (void *) &NativeCMD::minus}
	};
}

#endif