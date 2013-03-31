/*
 * exe_type.h
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

#ifndef EXE_TYPE_H_
#define EXE_TYPE_H_

#include <stack>
#include <vector>
#include "par_type.h"

/*
 * Executor types
 */
enum {
	EXE_EXC_ARG_OUT_OF_BOUNDS = 0,
	EXE_EXC_INVAL_ACTION_TYPE,
	EXE_EXC_INVAL_EVAL_ACTION_TYPE,
	EXE_EXC_NO_NEXT_STATEMENT,
	EXE_EXC_UNALLOC_ACTION,
	EXE_EXC_UNALLOC_EVAL_ACTION,
};

enum {
	EXE_ACTION_CONFIG_PARSER = 0,
};

enum {
	EXE_EVAL_ACTION_STATEMENT = 0,
};

enum {
	EXE_SIG_EXIT = 0,
	EXE_SIG_BREAK,
	EXE_SIG_CONTINUE,
	EXE_SIG_NONE,
};

/*
 * Max executor types
 */
#define MAX_EXE_EXC_TYPE EXE_EXC_UNALLOC_EVAL_ACTION
#define MAX_EXE_ACTION_TYPE EXE_ACTION_CONFIG_PARSER
#define MAX_EXE_EVAL_ACTION_TYPE EXE_EVAL_ACTION_STATEMENT
#define MAX_EXE_SIG_TYPE EXE_SIG_NONE

/*
 * Executor strings
 */
static const std::string EXE_EXC_STR[] = {
	"Attempting to retrieve non-existant argument",
	"Invalid executor action type",
	"Invalid executor evaluation action type",
	"Attempting to step to non-existant statement",
	"Attempting call to unallocated executor action",
	"Attempting call to unallocated executor evaluation action",
};

static const std::string EXE_ACTION_STR[] = {
	"EXE_ACTION_CONFIG_PARSER",
};

static const std::string EXE_EVAL_ACTION_STR[] = {
	"EXE_EVAL_ACTION_STATEMENT",
};

/*
 * Executor string lookup macros
 */
#define EXE_EXC_STRING(_T_) (_T_ > MAX_EXE_EXC_TYPE ? "Unknown executor exception type" : EXE_EXC_STR[_T_])
#define EXE_ACTION_STRING(_T_) (_T_ > MAX_EXE_ACTION_TYPE ? "Unknown executor action type" : EXE_ACTION_STR[_T_])
#define EXE_EVAL_ACTION_STRING(_T_) (_T_ > MAX_EXE_EVAL_ACTION_TYPE ? "Unknown executor evaluation action type" : EXE_EVAL_ACTION_STR[_T_])

/*
 * Executor token macros
 */
#define EXE_SET_TOKEN_TYPE(_T_, _TY_, _V_) {\
	_T_.set_class_type(CLASS_TOKEN_TYPE_TOKEN);\
	_T_.set_type(_TY_);\
	_T_.get_text() = _V_;\
	}

/*
 * Executor exception macros
 */
#define THROW_EXE_EXC(_T_) {\
	std::stringstream ss;\
	ss << EXE_EXC_STRING(_T_);\
	throw std::runtime_error(ss.str().c_str());\
	}
	
#define THROW_EXE_EXC_W_MESS(_M_, _T_) {\
	std::stringstream ss;\
	ss << EXE_EXC_STRING(_T_) << ": " << _M_;\
	throw std::runtime_error(ss.str().c_str());\
	}

/*
 * Executor token type macros
 */
#define IS_CLASS_TYPE_T(_T_, _TY_) (_T_.get_class_type() == _TY_)
#define IS_TYPE_T(_T_, _TY_) (_T_.get_type() == _TY_)
#define IS_SUBTYPE_T(_T_, _TY_) (_T_.get_subtype() == _TY_)
#define IS_TOKEN_PRIMITIVE_TYPE_T(_T_, _TY_) (IS_CLASS_TYPE_T(_T_, CLASS_TOKEN_TYPE_TOKEN) && IS_TYPE_T(_T_, _TY_))
#define IS_TOKEN_TYPE_T(_T_, _TY_, _S_) (IS_CLASS_TYPE_T(_T_, CLASS_TOKEN_TYPE_TOKEN) && IS_TYPE_T(_T_, _TY_) && IS_SUBTYPE_T(_T_, _S_))
#define IS_TOKEN_CLASS_TYPE_T(_T_, _C_, _TY_, _S_) (IS_CLASS_TYPE_T(_T_, _C_) && IS_TYPE_T(_T_, _TY_) && IS_SUBTYPE_T(_T_, _S_))

/*
 * Executor common types
 */
typedef std::vector<tok> tok_vector, *tok_vector_ptr;
typedef std::stack<std::vector<tok_stmt>> tok_stack, *tok_stack_ptr;
typedef void(*exe_action)(par_ptr);
typedef size_t(*exe_eval_action)(const tok_vector &, par_ptr, cont_tree &, tok_stack &);

#endif