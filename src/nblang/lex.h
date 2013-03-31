/*
 * lex.h
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

#ifndef LEX_H_
#define LEX_H_

#include <vector>
#include "cont.h"
#include "lex_base.h"
#include "lex_type.h"
#include "tok.h"

/*
 * Lexer class
 */
typedef class _lex :
		public lex_base {

	public:

		/*
		 * Lexer constructor
		 */
		_lex(void);

		/*
		 * Lexer constructor
		 * @param other lexer object reference
		 */
		_lex(
			const _lex &other
			);

		/*
		 * Lexer destructor
		 */
		virtual ~_lex(void);

		/*
		 * Lexer assignment operator
		 * @param other lexer object reference
		 */
		_lex &operator=(
			const _lex &other
			);

		/*
		 * Clear lexer
		 */
		void clear(void);

		/*
		 * Enumerate all tokens
		 */
		void enumerate(void);

		/* 
		 * Retrieve current token
		 * @return token object reference
		 */
		_tok &get(void);

		/*
		 * Retrieve current token count
		 * @return token count
		 */
		size_t get_count(void);

		/*
		 * Retrieve current token position
		 * @return token position
		 */
		size_t get_position(void);
		
		/*
		 * Retrieve next token status
		 * @return true if has next token, false otherwise
		 */
		bool has_next(void);
		
		/*
		 * Retrieve previous token status
		 * @return true if has previous token, false otherwise
		 */
		bool has_previous(void);
		
		/*
		 * Retrieve token text status
		 * @return true if token has text, false otherwise
		 */
		bool has_text(void);

		/*
		 * Retrieve token status
		 * @return true if lexer has token, false otherwise
		 */
		bool has_tokens(void);

		/*
		 * Insert class token type into token list
		 * @param class_type token class type
		 */
		void insert_class_token(
			size_t class_type
			);

		/*
		 * Move to next token
		 */
		void move_next(void);
		
		/*
		 * Move to previous token
		 */
		void move_previous(void);

		/*
		 * Reset lexer
		 */
		void reset(void);

		/*
		 * Set lexer action type
		 * @param action action callback
		 * @param action type
		 */
		void set_action(
			lex_action action,
			size_t type
			);

		/*
		 * Set lexer enumeration action type
		 * @param action enumeration action callback
		 * @param type enumeration action type
		 */
		void set_enumeration_action(
			lex_enum_action action,
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

	protected:

		/*
		 * Token position
		 */
		size_t _position;

		/* 
		 * Enumerated tokens
		 */
		std::vector<tok> _tokens;

		/*
		 * Lexer action types
		 */
		lex_action _actions[MAX_LEX_ACTION_TYPE + 1];
		lex_enum_action _enum_actions[MAX_LEX_ENUM_ACTION_TYPE + 1];

		/*
		 * Invoke lexer action callback
		 * @param type action type
		 */
		void _invoke_action(
			size_t type
			);

		/*
		 * Invoke lexer enumeration action callback
		 * @param token token object reference
		 * @param type action type
		 */
		void _invoke_enumeration_action(
			_tok &token,
			size_t type
			);

} lex, *lex_ptr;

#endif