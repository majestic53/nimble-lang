/*
 * par_ext_type.h
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

#ifndef PAR_EXT_TYPE_H_
#define PAR_EXT_TYPE_H_

#include <string>

/*
 * Parser extension types
 */
enum {
	PAR_EXT_EXC_EXPECT_ARGUMENT = 0,
	PAR_EXT_EXC_EXPECT_ASSIGNMENT,
	PAR_EXT_EXC_EXPECT_CALL,
	PAR_EXT_EXC_EXPECT_CLOSE_BRACE,
	PAR_EXT_EXC_EXPECT_CLOSE_PARENTHESIS,
	PAR_EXT_EXC_EXPECT_CLOSE_STATEMENT_BRACE,
	PAR_EXT_EXC_EXPECT_COMPARATOR,
	PAR_EXT_EXC_EXPECT_COMPARATOR_SEPERATOR,
	PAR_EXT_EXC_EXPECT_CONDITIONAL,
	PAR_EXT_EXC_EXPECT_CONTROL,
	PAR_EXT_EXC_EXPECT_EXPRESSION,
	PAR_EXT_EXC_EXPECT_EXPRESSION_OPERATOR,
	PAR_EXT_EXC_EXPECT_FILE_STREAM_OPERATOR,
	PAR_EXT_EXC_EXPECT_IDENTIFIER,
	PAR_EXT_EXC_EXPECT_IDENTIFIER_OPERATOR,
	PAR_EXT_EXC_EXPECT_IN_LIST_OPERATOR,
	PAR_EXT_EXC_EXPECT_INPUT_IO_OPERATOR,
	PAR_EXT_EXC_EXPECT_IO_OPERATOR,
	PAR_EXT_EXC_EXPECT_ITERATOR,
	PAR_EXT_EXC_EXPECT_LIST_SEPERATOR,
	PAR_EXT_EXC_EXPECT_OPEN_STATEMENT_BRACE,
	PAR_EXT_EXC_EXPECT_OUT_LIST_OPERATOR,
	PAR_EXT_EXC_EXPECT_STATEMENT,
	PAR_EXT_EXC_EXPECT_STATEMENT_SEPERATOR,
	PAR_EXT_EXC_EXPECT_STREAM,
	PAR_EXT_EXC_EXPECT_TERMINATOR,
	PAR_EXT_EXC_EXPECT_USING,
	PAR_EXT_EXC_INVAL_ACTION_PARAM,
	PAR_EXT_EXC_INVAL_CLASS_TOKEN_TYPE,
	PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM,
	PAR_EXT_EXC_INVAL_PARAM,
	PAR_EXT_EXC_UNEXPECT_EOS,
};

/*
 * Max parser extension types
 */
#define MAX_PAR_EXT_EXC_TYPE PAR_EXT_EXC_UNEXPECT_EOS

/*
 * Parser extension strings
 */
static const std::string PAR_EXT_EXC_STR[] = {
	"Expecting argument",
	"Expecting assignment",
	"Expecting call",
	"Expecting close brace: \']\'",
	"Expecting close parenthesis: \')\'",
	"Expecting close statement brace: \'}\'",
	"Expecting comparator statement",
	"Expecting comparator seperator: \'?\'",
	"Expecting conditional statement",
	"Expecting control statement",
	"Expecting expression",
	"Expecting expression operator",
	"Expecting file stream operator",
	"Expecting identifier",
	"Expecting identifier operator",
	"Expecting in-list operator",
	"Expecting input io operator",
	"Expecting io operator",
	"Expecting iterator",
	"Expecting list seperator: \',\'",
	"Expecting open statement brace: \'{\'",
	"Expecting out-list operator",
	"Expecting statement",
	"Expecting statement seperator: \':\'",
	"Expecting stream",
	"Expecting terminator: \';\'",
	"Expecting using statement",
	"Invalid parser action parameter",
	"Encountered unknown class token type",
	"Invalid parser enumeration action parameter",
	"Invalid parser parameter",
	"Unexpected end of stream in parser",
};

/*
 * Parser extension string lookup macro
 */
#define PAR_EXT_EXC_STRING(_T_) (_T_ > MAX_PAR_EXT_EXC_TYPE ? "Unknown parser extension exception type" : PAR_EXT_EXC_STR[_T_])

/*
 * Parser extension exception macros
 */
#define THROW_PAR_EXT_EXC(_T_) {\
	std::stringstream ss;\
	ss << PAR_EXT_EXC_STRING(_T_);\
	throw std::runtime_error(ss.str().c_str());\
	}
	
#define THROW_PAR_EXT_EXC_W_MESS(_M_, _T_) {\
	std::stringstream ss;\
	ss << PAR_EXT_EXC_STRING(_T_) << ": " << _M_;\
	throw std::runtime_error(ss.str().c_str());\
	}

#endif