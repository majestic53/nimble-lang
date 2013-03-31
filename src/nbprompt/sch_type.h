/*
 * sch_type.h
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

#ifndef SCH_TYPE_H_
#define SCH_TYPE_H_

#include <string>

/*
 * Schema class types
 */
class _sch;
typedef _sch sch, *sch_ptr;

/*
 * Schema exception types
 */
enum {
	SCH_EXC_INVAL_ARG_INDEX = 0,
	SCH_EXC_NO_ARG,
	SCH_EXC_UNALLOC_ACTION,
};

/* 
 * Max schema types
 */
#define MAX_SCH_EXC_TYPE SCH_EXC_UNALLOC_ACTION

/*
 * Schema exception strings
 */
static const std::string SCH_EXC_STR[] = {
	"Attempting to transition to invalid argument index",
	"Attempting to transition in empty argument list",
	"Attempting call to unallocated prompt action",
};

/*
 * Schema exception string lookup macros
 */
#define SCH_EXC_STRING(_T_) (_T_ > MAX_SCH_EXC_TYPE ? "Unknown schema exception type" : SCH_EXC_STR[_T_])

/*
 * Schema exception macros
 */
#define THROW_SCH_EXC(_T_) {\
	std::stringstream ss;\
	ss << SCH_EXC_STRING(_T_);\
	throw std::runtime_error(ss.str().c_str());\
	}
	
#define THROW_SCH_EXC_W_MESS(_M_, _T_) {\
	std::stringstream ss;\
	ss << SCH_EXC_STRING(_T_) << ": " << _M_;\
	throw std::runtime_error(ss.str().c_str());\
	}

#endif