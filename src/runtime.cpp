#include "calc.hpp"
using namespace std;
using namespace GiNaC;

SymbolRecord::SymbolRecord(const SymbolRecord & old_symbol)
{
	name = old_symbol.name;
	set_type = old_symbol.set_type;
	if (set_type == TYPE_NUMERIC) { ndata = old_symbol.ndata; }
	else if (set_type == TYPE_EX) { exdata = old_symbol.exdata; }
}
SymbolRecord::~SymbolRecord ()
{
	if (set_type == TYPE_NUMERIC) { delete ndata; }
	else if (set_type == TYPE_EX) { delete exdata; }
}

bool SymbolRecord::operator== (const string & str) { return name == str; }

SymbolRecord * SymbolRecord::sto (numeric * value)
{
	set_type = TYPE_NUMERIC;
	ndata = value;
	return this;
}
SymbolRecord * SymbolRecord::sto (ex * value)
{
	set_type = TYPE_EX;
	exdata = value;
	return this;
}
SymbolRecord * Runtime::get_or_register_symbol(const string & name)
{
	vector<SymbolRecord>::iterator end { symbol_table.end() };
	vector<SymbolRecord>::iterator it { find(symbol_table.begin(), end, name)};
	if (it == end) {
		realsymbol sym {name.c_str()}; //TODO support complex symbols
		symbol_table.push_back(SymbolRecord(name, sym));	
		return &symbol_table.back();
	}
	return &*it;
}

bool Runtime::symbol_exists_q (string & name)
{
	return (find(symbol_table.begin(), symbol_table.end(), name) != symbol_table.end());
}
