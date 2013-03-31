/*
 * par_type.h
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

#ifndef PAR_TYPE_H_
#define PAR_TYPE_H_

#include "cont.h"
#include "lex_type.h"
#include "tok.h"

/*
 * Parser class type
 */
class _par;
typedef _par par, *par_ptr;

/*
 * Parser types
 */
enum {
	PAR_EXC_INVAL_ACTION_TYPE = 0,
	PAR_EXC_INVAL_ENUM_ACTION_TYPE,
	PAR_EXC_NO_NEXT_TOKEN,
	PAR_EXC_NO_PREV_TOKEN,
	PAR_EXC_OUT_OF_BOUNDS,
	PAR_EXC_UNALLOC_ACTION,
	PAR_EXC_UNALLOC_ENUM_ACTION,
};

enum {
	PAR_ACTION_CONFIG_LEXER = 0,
};

enum {
	PAR_ENUM_ACTION_STATEMENT = 0,
};

/*
 * Max parser types
 */
#define MAX_PAR_EXC_TYPE PAR_EXC_UNALLOC_ENUM_ACTION
#define MAX_PAR_ACTION_TYPE PAR_ACTION_CONFIG_LEXER
#define MAX_PAR_ENUM_ACTION_TYPE PAR_ENUM_ACTION_STATEMENT
#define MIN_IMPORT_STATEMENT_COUNT 3

/*
 * Parser strings
 */
static const std::string PAR_EXC_STR[] = {
	"Invalid parser action type",
	"Invalid parser enumeration action type",
	"No next statement found",
	"No previous statement found",
	"Attempting to transition to non-existant statement",
	"Attempting call to unallocated parser action",
	"Attempting call to unallocated parser enumeration action",
};

static const std::string PAR_ACTION_STR[] = {
	"PAR_ACTION_CONFIG_LEXER",
};

static const std::string PAR_ENUM_ACTION_STR[] = {
	"PAR_ENUM_ACTION_STATEMENT",
};

/*
 * Parser string lookup macros
 */
#define PAR_EXC_STRING(_T_) (_T_ > MAX_PAR_EXC_TYPE ? "Unknown parser exception type" : PAR_EXC_STR[_T_])
#define PAR_ACTION_STRING(_T_) (_T_ > MAX_PAR_ACTION_TYPE ? "Unknown parser action type" : PAR_ACTION_STR[_T_])
#define PAR_ENUM_ACTION_STRING(_T_) (_T_ > MAX_PAR_ENUM_ACTION_TYPE ? "Unknown parser enumeration action type" : PAR_ENUM_ACTION_STR[_T_])

/*
 * Parser exception macros
 */
#define THROW_PAR_EXC(_T_) {\
	std::stringstream ss;\
	ss << PAR_EXC_STRING(_T_);\
	throw std::runtime_error(ss.str().c_str());\
	}
	
#define THROW_PAR_EXC_W_MESS(_M_, _T_) {\
	std::stringstream ss;\
	ss << PAR_EXC_STRING(_T_) << ": " << _M_;\
	throw std::runtime_error(ss.str().c_str());\
	}

/*
 * Parser type macros
 */
#define GET_TOKEN_L(_L_) (_L_->get())
#define HAS_TEXT_L(_L_) (!_L_->get().get_text().empty())
#define IS_CLASS_TYPE_L(_L_, _T_) (_L_->get().get_class_type() == _T_)
#define IS_TYPE_L(_L_, _T_) (_L_->get().get_type() == _T_)
#define IS_SUBTYPE_L(_L_, _T_) (_L_->get().get_subtype() == _T_)
#define IS_TOKEN_PRIMITIVE_TYPE_L(_L_, _T_) (IS_CLASS_TYPE_L(_L_, CLASS_TOKEN_TYPE_TOKEN) && IS_TYPE_L(_L_, _T_))
#define IS_TOKEN_SYMBOL_TYPE_L(_L_, _T_) (IS_CLASS_TYPE_L(_L_, CLASS_TOKEN_TYPE_TOKEN) && IS_TYPE_L(_L_, TOKEN_TYPE_SYMBOL) && IS_SUBTYPE_L(_L_, _T_))
#define IS_TOKEN_TYPE_L(_L_, _T_, _S_) (IS_CLASS_TYPE_L(_L_, CLASS_TOKEN_TYPE_TOKEN) && IS_TYPE_L(_L_, _T_) && IS_SUBTYPE_L(_L_, _S_))
#define IS_TOKEN_CLASS_TYPE_L(_L_, _C_, _T_, _S_) (IS_CLASS_TYPE_L(_L_, _C_) && IS_TYPE_L(_L_, _T_) && IS_SUBTYPE_L(_L_, _S_))

/*
 * Parser common types
 */
typedef void(*par_action)(lex_ptr);
typedef void(*par_enum_action)(lex_ptr, tok_stmt &);

#endif