
// this is the universal header file. to adapt my program, you should simply have to include this file (and of course link all the source files)
// i've identified 5 or 6 moving parts, as well as a couple of important defines, which are all included in order below.


/**************************************\
\*                                    */
/*     Welcome to my header file.     *\
\*           made by  Lenny           */
/*                                    *\
\**************************************/


#ifndef CALC_H
#define CALC_H

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>

#include <ginac/ginac.h>

#define PARSE_EOF '\n'
#define ERROR_MSG "error"
#define CMD_DELIM '('

typedef const unsigned int cunt;

#include "nodecontainer.hpp"
#include "nodeoperators.hpp"
#include "stack.hpp"
#include "runtime.hpp"
#include "nativecmd.hpp"
#include "parser.hpp"

#endif
