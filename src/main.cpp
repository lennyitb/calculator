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
	errorMessage msg;                  // actually I'm sufficiently convinceed i now disagree. a symbol may be defined to evaluate to what is
	                                   // ostenibly just a node, stored in a stack, which may be deleted.
									   // it makes sense to say that a stack is singular in foct:
									   // TODO make the stack function as a runtime

	while (true)
	{
		if (!(msg = parse_to(cin, s)))
		{
			if (msg.status == STATUS_EXIT) { cout << "exit signal...  "; break; }
			else { cout << "error- unknown token: " << msg.get_message() << endl; }
		} else {
			cout << '\n';
			for (unsigned int i = s.depth(); i > 0; --i)
			{
				cout << i << ": " << s.get_level_ref(i)->get_data_str();
				cout << endl;
			}
		}
		#ifdef POLITE
			cout << "okay thank you." << endl;
		#endif
	}
	cout << "exiting..." << endl;
	return 0;
}