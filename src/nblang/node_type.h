/*
 * node_type.h
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

#ifndef NODE_TYPE_H_
#define NODE_TYPE_H_

#include <string>

/*
 * Node exception types
 */
enum {
	NODE_EXC_INVALID_CHILD_NODE_INDEX = 0,
	NODE_EXC_INVALID_PARENT_NODE,
};

/*
 * Max node types
 */
#define MAX_NODE_EXC_TYPE NODE_EXC_INVALID_PARENT_NODE

/*
 * Node strings
 */
static const std::string NODE_EXC_STR[] = {
	"Attempting to transition to invalid child node index",
	"Attempting to transition to invalid parent node",
};

/*
 * Node string lookup macros
 */
#define NODE_EXC_STRING(_T_) (_T_ > MAX_NODE_EXC_TYPE ? "Unknown node exception type" : NODE_EXC_STR[_T_])

/*
 * Node exception macros
 */
#define THROW_NODE_EXC(_T_) {\
	std::stringstream ss;\
	ss << NODE_EXC_STRING(_T_);\
	throw std::runtime_error(ss.str().c_str());\
	}
	
#define THROW_NODE_EXC_W_MESS(_M_, _T_) {\
	std::stringstream ss;\
	ss << NODE_EXC_STRING(_T_) << ": " << _M_;\
	throw std::runtime_error(ss.str().c_str());\
	}

#endif