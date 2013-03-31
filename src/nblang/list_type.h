/*
 * list_type.h
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

#ifndef LIST_TYPE_H_
#define LIST_TYPE_H_

#include <string>

/*
 * List exception types
 */
enum {
	LIST_EXC_INVALID_BACK_LINK = 0,
	LIST_EXC_INVALID_FRONT_LINK,
	LIST_EXC_INVALID_LINK_INDEX,
	LIST_EXC_OUT_OF_MEMORY,
};

/*
 * Max list types
 */
#define MAX_LIST_EXC_TYPE LIST_EXC_OUT_OF_MEMORY

/*
 * List strings
 */
static const std::string LIST_EXC_STR[] = {
	"Attempting to transition to invalid back link",
	"Attempting to transition to invalid front link",
	"Attempting to transition to invalid link index",
	"Failed to allocate link",
};

/*
 * List string lookup macros
 */
#define LIST_EXC_STRING(_T_) (_T_ > MAX_LIST_EXC_TYPE ? "Unknown list exception type" : LIST_EXC_STR[_T_])

/*
 * List exception macros
 */
#define THROW_LIST_EXC(_T_) {\
	std::stringstream ss;\
	ss << LIST_EXC_STRING(_T_);\
	throw std::runtime_error(ss.str().c_str());\
	}
	
#define THROW_LIST_EXC_W_MESS(_M_, _T_) {\
	std::stringstream ss;\
	ss << LIST_EXC_STRING(_T_) << ": " << _M_;\
	throw std::runtime_error(ss.str().c_str());\
	}

#endif