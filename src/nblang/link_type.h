/*
 * link_type.h
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

#ifndef LINK_TYPE_H_
#define LINK_TYPE_H_

#include <string>

/*
 * Link exception types
 */
enum {
	LINK_EXC_INVALID_NEXT_NODE = 0,
	LINK_EXC_INVALID_PREVIOUS_NODE,
};

/*
 * Max link types
 */
#define MAX_LINK_EXC_TYPE LINK_EXC_INVALID_PREVIOUS_NODE

/*
 * Link strings
 */
static const std::string LINK_EXC_STR[] = {
	"Attempting to transition to invalid next link",
	"Attempting to transition to invalid previous link",
};

/*
 * Link string lookup macros
 */
#define LINK_EXC_STRING(_T_) (_T_ > MAX_LINK_EXC_TYPE ? "Unknown link exception type" : LINK_EXC_STR[_T_])

/*
 * Link exception macros
 */
#define THROW_LINK_EXC(_T_) {\
	std::stringstream ss;\
	ss << LINK_EXC_STRING(_T_);\
	throw std::runtime_error(ss.str().c_str());\
	}
	
#define THROW_LINK_EXC_W_MESS(_M_, _T_) {\
	std::stringstream ss;\
	ss << LINK_EXC_STRING(_T_) << ": " << _M_;\
	throw std::runtime_error(ss.str().c_str());\
	}

#endif