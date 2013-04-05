/*
 * tok_type.h
 * Copyright (C) 2013 David Jolly
 * ----------------------
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TOK_TYPE_H_
#define TOK_TYPE_H_

#include <string>

class _tok;
typedef _tok tok, *tok_ptr;

/*
 * Token exception types
 */
enum {
	TOK_EXC_INVALID_CLS_TYPE = 0,
	TOK_EXC_INVALID_TYPE,
	TOK_EXC_INVALID_VALUE,
	TOK_EXC_NON_NUMERIC_TYPE,
};

/*
 * Max token types
 */
#define MAX_TOK_EXC_TYPE TOK_EXC_NON_NUMERIC_TYPE

/*
 * Token strings
 */
static const std::string TOK_EXC_STR[] = {
	"Invalid token class type",
	"Invalid token type",
	"Invalid token value",
	"Non-numeric token type",
};

/*
 * Token string lookup macros
 */
#define TOK_EXC_STRING(_T_) (_T_ > MAX_TOK_EXC_TYPE ? "Unknown token exception type" : TOK_EXC_STR[_T_])

/*
 * Token exception macros
 */
#define THROW_TOK_EXC(_T_) {\
	std::stringstream ss;\
	ss << TOK_EXC_STRING(_T_);\
	throw std::runtime_error(ss.str().c_str());\
	}
	
#define THROW_TOK_EXC_W_MESS(_M_, _T_) {\
	std::stringstream ss;\
	ss << TOK_EXC_STRING(_T_) << ": " << _M_;\
	throw std::runtime_error(ss.str().c_str());\
	}

#endif