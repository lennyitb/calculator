#include "calc.hpp"
using namespace std;
using namespace GiNaC;

int main()
{
	ostringstream s;

	Node n[3];
	n[0].data.type = TYPE_NUMERIC;
	n[0].data.data = new numeric{2, 3};
	n[1].data.type = TYPE_NUMERIC;
	n[1].data.data = new numeric{4};
	n[2].data.type = TYPE_CMD;

	n[2].links.down = &n[0];
	n[0].links.next = &n[1];

	cout << n[2].get_data_str_all() << endl;
	
	return 0;
}