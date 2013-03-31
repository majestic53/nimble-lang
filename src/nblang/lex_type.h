/*
 * lex_type.h
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

#ifndef LEX_TYPE_H_
#define LEX_TYPE_H_

#include "lex_base_type.h"
#include "tok_type.h"

/*
 * Lexer class type
 */
class _lex;
typedef _lex lex, *lex_ptr;

/* 
 * Lexer types
 */
enum {
	LEX_EXC_INSERT_INTO_EMPTY_LIST = 0,
	LEX_EXC_INVAL_ACTION_TYPE,
	LEX_EXC_INVAL_ENUM_ACTION_TYPE,
	LEX_EXC_INVAL_CLASS_SYMBOL,
	LEX_EXC_INVAL_STMT_TOK_TYPE,
	LEX_EXC_NO_NEXT_TOKEN,
	LEX_EXC_NO_PREV_TOKEN,
	LEX_EXC_OUT_OF_BOUNDS,
	LEX_EXC_UNALLOC_ACTION,
	LEX_EXC_UNALLOC_ENUM_ACTION,
};

enum {
	LEX_ACTION_SKIP_WHITESPACE = 0,
};

enum {
	LEX_ENUM_ACTION_ALPHA = 0,
	LEX_ENUM_ACTION_NUMBER,
	LEX_ENUM_ACTION_SYMBOL,
};

/*
 * Max lexer types
 */
#define MAX_LEX_EXC_TYPE LEX_EXC_UNALLOC_ENUM_ACTION
#define MAX_STMT_TOK_TYPE CLASS_TOKEN_TYPE_STREAM_STATEMENT
#define MAX_LEX_ACTION_TYPE LEX_ACTION_SKIP_WHITESPACE
#define MAX_LEX_ENUM_ACTION_TYPE LEX_ENUM_ACTION_SYMBOL
#define MIN_LEX_TOKEN_COUNT 2

/*
 * Lexer strings
 */
static const std::string LEX_EXC_STR[] = {
	"Attempting to insert statement token into empty token list",
	"Invalid lexer action type",
	"Invalid lexer enumeration action type",
	"Invalid class symbol type",
	"Invalid statement token type",
	"No next token found",
	"No previous token found",
	"Attempting to transition to non-existant token",
	"Attempting call to unallocated lexer action",
	"Attempting call to unallocated lexer enumeration action",
};

static const std::string LEX_ACTION_STR[] = {
	"LEX_ACTION_SKIP_WHITESPACE",
};

static const std::string LEX_ENUM_ACTION_STR[] = {
	"LEX_ENUM_ACTION_ALPHA",
	"LEX_ENUM_ACTION_NUMBER",
	"LEX_ENUM_ACTION_SYMBOL",
};

/*
 * Lexer string lookup macros
 */
#define LEX_EXC_STRING(_T_) (_T_ > MAX_LEX_EXC_TYPE ? "Unknown lexer exception type" : LEX_EXC_STR[_T_])
#define LEX_ACTION_STRING(_T_) (_T_ > MAX_LEX_ACTION_TYPE ? "Unknown lexer action type" : LEX_ACTION_STR[_T_])
#define LEX_ENUM_ACTION_STRING(_T_) (_T_ > MAX_LEX_ENUM_ACTION_TYPE ? "Unknown lexer enumeration action type" : LEX_ENUM_ACTION_STR[_T_])

/*
 * Lexer exception macros
 */
#define THROW_LEX_EXC(_T_) {\
	std::stringstream ss;\
	ss << LEX_EXC_STRING(_T_);\
	throw std::runtime_error(ss.str().c_str());\
	}
	
#define THROW_LEX_EXC_W_MESS(_M_, _T_) {\
	std::stringstream ss;\
	ss << LEX_EXC_STRING(_T_) << ": " << _M_;\
	throw std::runtime_error(ss.str().c_str());\
	}

/*
 * Lexer type macros
 */
#define IS_CHAR_LB(_LB_, _C_) (_LB_->get() == _C_)
#define IS_TYPE_LB(_LB_, _T_) (_LB_->get_base_type() == _T_)
#define IS_TYPE_T(_T_, _TP_) (_T_.get_type() == _TP_)

/*
 * Lexer common types
 */
typedef void(*lex_action)(lex_ptr);
typedef void(*lex_enum_action)(lex_ptr, tok &);

#endif