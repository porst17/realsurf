#ifndef _TRANSLATOR_H_
#define _TRANSLATOR_H_

#include <string>
#include <ostream>
#include <set>

namespace translator
{
	bool translate_formula( const std::string formula, unsigned int &degree, std::ostream &glsl_code, std::set<std::string> &params, std::ostream &error_msg );
};

#endif