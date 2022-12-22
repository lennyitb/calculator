#include "calc.hpp"
using namespace GiNaC;

Node operator+ (Node & a, Node & b)
{
	Node result;
	if (a.data.type == TYPE_NUMERIC && b.data.type == TYPE_NUMERIC) {
		return *result.set_numeric(new numeric(*a.data.data_numeric + *b.data.data_numeric));
	}
	if (a.data.type == TYPE_NUMERIC) {
		if (b.data.type == TYPE_SYMBOL) { return *result.set_ex(new ex(*a.data.data_numeric + *b.data.data_symbol)); }
		if (b.data.type == TYPE_EX) { return *result.set_ex(new ex(*a.data.data_numeric + *b.data.data_ex)); }
	}
	if (a.data.type == TYPE_SYMBOL) {
		if (b.data.type == TYPE_NUMERIC) { return *result.set_ex(new ex(*a.data.data_symbol + *b.data.data_numeric)); }
		if (b.data.type == TYPE_SYMBOL) { return *result.set_ex(new ex(*a.data.data_symbol + *b.data.data_symbol)); }
		if (b.data.type == TYPE_EX) { return *result.set_ex(new ex(*a.data.data_symbol + *b.data.data_ex)); }
	}
	if (a.data.type == TYPE_EX) {
		if (b.data.type == TYPE_NUMERIC) { return *result.set_ex(new ex(*a.data.data_ex + *b.data.data_numeric)); }
		if (b.data.type == TYPE_SYMBOL) { return *result.set_ex(new ex(*a.data.data_ex + *b.data.data_symbol)); }
		if (b.data.type == TYPE_EX) { return *result.set_ex(new ex(*a.data.data_ex + *b.data.data_ex)); }
	}
	return *result.set_type(TYPE_ERROR);
}

Node * inject_addto (Node * result, Node * op)
{
	if (result->data.type == TYPE_NUMERIC && op->data.type == TYPE_NUMERIC) {
		*result->data.data_numeric += *op->data.data_numeric;
		return result;
	}
	if (result->data.type == TYPE_EX) {
		if (op->data.type == TYPE_NUMERIC) { *result->data.data_ex += *op->data.data_numeric; return result; }
		if (op->data.type == TYPE_SYMBOL) { *result->data.data_ex += *op->data.data_symbol; return result; }
		if (op->data.type == TYPE_EX) { *result->data.data_ex += *op->data.data_ex; return result; }
	}
	return result->set_type(TYPE_ERROR);
}
Node * inject_minusto (Node * result, Node * op)
{
	if (result->data.type == TYPE_NUMERIC && op->data.type == TYPE_NUMERIC) {
		*result->data.data_numeric -= *op->data.data_numeric;
		return result;
	}
	if (result->data.type == TYPE_EX) {
		if (op->data.type == TYPE_NUMERIC) { *result->data.data_ex -= *op->data.data_numeric; return result; }
		if (op->data.type == TYPE_SYMBOL) { *result->data.data_ex -= *op->data.data_symbol; return result; }
		if (op->data.type == TYPE_EX) { *result->data.data_ex -= *op->data.data_ex; return result; }
	}
	return result->set_type(TYPE_ERROR);
}

// possible cases after switcharoo
// 2. a numeric, b symbol
// 3. a numeric, b ex
// 5. a symbol, b symbol
// 6. a symbol, b ex
// 7. a ex, b numeric
// 9. a ex, b ex

Node operator- (Node & a, Node & b)
{
	Node result;
	if (a.data.type == TYPE_NUMERIC && b.data.type == TYPE_NUMERIC) {
		return *result.set_numeric(new numeric(*a.data.data_numeric - *b.data.data_numeric));
	}
	Node * lhr; Node * rhr;
	if ((a.data.type == TYPE_SYMBOL && b.data.type == TYPE_NUMERIC)||(a.data.type == TYPE_EX && b.data.type == TYPE_NUMERIC))
		{ lhr = &a; rhr = &b; } else { lhr = &b; rhr = &a; }
	if (lhr->data.type == TYPE_NUMERIC) {
		if (rhr->data.type == TYPE_SYMBOL) { return *result.set_ex(new ex(*lhr->data.data_numeric - *rhr->data.data_symbol)); }
		if (rhr->data.type == TYPE_EX) { return *result.set_ex(new ex(*lhr->data.data_numeric - *rhr->data.data_ex)); }
	}
	if (lhr->data.type == TYPE_SYMBOL) {
		if (rhr->data.type == TYPE_SYMBOL) { return *result.set_ex(new ex(*lhr->data.data_symbol - *rhr->data.data_symbol)); }
		if (rhr->data.type == TYPE_EX) { return *result.set_ex(new ex(*lhr->data.data_symbol - *rhr->data.data_ex)); }
	}
	if (lhr->data.type == TYPE_EX) {
		if (rhr->data.type == TYPE_NUMERIC) { return *result.set_ex(new ex(*lhr->data.data_ex - *rhr->data.data_numeric)); }
		if (rhr->data.type == TYPE_EX) { return *result.set_ex(new ex(*lhr->data.data_ex - *rhr->data.data_ex)); }
	}
	return *result.set_type(TYPE_ERROR);
}