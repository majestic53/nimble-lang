/*
 * tok.h
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

#ifndef TOK_H_
#define TOK_H_

#include <string>

/*
 * Token class
 */
typedef class _tok {

	public:

		/*
		 * Token constructor
		 */
		_tok(void);

		/*
		 * Token constructor
		 * @param class_type token class type
		 */
		_tok(
			size_t class_type
			);

		/*
		 * Token constructor
		 * @param other token object reference
		 */
		_tok(
			const _tok &other
			);

		/*
		 * Token destructor
		 */
		virtual ~_tok(void);

		/*
		 * Token assignment operator
		 * @param other token object reference
		 * @return token object reference
		 */
		_tok &operator=(
			const _tok &other
			);

		/*
		 * Ceiling token value
		 */
		void ceiling(void);

		/*
		 * Clear token
		 */
		void clear(void);

		/*
		 * Floor token value
		 */
		void floor(void);

		/*
		 * Retrieve token class type
		 * @return token class type
		 */
		size_t get_class_type(void);

		/*
		 * Retrieve token line number
		 * @return token line number
		 */
		size_t get_line(void);

		/*
		 * Retrieve token line position
		 * @return token line position
		 */
		size_t get_position(void);

		/*
		 * Retrieve token subtype
		 * @return token subtype
		 */
		size_t get_subtype(void);

		/*
		 * Retrieve token type
		 * @return token type
		 */
		size_t get_type(void);

		/*
		 * Retrieve token text reference
		 * @return token text reference
		 */
		std::string &get_text(void);

		/*
		 * Negate token value
		 */
		void negate(void);

		/*
		 * Round token value
		 */
		void round(void);

		/*
		 * Set token as type of a given subtype
		 * @param type token type
		 * @param subtype token subtype
		 */
		void set_as_token(
			size_t type,
			size_t subtype
			);

		/*
		 * Set token class type
		 * @param class_type token class type
		 */
		void set_class_type(
			size_t class_type
			);

		/*
		 * Set token line number
		 * @param line token line number
		 */
		void set_line(
			size_t line
			);

		/*
		 * Set token line position
		 * @param line token line position
		 */
		void set_position(
			size_t position
			);

		/* 
		 * Set token subtype
		 * @param subtype token subtype
		 */
		void set_subtype(
			size_t subtype
			);

		/* 
		 * Set token type
		 * @param type token type
		 */
		void set_type(
			size_t type
			);

		/*
		 * Shift left token value by a given amount
		 * @param amount amount to shift by
		 */
		void shift_left(
			size_t amount
			);

		/*
		 * Shift right token value by a given amount
		 * @param amount amount to shift by
		 */
		void shift_right(
			size_t amount
			);

		/*
		 * Retrieve float representation of token value
		 */
		double to_float(void);

		/*
		 * Retrieve integer representation of token value
		 */
		long to_integer(void);

		/*
		 * Retrieve string represenation
		 * @param verbose verbose string status
		 * @return string represenation
		 */
		std::string to_string(
			bool verbose
			);

	protected:

		/*
		 * Token type information
		 */
		size_t _class_type;
		size_t _line;
		size_t _position;
		size_t _subtype;
		size_t _type;

		/*
		 * Token text
		 */
		std::string _text;

} tok, *tok_ptr;

#endif