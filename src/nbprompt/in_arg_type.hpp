/*
 * in_arg_type.h
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

#ifndef IN_ARG_TYPE_H_
#define IN_ARG_TYPE_H_

#include <string>

/*
 * Input argument class types
 */
class _in_arg;
typedef _in_arg in_arg, *in_arg_ptr;

/*
 * Input argument exception types
 */
enum {
	IN_ARG_EXC_INVAL_ARG_INDEX = 0,
};

/*
 * Max input argument types
 */
#define MAX_IN_ARG_EXC_TYPE IN_ARG_EXC_INVAL_ARG_INDEX

/*
 * Input argument exception strings
 */
static const std::string IN_ARG_EXC_STR[] = {
	"Attempting to transition to invalid input argument index",
};

/*
 * Input argument exception string lookup macros
 */
#define IN_ARG_EXC_STRING(_T_) (_T_ > MAX_IN_ARG_EXC_TYPE ? "Unknown input argument exception type" : IN_ARG_EXC_STR[_T_])

/*
 * Input argument exception macros
 */
#define THROW_IN_ARG_EXC(_T_) {\
	std::stringstream ss;\
	ss << IN_ARG_EXC_STRING(_T_);\
	throw std::runtime_error(ss.str().c_str());\
	}
	
#define THROW_IN_ARG_EXC_W_MESS(_M_, _T_) {\
	std::stringstream ss;\
	ss << IN_ARG_EXC_STRING(_T_) << ": " << _M_;\
	throw std::runtime_error(ss.str().c_str());\
	}

#endif