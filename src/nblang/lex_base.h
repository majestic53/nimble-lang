/*
 * lex_base.h
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

#ifndef LEX_BASE_H_
#define LEX_BASE_H_

#include "lex_base_type.h"

/*
 * Lexer base class
 */
typedef class _lex_base {	
	public:
	
		/*
		 * Lexer base constructor
		 */
		_lex_base(void);
		
		/*
		 * Lexer base constructor
		 * @param input input string reference
		 * @param is_file true if input is file path, false otherwise
		 */
		_lex_base(
			const std::string &input,
			bool is_file
			);
			
		/*
		 * Lexer base constructor
		 * @param other lexer base object reference
		 */
		_lex_base(
			const _lex_base &other
			);
			
		/*
		 * Lexer destructor
		 */
		virtual ~_lex_base(void);
		
		/*
		 * Lexer assignment operator
		 * @param other lexer base object reference
		 * @return lexer base object reference
		 */
		_lex_base &operator=(
			const _lex_base &other
			);
			
		/*
		 * Clear lexer base
		 */
		void clear(void);
		
		/*
		 * Retrieve lexer base character
		 * @return lexer base character reference
		 */
		char &get(void);

		/* 
		 * Retrieve lexer base absolute character position
		 * @return lexer base absolute character position
		 */
		size_t get_absolute_position(void);

		/* 
		 * Retrieve lexer base character base type
		 * @return lexer base character base type
		 */
		size_t get_base_type(void);

		/*
		 * Retrieve lexer base character count
		 * @return lexer base character count
		 */
		size_t get_count(void);
		
		/*
		 * Retrieve lexer base line number
		 * @return lexer base line number
		 */
		size_t get_line(void);
		
		/* 
		 * Retrieve lexer base relative character position
		 * @return lexer base relative character position
		 */
		size_t get_relative_position(void);
		
		/*
		 * Retrieve lexer base next character status
		 * @return true if has next character, false otherwise
		 */
		bool has_next(void);
		
		/*
		 * Retrieve lexer base previous character status
		 * @return true if has previous character, false otherwise
		 */
		bool has_previous(void);
		
		/*
		 * Initialize lexer base with given input
		 * @param input input string reference
		 * @param is_file true if input is file path, false otherwise
		 */
		void initialize(
			const std::string &input,
			bool is_file
			);

		/*
		 * Move lexer base to next character
		 */
		void move_next(void);
		
		/*
		 * Move lexer base to previous character
		 */
		void move_previous(void);
		
		/*
		 * Reset lexer base
		 */
		void reset(void);
	
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
		 * Current line number
		 */
		size_t _line;
		
		/* 
		 * Absolute character position
		 */
		long long _absolute_position;
		
		/*
		 * Relative character position
		 */
		long long _relative_position;

		/*
		 * Current character information
		 */
		size_t _base_type;
		std::string _input;
	
} lex_base, *lex_base_ptr;

#endif