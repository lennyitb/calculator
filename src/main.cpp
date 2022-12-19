#include "calc.hpp"
using namespace std;
using namespace GiNaC;

int main()
{
	#ifdef DEBUG
	cout << "this is a debug build." << endl;
	#endif
	NodeContainer c{100};
	constexpr unsigned int num_nodes = 4;
	Node * n[num_nodes];
	for (unsigned int i = 0; i < num_nodes; ++i)
	{
		n[i] = c.reserve_node();
	}
	n[0]->data.type = TYPE_CMD;
	n[0]->links.down = n[1];
	n[1]->data.type = TYPE_NUMERIC;
	n[1]->links.next = n[3];
	n[1]->data.data = new numeric {5,7};
	n[2]->data.type = TYPE_NUMERIC;
	n[2]->data.data = new numeric {7,13};
	*n[3] = *n[2];

	cout << n[0]->get_data_str() << endl;

	c.delete_all_from_root(n[0]);

	return 0;
}

// 	Node n[3];
// 	n[0].data.type = TYPE_NUMERIC;
// 	n[0].data.data = new numeric{2, 3};
// 	n[1].data.type = TYPE_NUMERIC;
// 	n[1].data.data = new numeric{4};
// 	n[2].data.type = TYPE_CMD;

// 	n[2].links.down = &n[0];
// 	n[0].links.next = &n[1];

// 	cout << n[2].get_data_str() << endl;
	
// 	return 0;
// }