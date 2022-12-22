#include "calc.hpp"
using namespace GiNaC;

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
Node * inject_timesto (Node * result, Node * op)
{
	if (result->data.type == TYPE_NUMERIC && op->data.type == TYPE_NUMERIC) {
		*result->data.data_numeric *= *op->data.data_numeric;
		return result;
	}
	if (result->data.type == TYPE_EX) {
		if (op->data.type == TYPE_NUMERIC) { *result->data.data_ex *= *op->data.data_numeric; return result; }
		if (op->data.type == TYPE_SYMBOL) { *result->data.data_ex *= *op->data.data_symbol; return result; }
		if (op->data.type == TYPE_EX) { *result->data.data_ex *= *op->data.data_ex; return result; }
	}
	return result->set_type(TYPE_ERROR);
}
Node * inject_divideto (Node * result, Node * op)
{
	if (result->data.type == TYPE_NUMERIC && op->data.type == TYPE_NUMERIC) {
		*result->data.data_numeric /= *op->data.data_numeric;
		return result;
	}
	if (result->data.type == TYPE_EX) {
		if (op->data.type == TYPE_NUMERIC) { *result->data.data_ex /= *op->data.data_numeric; return result; }
		if (op->data.type == TYPE_SYMBOL) { *result->data.data_ex /= *op->data.data_symbol; return result; }
		if (op->data.type == TYPE_EX) { *result->data.data_ex /= *op->data.data_ex; return result; }
	}
	return result->set_type(TYPE_ERROR);
}
