#ifndef RUNTIME_H
#define RUNTIME_H

//the Runtime class is meant to store things like user variable definitions and user macros
//the HP 50g I'm drawing a lot of inspiration from has a mini directory system where each user variable is represented as a file

//depends on string, ginac, nodecontainer, and stack

struct SymbolRecord
{
	std::string name;
	nodeDataType set_type {TYPE_SYMBOL};
	union {
		GiNaC::realsymbol rvalue;
		GiNaC::symbol cvalue;
	};
	union {
		GiNaC::numeric * ndata;
		GiNaC::ex * exdata;
	};
	enum symbolType {REAL, COMPLEX} type;
	SymbolRecord(std::string s, GiNaC::realsymbol symb) : name(s),  type(REAL) { rvalue = symb; }
	SymbolRecord(std::string s, GiNaC::symbol symb): name(s), type(COMPLEX) { cvalue = symb; }
	SymbolRecord(const SymbolRecord & old_symbol);
	~SymbolRecord();

	bool operator== (const std::string & str);

	SymbolRecord * sto (GiNaC::numeric * number);
	SymbolRecord * sto (GiNaC::ex * expr);
	inline bool is_set_q() { return set_type != TYPE_SYMBOL; }
};

class Runtime
{
	std::vector<SymbolRecord> symbol_table;
	// enum realOrComplex {REAL, COMPLEX} modeRealOrComplex {REAL}; //when i support complex symbols
public:
	// Runtime(); //not convinced I need either of these
	// ~Runtime();
	SymbolRecord * get_or_register_symbol (const std::string & name);
	bool symbol_exists_q (std::string & name);
};

#endif