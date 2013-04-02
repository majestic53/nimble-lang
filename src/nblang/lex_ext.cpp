/*
 * lex_ext.cpp
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

#include <sstream>
#include <stdexcept>
#include "lex_base.h"
#include "lex_ext.h"
#include "lex_ext_type.h"
#include "tok.h"

//#define LEXER_EXT_TRACING
#ifndef LEXER_EXT_TRACING
#define TRACE_EVENT(_M_, _T_)
#else
#define TRACE_HEADER "lexer_ext"
#include "trace.h"
#endif

void 
lex_enum_alpha(
	lex_ptr lexer, 
	tok &token
	)
{
	TRACE_EVENT("+lex_enum_alpha", TRACE_TYPE_INFORMATION);

	lex_base_ptr base = (lex_base_ptr) lexer;

	if(base) {
		token.set_as_token(TOKEN_TYPE_IDENTIFIER, INVALID_TYPE);

		if(IS_TYPE_LB(base, SYMBOL_BASE_TYPE_ALPHA)
				|| IS_CHAR_LB(base, SYMBOL_UNDERSCORE)) {
			token.set_value(token.get_text() + base->get());

			while(base->has_next()) {
				base->move_next();

				if(!IS_TYPE_LB(base, SYMBOL_BASE_TYPE_ALPHA)
						&& !IS_TYPE_LB(base, SYMBOL_BASE_TYPE_DIGIT)
						&& !IS_CHAR_LB(base, SYMBOL_UNDERSCORE)) {
					break;
				}
				token.set_value(token.get_text() + base->get());
			}

			if(IS_ACCESS_TYPE(token.get_text())) {
				token.set_as_token(TOKEN_TYPE_ACCESS, INVALID_TYPE);
			} else if(IS_ARGUMENT_TYPE(token.get_text())) {
				token.set_as_token(TOKEN_TYPE_ARGUMENT, INVALID_TYPE);
			} else if(IS_ASSIGNMENT_TYPE(token.get_text())) {
				token.set_as_token(TOKEN_TYPE_ASSIGNMENT, INVALID_TYPE);
			} else if(IS_BINARY_OPERATOR_TYPE(token.get_text())) {
				token.set_as_token(TOKEN_TYPE_BINARY_OPERATOR, INVALID_TYPE);
			} else if(IS_CALL_TYPE(token.get_text())) {
				token.set_as_token(TOKEN_TYPE_CALL, INVALID_TYPE);
			} else if(IS_COMPARATOR_TYPE(token.get_text())) {
				token.set_as_token(TOKEN_TYPE_COMPARATOR, INVALID_TYPE);
			} else if(IS_CONDITIONAL_SEPERATOR_TYPE(token.get_text())) {
				token.set_as_token(TOKEN_TYPE_CONDITIONAL_SEPERATOR, INVALID_TYPE);
			} else if(IS_CONDITIONAL_TYPE(token.get_text())) {
				token.set_as_token(TOKEN_TYPE_CONDITIONAL, INVALID_TYPE);
			} else if(IS_CONSTANT_TYPE(token.get_text())) {
				token.set_as_token(TOKEN_TYPE_CONSTANT, INVALID_TYPE);
			} else if(IS_CONTROL_TYPE(token.get_text())) {
				token.set_as_token(TOKEN_TYPE_CONTROL, INVALID_TYPE);
			} else if(IS_CONVERSION_OPERATOR_TYPE(token.get_text())) {
				token.set_as_token(TOKEN_TYPE_CONVERSION_OPERATOR, INVALID_TYPE);
			} else if(IS_FILE_STREAM_OPERATOR_TYPE(token.get_text())) {
				token.set_as_token(TOKEN_TYPE_FILE_STREAM_OPERATOR, INVALID_TYPE);
			} else if(IS_IN_LIST_OPERATOR_TYPE(token.get_text())) {
				token.set_as_token(TOKEN_TYPE_IN_LIST_OPERATOR, INVALID_TYPE);
			} else if(IS_IO_OPERATOR_TYPE(token.get_text())) {
				token.set_as_token(TOKEN_TYPE_IO_OPERATOR, INVALID_TYPE);
			} else if(IS_ITERATOR_OPERATOR_TYPE(token.get_text())) {
				token.set_as_token(TOKEN_TYPE_ITERATOR_OPERATOR, INVALID_TYPE);
			} else if(IS_LOGICAL_OPERATOR_TYPE(token.get_text())) {
				token.set_as_token(TOKEN_TYPE_LOGICAL_OPERATOR, INVALID_TYPE);
			} else if(IS_OUT_LIST_OPERATOR_TYPE(token.get_text())) {
				token.set_as_token(TOKEN_TYPE_OUT_LIST_OPERATOR, INVALID_TYPE);
			} else if(IS_STREAM_TYPE(token.get_text())) {
				token.set_as_token(TOKEN_TYPE_STREAM, INVALID_TYPE);
			} else if(IS_USING_TYPE(token.get_text())) {
				token.set_as_token(TOKEN_TYPE_USING, INVALID_TYPE);
			}

			if(!IS_TYPE_T(token, TOKEN_TYPE_IDENTIFIER)) {
				token.set_as_token(token.get_type(), lang_find_subtype(token.get_text(), token.get_type()));
				token.set_value(std::string());
			}
		} else {
			TRACE_EVENT("Invalid alpha symbol: " << base->to_string(true), TRACE_TYPE_ERROR);
			THROW_LEX_EXT_EXC_W_MESS(base->to_string(false), LEX_EXT_EXC_INVAL_ALPHA_SYMBOL);
		}
	}

	TRACE_EVENT("-lex_enum_alpha", TRACE_TYPE_INFORMATION);
}

void 
lex_enum_number(
	lex_ptr lexer, 
	tok &token
	)
{
	TRACE_EVENT("+lex_enum_number", TRACE_TYPE_INFORMATION);

	bool has_decimal = false;
	lex_base_ptr base = (lex_base_ptr) lexer;

	if(base) {
		token.set_as_token(TOKEN_TYPE_INTEGER, INVALID_TYPE);

		if(IS_TYPE_LB(base, SYMBOL_BASE_TYPE_DIGIT)) {
			token.set_value(token.get_text() + base->get());

			while(base->has_next()) {
				base->move_next();

				if(!IS_TYPE_LB(base, SYMBOL_BASE_TYPE_DIGIT)) {
					break;
				}
				token.set_value(token.get_text() + base->get());
			}

			if(IS_CHAR_LB(base, SYMBOL_DECIMAL)) {
				token.set_value(token.get_text() + base->get());
				has_decimal = true;

				while(base->has_next()) {
					base->move_next();

					if(!IS_TYPE_LB(base, SYMBOL_BASE_TYPE_DIGIT)) {
						break;
					}
					token.set_value(token.get_text() + base->get());
				}
			}
		} else {
			TRACE_EVENT("Invalid digit symbol: " << base->to_string(true), TRACE_TYPE_ERROR);
			THROW_LEX_EXT_EXC_W_MESS(base->to_string(false), LEX_EXT_EXC_INVAL_DIGIT_SYMBOL);
		}

		if(has_decimal) {
			token.set_as_token(TOKEN_TYPE_FLOAT, INVALID_TYPE);
		}
	}

	TRACE_EVENT("-lex_enum_number", TRACE_TYPE_INFORMATION);
}

void 
lex_enum_symbol(
	lex_ptr lexer, 
	tok &token
	)
{
	TRACE_EVENT("+lex_enum_symbol", TRACE_TYPE_INFORMATION);

	bool found_end = false;
	lex_base_ptr base = (lex_base_ptr) lexer;
	std::string symbol(1, base->get()), first_delim;

	if(base) {

		if(IS_SYMBOL_TYPE(symbol)) {
			token.set_as_token(TOKEN_TYPE_SYMBOL, lang_find_subtype(symbol, TOKEN_TYPE_SYMBOL));

			if(base->has_next()) {
				base->move_next();
			}
		} else {
		
			switch(base->get()) {
				case SYMBOL_STRING_DELIM:
					token.set_as_token(TOKEN_TYPE_VAR_STRING, INVALID_TYPE);
					first_delim = base->to_string(true);

					if(base->has_next()) {
						base->move_next();

						while(base->has_next()) {
						
							if(IS_CHAR_LB(base, SYMBOL_STRING_DELIM)) {
								found_end = true;
								break;
							}
							token.set_value(token.get_text() + base->get());
							base->move_next();
						}

						if(base->has_next()) {
							base->move_next();
						}
					}

					if(!found_end) {
						TRACE_EVENT("Unterminated string detected at position: " << first_delim, TRACE_TYPE_ERROR);
						THROW_LEX_EXT_EXC_W_MESS(first_delim, LEX_EXT_EXC_UNTERM_STRING);
					}
					break;
				case SYMBOL_UNDERSCORE:
					lex_enum_alpha(lexer, token);
					break;
				default:
					TRACE_EVENT("Invalid symbol: " << base->to_string(true), TRACE_TYPE_ERROR);
					THROW_LEX_EXT_EXC_W_MESS(base->to_string(false), LEX_EXT_EXC_INVAL_SYMBOL);
					break;
			}
		}
	}

	TRACE_EVENT("-lex_enum_symbol", TRACE_TYPE_INFORMATION);
}

void lex_skip_whitespace(
	lex_ptr lexer
	)
{
	TRACE_EVENT("+lex_skip_whitespace", TRACE_TYPE_VERBOSE);

	lex_base_ptr base = (lex_base_ptr) lexer;

	if(base) {

		while(base->has_next()) {

			if(!IS_TYPE_LB(base, SYMBOL_BASE_TYPE_WHITESPACE)) {
				break;
			}
			base->move_next();
		}

		if(IS_CHAR_LB(base, SYMBOL_COMMENT)) {

			while(base->has_next()) {

				if(IS_TYPE_LB(base, SYMBOL_BASE_TYPE_WHITESPACE)
						&& IS_CHAR_LB(base, WS_NEWLINE)) {
					break;
				}
				base->move_next();
			}

			if(base->has_next()) {
				base->move_next();
			}
			lex_skip_whitespace(lexer);
		}
	}

	TRACE_EVENT("-lex_skip_whitespace", TRACE_TYPE_VERBOSE);
}