#include "calc.hpp"

namespace Version {
#include "version.hpp"
}

using namespace std;
using namespace Parser;

/// The main function.
int main()
{
	Version::stream_version_info(cout);
	#ifdef DEBUG
		cout << "this is a debug build." << endl;
	#endif
	#ifdef POLITE
		cout << "please input an expression:" << endl;
	#endif

	Stack s {100};
	Runtime r; Parser::set_runtime(r); // may rethink this. The idea is that theoretically, there could be multiple stacks, but only one 'runtime'.
	errorMessage msg;                  // actually I'm sufficiently convinced i now disagree. a symbol may be defined to evaluate to what is
	                                   // ostensibly just a node, stored in a stack, which may be deleted.
									   // it makes sense to say that a stack is singular in fact:
									   // TODO make the stack function as a runtime

	//quit if this loop is broken for any reason
	while (true)
	{
	//set errorMessage msg to return of parse_to
	//not errorMessage will test false if there is an error/exit
		if (!(msg = parse_to(cin, s)))
	//if error in input (or exit command)
		{
			cout << endl;
			if (msg.status == STATUS_EXIT) { cout << "exit signal...  "; break; }
			else { cout << "error- unknown token: " << msg.get_message() << endl; }
	//case for no error on input
		} else {
			cout << '\n';
	//print each level of the stack, starting at the top
			for (unsigned int i = s.depth(); i > 0; --i)
			{
				if (i < 10) { cout << ' '; }
				cout << i << ": " << s.get_level_ref(i)->get_data_str();
				cout << endl;
			}
		}
	//and don't forget to thank our guests
		#ifdef POLITE
			cout << "okay thank you." << endl;
		#endif
	}


	cout << endl;
	#ifdef POLITE
		cout << "thanks for stopping by! ";
	#endif
	cout << "exiting..." << endl;
	return 0;
}