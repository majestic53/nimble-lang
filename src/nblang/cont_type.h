/*
 * cont_type.h
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

#ifndef CONT_TYPE_H_
#define CONT_TYPE_H_

#include <string>

/*
 * Context exception types
 */
enum {
	CONT_EXC_INVALID_KEY = 0,
	CONT_EXC_INVALID_INDEX,
};

/*
 * Max context types
 */
#define MAX_CONT_EXC_TYPE CONT_EXC_INVALID_INDEX

/*
 * Context strings
 */
static const std::string CONT_EXC_STR[] = {
	"Key does not exist in this context",
	"Index does not exist in this key",
};

/*
 * Context string lookup macro
 */
#define CONT_EXC_STRING(_T_) (_T_ > MAX_CONT_EXC_TYPE ? "Unknown context exception type" : CONT_EXC_STR[_T_])

/*
 * Context exception macro
 */
#define THROW_CONT_EXC(_T_) {\
	std::stringstream ss;\
	ss << CONT_EXC_STRING(_T_);\
	throw std::runtime_error(ss.str().c_str());\
	}
	
#define THROW_CONT_EXC_W_MESS(_M_, _T_) {\
	std::stringstream ss;\
	ss << CONT_EXC_STRING(_T_) << ": " << _M_;\
	throw std::runtime_error(ss.str().c_str());\
	}

#endif