#include "calc.hpp"
using namespace GiNaC;

//TODO it's going to be a headache to make all this support real and complex symbols. I can see it coming
// This is better than the last system but I should really think of something.
namespace NodeInjectOperator
{
	Node * inject_addto (Node * result, Node * op)
	{
		if (result->data.type == TYPE_NUMERIC && op->data.type == TYPE_NUMERIC) {
			*result->data.data_numeric += *op->data.data_numeric;
			return result;
		}
		if (result->data.type == TYPE_EX) {
			if (op->data.type == TYPE_NUMERIC) { *result->data.data_ex += *op->data.data_numeric; }
			else if (op->data.type == TYPE_SYMBOL) { *result->data.data_ex += op->data.data_symbol_record->rvalue; }
			else if (op->data.type == TYPE_EX) { *result->data.data_ex += *op->data.data_ex; }
			return result;
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
			if (op->data.type == TYPE_NUMERIC) { *result->data.data_ex -= *op->data.data_numeric; }
			else if (op->data.type == TYPE_SYMBOL) { *result->data.data_ex -= op->data.data_symbol_record->rvalue; }
			else if (op->data.type == TYPE_EX) { *result->data.data_ex -= *op->data.data_ex; }
			return result;
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
			if (op->data.type == TYPE_NUMERIC) { *result->data.data_ex *= *op->data.data_numeric; }
			else if (op->data.type == TYPE_SYMBOL) { *result->data.data_ex *= op->data.data_symbol_record->rvalue; }
			else if (op->data.type == TYPE_EX) { *result->data.data_ex *= *op->data.data_ex; }
			return result;
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
			if (op->data.type == TYPE_NUMERIC) { *result->data.data_ex /= *op->data.data_numeric; }
			else if (op->data.type == TYPE_SYMBOL) { *result->data.data_ex /= op->data.data_symbol_record->rvalue; }
			else if (op->data.type == TYPE_EX) { *result->data.data_ex /= *op->data.data_ex; }
			return result;
		}
		return result->set_type(TYPE_ERROR);
	}
}
