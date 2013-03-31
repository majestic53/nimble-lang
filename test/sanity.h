/*
 * sanity.h
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

#ifndef SANITY_H_
#define SANITY_H_

#include "..\src\nblang\exe_type.h"

/*
 * Invoke executor for testing purposes
 * @param input input string reference
 * @param arguments input arguments reference
 * @param is_file true if input string is a file path, false otherwise
 */
extern void exe_test(
	const std::string &input,
	const tok_vector &arguments,
	bool is_file
	);

/*
 * Invoke lexer base for testing purposes
 * @param input input string reference
 * @param is_file true if input string is a file path, false otherwise
 */
extern void lex_base_test(
	const std::string &input,
	bool is_file
	);

/*
 * Invoke lexer for testing purposes
 * @param input input string reference
 * @param is_file true if input string is a file path, false otherwise
 */
extern void lex_test(
	const std::string &input,
	bool is_file
	);

/*
 * Invoke parser for testing purposes
 * @param input input string reference
 * @param is_file true if input string is a file path, false otherwise
 */
extern void par_test(
	const std::string &input,
	bool is_file
	);

#endif