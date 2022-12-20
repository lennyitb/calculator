#include "calc.hpp"
using namespace std;
using namespace GiNaC;

namespace Parser
{
	void parse_native_cmd_to(const string & s, Stack & stack)
	{
		if (s == "plus") { stack.push_new()->set_type(TYPE_CMD); }
	}
	void parse_word_to(const string & s, Stack & stack)
	{
		regex numeric_rgx("^[+-]?([0-9]*[.])?[0-9]+([eE][+-]?[0-9]+)?$");
		regex cmd_rgx("^[a-zA-Z]+$");

		if(regex_match(s,numeric_rgx))
		{
			stack.push_new()->set_numeric(new numeric {s.c_str()});
		}
		if(regex_match(s,cmd_rgx))
		{
			stack.push_new()->set_type(TYPE_CMD);
		}
	}
	Stack & parse_to(istream & is, Stack & stack)
	{
		string s;
		do {
			is >> s;
			parse_word_to(s, stack);
		} while (is.peek() != PARSE_EOF);
		return stack;
	}
}