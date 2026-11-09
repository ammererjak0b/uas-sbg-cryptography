#ifndef DBGHELP_H_INCLUDED
#define DBGHELP_H_INCLUDED

#include <gmpxx.h>
#include <string>

//Make sure that mpz_class::get_str is called at least once
//so that it is linked and can be used in debug expressions
static std::string DBGHELP_H_DUMMY = mpz_class().get_str();

#endif
