/*
 * exe_ext_type.h
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

#ifndef EXE_EXT_TYPE_H_
#define EXE_EXT_TYPE_H_

#include <string>

/*
 * Executor extension types
 */
enum {
	EXE_EXT_EXC_DIVIDE_BY_ZERO = 0,
	EXE_EXT_EXC_INVAL_ACTION_PARAM,
	EXE_EXT_EXC_INVAL_ASSIGNMENT_COUNT,
	EXE_EXT_EXC_INVAL_ASSIGNMENT_TOKEN,
	EXE_EXT_EXC_INVAL_CALL_TOKEN,
	EXE_EXT_EXC_INVAL_COMPARATOR_TOKEN,
	EXE_EXT_EXC_INVAL_CONDITIONAL_TOKEN,
	EXE_EXT_EXC_INVAL_CONDITIONAL_STMT_TOKEN,
	EXE_EXT_EXC_INVAL_CONDITIONAL_STMT_LIST_TOKEN,
	EXE_EXT_EXC_INVAL_CONTEXT_NODE,
	EXE_EXT_EXC_INVAL_CONTROL_TOKEN,
	EXE_EXT_EXC_INVAL_EVAL_ACTION_PARAM,
	EXE_EXT_EXC_INVAL_EXPRESSION_CLASS_TOKEN,
	EXE_EXT_EXC_INVAL_FUNCT_PARAM_CLASS_TOKEN,
	EXE_EXT_EXC_INVAL_FUNCT_PARAM_COUNT,
	EXE_EXT_EXC_INVAL_IDENT_OPERATOR_TOKEN,
	EXE_EXT_EXC_INVAL_IN_LIST_TOKEN,
	EXE_EXT_EXC_INVAL_INT_OPER_TOKEN,
	EXE_EXT_EXC_INVAL_IO_TOKEN,
	EXE_EXT_EXC_INVAL_ITERATOR_TOKEN,
	EXE_EXT_EXC_INVAL_ITERATABLE_TOKEN,
	EXE_EXT_EXC_INVAL_ITERATABLE_CLASS_TOKEN,
	EXE_EXT_EXC_INVAL_OUT_LIST_TOKEN,
	EXE_EXT_EXC_INVAL_PARAM,
	EXE_EXT_EXC_INVAL_RANGE_STMT_CLASS_TOKEN,
	EXE_EXT_EXC_INVAL_STMT_CLASS_TOKEN,
	EXE_EXT_EXC_INVAL_STMT_LIST_CLASS_TOKEN,
	EXE_EXT_EXC_INVAL_STMT_TOKEN,
	EXE_EXT_EXC_INVAL_STREAM_TOKEN,
	EXE_EXT_EXC_INVAL_STRING_OPER_TOKEN,
	EXE_EXT_EXC_INVAL_TOKEN_CLASS_TOKEN,
	EXE_EXT_EXC_INVAL_TOKEN_INDEX,
	EXE_EXT_EXC_INVAL_USING_TOKEN,
	EXE_EXT_EXC_KEY_NOT_FOUND,
	EXE_EXT_EXC_REF_TO_NON_EXISTENT_ARG,
	EXE_EXT_EXC_REF_TO_NON_EXISTENT_FUNCT,
	EXE_EXT_EXC_REF_TO_NON_EXISTENT_INDEX,
	EXE_EXT_EXC_UNEXPECT_EOS,
};

/*
 * Max executor extension types
 */
#define MAX_EXE_EXT_EXC_TYPE EXE_EXT_EXC_UNEXPECT_EOS

/*
 * Executor tree offsets
 */
#define ASSIGNMENT_INDEX_CHILD 1
#define ASSIGNMENT_MAX_CHILD_COUNT 3
#define ASSIGNMENT_TYPE_CHILD 1
#define COND_STMT_COMPARATOR_CHILD 0
#define COND_STMT_EXPRESSION_CHILD_0 1
#define COND_STMT_EXPRESSION_CHILD_1 2
#define COND_STMT_SEPERATOR_CHILD 1
#define FILE_STREAM_EXPRESSION_CHILD 1
#define FUNCT_SCHEMA_ID_PARAM_LIST_CHILD 0
#define FUNCT_SCHEMA_STMT_LIST_CHILD 1
#define FUNCT_SCHEMA_EXPR_PARAM_LIST_CHILD 2
#define FUNCT_LIST_INDEX_CHILD 1
#define FUNCT_LIST_INDEX_CHILD_COUNT 2
#define IDENTIFIER_TYPE_CHILD 0
#define IF_COND_STMT_CHILD 0
#define IF_COND_STMT_LIST_CHILD 2
#define IFELSE_COND_STMT_CHILD 0
#define IFELSE_COND_STMT0_LIST_CHILD 2
#define IFELSE_COND_STMT1_LIST_CHILD 3
#define IN_LIST_EXPRESSION_CHILD 1
#define IO_OUT_EXPRESSION_CHILD 1
#define ITER_LIST_INDEX_CHILD 1
#define	ITER_MAX_CHILD_COUNT 2
#define MIN_CHILD_COUNT 1
#define RANGE_COND_STMT_CHILD 0
#define RANGE_COND_STMT_LIST_CHILD 1
#define RANGE_STMT_COND_EXPR_LIST_CHILD 2
#define RANGE_STMT_COND_STMT_CHILD 0
#define STREAM_STMT_STREAM_LIST_CHILD 1
#define UNTIL_COND_STMT_LIST_CHILD 0
#define UNTIL_COND_STMT_CHILD 1
#define UNTIL_STMT_SEPERATOR_CHILD 2
#define WHILE_COND_STMT_CHILD 0
#define WHILE_COND_STMT_LIST_CHILD 2

/*
 * Executor extension strings
 */
static const std::string EXE_EXT_EXC_STR[] = {
	"Attempting to divide by zero",
	"Invalid executor action parameter",
	"Invalid assignment count",
	"Invalid assignment token",
	"Invalid call token",
	"Invalid comparator token",
	"Invalid conditional token",
	"Invalid conditional statement class token",
	"Invalid conditional statement list class token",
	"Invalid context node",
	"Invalid control token",
	"Invalid executor evaluation action parameter",
	"Invalid expression class token",
	"Invalid function class token",
	"Invalid function parameter count",
	"Invalid identifier operator token",
	"Invalid in-list operator token",
	"Expecting left/right integer operand token types",
	"Invalid io token",
	"Invalid iterator token",
	"Expecting iteratable token",
	"Expecting iteratable class token",
	"Invalid out-list operator token",
	"Invalid executor parameter",
	"Invalid range statement class token",
	"Invalid statement class token",
	"Invalid statement list class token",
	"Invalid statement token",
	"Invalid stream token",
	"Expecting left string operand token type",
	"Invalid token class token",
	"Invalid token index",
	"Invalid using token",
	"Key does not exist in this context",
	"Attempting to reference non-existent argument",
	"Attempting to reference non-existant function",
	"Attempting to reference non-existent index",
	"Unexpected end of stream in executor",
};

/*
 * Executor extension string lookup macros
 */
#define EXE_EXT_EXC_STRING(_T_) (_T_ > MAX_EXE_EXT_EXC_TYPE ? "Unknown executor extension exception type" : EXE_EXT_EXC_STR[_T_])

/*
 * Executor extension exception macros
 */
#define THROW_EXE_EXT_EXC(_T_) {\
	std::stringstream ss;\
	ss << EXE_EXT_EXC_STRING(_T_);\
	throw std::runtime_error(ss.str().c_str());\
	}
	
#define THROW_EXE_EXT_EXC_W_MESS(_M_, _T_) {\
	std::stringstream ss;\
	ss << EXE_EXT_EXC_STRING(_T_) << ": " << _M_;\
	throw std::runtime_error(ss.str().c_str());\
	}

#endif