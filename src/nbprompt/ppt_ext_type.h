/*
 * ppt_ext_type.h
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

#ifndef PPT_EXT_TYPE_H_
#define PPT_EXT_TYPE_H_

#include <string>

/*
 * Prompt extension types
 */
enum {
	PPT_EXT_EXC_INVAL_ARG = 0,
	PPT_EXT_EXC_INVAL_ARG_TYPE,
	PPT_EXT_EXC_INVAL_PARAM,
	PPT_EXT_EXC_MISSING_INPUT,
	PPT_EXT_UNIMPL_CMD_TYPE,
};

/*
 * Max prompt extension types
 */
#define MAX_PPT_EXT_EXC_TYPE PPT_EXT_UNIMPL_CMD_TYPE

/*
 * Prompt extension strings
 */
static const std::string PPT_EXT_EXC_STR[] = {
	"Invalid prompt argument",
	"Invalid prompt argument type",
	"Invalid prompt parameter",
	"Missing prompt input for command",
	"Unimplemented command",
};

/*
 * Prompt extension string lookup macros
 */
#define PPT_EXT_EXC_STRING(_T_) (_T_ > MAX_PPT_EXT_EXC_TYPE ? "Unknown prompt extension exception type" : PPT_EXT_EXC_STR[_T_])

/*
 * Prompt extension exception macros
 */
#define THROW_PPT_EXT_EXC(_T_) {\
	std::stringstream ss;\
	ss << PPT_EXT_EXC_STRING(_T_);\
	throw std::runtime_error(ss.str().c_str());\
	}
	
#define THROW_PPT_EXT_EXC_W_MESS(_M_, _T_) {\
	std::stringstream ss;\
	ss << PPT_EXT_EXC_STRING(_T_) << ": " << _M_;\
	throw std::runtime_error(ss.str().c_str());\
	}

#endif