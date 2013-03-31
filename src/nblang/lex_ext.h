/*
 * lex_ext.h
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

#ifndef LEX_EXT_H_
#define LEX_EXT_H_

#include "lex_type.h"

/*
 * Lexer extension enumeration routines
 */
extern void lex_enum_alpha(
	lex_ptr lexer, 
	tok &token
	);

extern void lex_enum_number(
	lex_ptr lexer, 
	tok &token
	);

extern void lex_enum_symbol(
	lex_ptr lexer, 
	tok &token
	);

extern void lex_skip_whitespace(
	lex_ptr lexer
	);

#endif