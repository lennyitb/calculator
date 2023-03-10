#ifndef NODEOPERATORS_HPP
#define NODEOPERATORS_HPP

namespace NodeInjectOperator
{
	Node * inject_addto (Node * result, Node * op);
	Node * inject_minusto (Node * result, Node * op);
	Node * inject_timesto (Node * result, Node * op);
	Node * inject_divideto (Node * result, Node * op);
}

#endif
