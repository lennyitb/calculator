#include "calc.hpp"
#include "version.hpp"

using namespace std;

/// The main function.
int main()
{
	cout << FULL_NAME << " v" << VERSION_STR << " build " << BUILD_NUM << endl;
	#ifdef DEBUG
	cout << "this is a debug build." << endl;
	#endif


	Stack s {100};
	Runtime r; Parser::set_runtime(r); //may rethink this. The idea is that theoretically, there could be multiple stacks, but only one 'runtime'.

	while (true)
	{
		if (!Parser::parse_to(cin, s)) { break; }
		cout << '\n';
		for (unsigned int i = s.depth(); i > 0; --i)
		{
			cout << i << ": " << s.get_level_ref(i)->get_data_str();
			cout << endl;
		}
		cout << "okay thank you." << endl;
	}
	cout << "exiting..." << endl;
	return 0;
}