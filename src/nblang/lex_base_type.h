/*
 * lex_base_type.h
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

#ifndef LEX_BASE_TYPE_H_
#define LEX_BASE_TYPE_H_

#include "lang.h"
#include <string>

/*
 * Lexer base class type
 */
class _lex_base;
typedef _lex_base lex_base, *lex_base_ptr;

/*
 * Lexer base types
 */
enum {
	LEX_BASE_EXC_FILE_NOT_FOUND = 0,
	LEX_BASE_EXC_NO_NEXT_SYMBOL,
	LEX_BASE_EXC_NO_PREV_SYMBOL,
	LEX_BASE_EXC_OUT_OF_BOUNDS,
};

enum {
	SYMBOL_BASE_TYPE_ALPHA,
	SYMBOL_BASE_TYPE_DIGIT,
	SYMBOL_BASE_TYPE_SYMBOL,
	SYMBOL_BASE_TYPE_WHITESPACE,
};

/*
 * Max lexer base types
 */
#define MAX_LEX_BASE_EXC_TYPE LEX_BASE_EXC_OUT_OF_BOUNDS
#define MAX_SYMBOL_BASE_TYPE SYMBOL_BASE_TYPE_WHITESPACE

/*
 * Lexer base strings
 */
static const std::string LEX_BASE_EXC_STR[] = {
	"File not found",
	"No next symbol found",
	"No previous symbol found",
	"Attempting to transition to non-existant symbol",
};

static const std::string SYMBOL_BASE_TYPE_STR[] = {
	"alpha", "digit", "symbol", "whitespace",
};

/*
 * Lexer base whitespace characters
 */
#define WS_CARAGE_RETURN '\r'
#define WS_ENDSTREAM '\0'
#define WS_FEED '\f'
#define WS_HORIZONTAL_TAB '\t'
#define WS_NEWLINE '\n'
#define WS_SPACE ' '
#define WS_VERTICAL_TAB '\v'
		
/*
 * Lexer base string lookup macros
 */
#define LEX_BASE_EXC_STRING(_T_) (_T_ > MAX_LEX_BASE_EXC_TYPE ? "Unknown lexer base exception type" : LEX_BASE_EXC_STR[_T_])
#define SYMBOL_BASE_TYPE_STRING(_T_) (_T_ > MAX_SYMBOL_BASE_TYPE ? "unknown symbol base type" : SYMBOL_BASE_TYPE_STR[_T_])

/*
 * Lexer base exception macros
 */
#define THROW_LEX_BASE_EXC(_T_) {\
	std::stringstream ss;\
	ss << LEX_BASE_EXC_STRING(_T_);\
	throw std::runtime_error(ss.str().c_str());\
	}
	
#define THROW_LEX_BASE_EXC_W_MESS(_M_, _T_) {\
	std::stringstream ss;\
	ss << LEX_BASE_EXC_STRING(_T_) << ": " << _M_;\
	throw std::runtime_error(ss.str().c_str());\
	}
	
/*
 * Lexer base type macros
 */
#define IS_ALPHA(_C_) (isalpha(_C_) > 0)
#define IS_DIGIT(_C_) (isdigit(_C_) > 0)
#define IS_WHITESPACE(_C_) (isspace(_C_) > 0)
#define IS_SYMBOL(_C_) (!IS_ALPHA(_C_) && !IS_DIGIT(_C_) && !IS_WHITESPACE(_C_))
#define DETERMINE_SYMBOL_BASE_TYPE(_C_)\
	(IS_ALPHA(_C_) ? SYMBOL_BASE_TYPE_ALPHA : \
		(IS_DIGIT(_C_) ? SYMBOL_BASE_TYPE_DIGIT : \
		(IS_WHITESPACE(_C_) ? SYMBOL_BASE_TYPE_WHITESPACE : \
		SYMBOL_BASE_TYPE_SYMBOL)))

#endif