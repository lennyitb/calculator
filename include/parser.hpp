#ifndef PARSER_H
#define PARSER_H

namespace Parser
{
	void set_runtime (Runtime & r);
	bool parse_to(std::istream & is, Stack & stack);
}

#endif