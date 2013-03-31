/*
 * arg.h
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

#ifndef ARG_H_
#define ARG_H_

#include <string>
#include "..\nblang\tok_type.h"

/*
 * Argument class
 */
typedef class _arg {

	public:

		/*
		 * Argument constructor
		 */
		_arg(void);

		/*
		 * Argument constructor
		 * @param type argument type
		 */
		_arg(
			size_t type
			);

		/*
		 * Argument constructor
		 * @param text argument text reference
		 * @param type argument type
		 */
		_arg(
			const std::string text,
			size_t type
			);

		/*
		 * Argument constructor
		 * @param other argument object
		 */
		_arg(
			const _arg &other
			);

		/*
		 * Argument destructor
		 */
		virtual ~_arg(void);

		/*
		 * Argument assignment operator
		 * @param other argument object reference
		 * @return argument object refence
		 */
		_arg &operator=(
			const _arg &other
			);

		/*
		 * Argument equals operator
		 * @param other argument object reference
		 * @return true if equals, false otherwise
		 */
		bool operator==(
			const _arg &other
			);

		/*
		 * Argument not equals operator
		 * @param other argument object reference
		 * @return true if not equals, false otherwise
		 */
		bool operator!=(
			const _arg &other
			);

		/*
		 * Clear argument
		 */
		void clear(void);

		/*
		 * Clear argument and set type
		 * @param type argument type
		 */
		void clear(
			size_t type
			);

		/*
		 * Retrieve argument text
		 * @return argument text
		 */
		std::string get_text(void);

		/* 
		 * Retrieve argument type
		 * @return argument type
		 */
		size_t get_type(void);

		/*
		 * Retrieve argument text status
		 * @return true if text not empty, false otherwise
		 */
		bool has_text(void);

		/*
		 * Set argument text
		 * @param text argument text reference
		 */
		void set_text(
			const std::string &text
			);

		/*
		 * Set argument type
		 * @param type argument type
		 */
		void set_type(
			size_t type
			);

		/*
		 * Retrieve string represenation
		 * @param verbose verbose string status
		 * @return string represenation
		 */
		std::string to_string(
			bool verbose
			);

		/*
		 * Retrieve token representation
		 * @return token representation
		 */
		tok to_token(void);

	protected:

		/*
		 * Token text
		 */
		std::string _text;

		/*
		 * Token type
		 */
		size_t _type;

} arg, *arg_ptr;

#endif