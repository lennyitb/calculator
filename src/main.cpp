#include "calc.hpp"
using namespace std;

/// The main function.
int main()
{
	#ifdef DEBUG
	cout << "this is a debug build." << endl;
	#endif

	Stack s {100};

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