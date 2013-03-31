/*
 * tree_type.h
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

#ifndef TREE_TYPE_H_
#define TREE_TYPE_H_

#include <string>

/*
 * Tree exception types
 */
enum {
	TREE_EXC_INVAL_CHILD_NODE = 0,
	TREE_EXC_INVAL_CHILD_NODE_INDEX,
	TREE_EXC_INVAL_PARENT_NODE,
	TREE_EXC_INVAL_POS_PTR,
	TREE_EXC_INVAL_ROOT_PTR,
	TREE_EXC_OUT_OF_MEMORY,
};

/*
 * Max tree types
 */
#define MAX_TREE_EXC_TYPE TREE_EXC_OUT_OF_MEMORY

/*
 * Tree strings
 */
static const std::string TREE_EXC_STR[] = {
	"Attempting to transition to invalid child node",
	"Attempting to transition to invalid child node index",
	"Attempting to transition to invalid parent node",
	"Attempting to transition to invalid position node",
	"Attempting to transition to invalid root node",
	"Failed to allocate node",
};

/*
 * Tree string lookup macros
 */
#define TREE_EXC_STRING(_T_) (_T_ > MAX_TREE_EXC_TYPE ? "Unknown tree exception type" : TREE_EXC_STR[_T_])

/*
 * Tree exception macros
 */
#define THROW_TREE_EXC(_T_) {\
	std::stringstream ss;\
	ss << TREE_EXC_STRING(_T_);\
	throw std::runtime_error(ss.str().c_str());\
	}
	
#define THROW_TREE_EXC_W_MESS(_M_, _T_) {\
	std::stringstream ss;\
	ss << TREE_EXC_STRING(_T_) << ": " << _M_;\
	throw std::runtime_error(ss.str().c_str());\
	}

#endif