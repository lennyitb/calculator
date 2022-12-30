#ifndef PARSER_H
#define PARSER_H

namespace Parser
{
	enum errorStatus
	{
		STATUS_OK,
		STATUS_EXIT,
		STATUS_UNKNOWN_TOKEN,
		STATUS_UNKNOWN_ERROR
	};
	class errorMessage
	{
	public:
		errorStatus status;
		std::string message;
		inline const char * get_message() { return message.c_str(); }
		errorMessage(errorStatus s, std::string m) : status(s), message(m) {}
		errorMessage() : status(STATUS_OK), message("") {}
		inline operator bool()
		{
			return status == STATUS_OK; 
		}
	};
	void set_runtime (Runtime & r);
	errorMessage parse_to(std::istream & is, Stack & stack);
}

#endif