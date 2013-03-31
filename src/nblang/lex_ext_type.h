/*
 * lex_ext_type.h
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

#ifndef LEX_EXT_TYPE_H_
#define LEX_EXT_TYPE_H_

#include <string>

/*
 * Lexer extension types
 */
enum {
	LEX_EXT_EXC_INVAL_ALPHA_SYMBOL = 0,
	LEX_EXT_EXC_INVAL_DIGIT_SYMBOL,
	LEX_EXT_EXC_INVAL_SYMBOL,
	LEX_EXT_EXC_UNTERM_STRING,
};

/*
 * Max lexer extension types
 */
#define MAX_LEX_EXT_EXC_TYPE LEX_EXT_EXC_UNTERM_STRING

/*
 * Lexer extension strings
 */
static const std::string LEX_EXT_EXC_STR[] = {
	"Invalid alpha symbol type",
	"Invalid digit symbol type",
	"Invalid symbol type",
	"Unterminated string detected",
};

/*
 * Lexer extension string lookup macros
 */
#define LEX_EXT_EXC_STRING(_T_) (_T_ > MAX_LEX_EXT_EXC_TYPE ? "Unknown lexer extension exception type" : LEX_EXT_EXC_STR[_T_])

/* 
 * Lexer extension exception macros
 */
#define THROW_LEX_EXT_EXC(_T_) {\
	std::stringstream ss;\
	ss << LEX_EXT_EXC_STRING(_T_);\
	throw std::runtime_error(ss.str().c_str());\
	}
	
#define THROW_LEX_EXT_EXC_W_MESS(_M_, _T_) {\
	std::stringstream ss;\
	ss << LEX_EXT_EXC_STRING(_T_) << ": " << _M_;\
	throw std::runtime_error(ss.str().c_str());\
	}

#endif