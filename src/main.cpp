#include "calc.hpp"
using namespace std;
using namespace GiNaC;

int main()
{
	#ifdef DEBUG
	cout << "this is a debug build." << endl;
	#endif

	Stack s {100};

	while (true)
	{
		if (!Parser::parse_to(cin, s)) { break; }
		cout << "okay thank you." << endl;
		for (unsigned int i = s.depth(); i > 0; --i)
		{
			cout << i << ": " << s.get_level_ref(i)->get_data_str();
			cout << endl;
		}
	}
	cout << "exiting..." << endl;
	return 0;
}