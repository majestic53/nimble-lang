/*
 * par.h
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

#ifndef PAR_H_
#define PAR_H_

#include "lex.h"
#include "par_type.h"

/*
 * Parser class
 */
typedef class _par :
		public lex {

	public:

		/*
		 * Parser constructor
		 */
		_par(void);

		/*
		 * Parser constructor
		 * @param other parser object reference
		 */
		_par(
			const _par &other
			);

		/*
		 * Parser destructor
		 */
		virtual ~_par(void);

		/*
		 * Parser assignment operator
		 * @param other parser object reference
		 */
		_par &operator=(
			const _par &other
			);

		/*
		 * Clear parser
		 */
		void clear(void);

		/*
		 * Enumerate all statements
		 */
		void enumerate(void);

		/*
		 * Export statement at a given index
		 * @param index statement index
		 * @return statement object reference
		 */
		tok_stmt & _par::export_statement(
			size_t index
			);

		/*
		 * Export all statements
		 * @return statement object reference
		 */
		tok_stmt_list &export_statements(void);

		/* 
		 * Retrieve current statement
		 * @return statement object reference
		 */
		tok_stmt &get(void);

		/*
		 * Retrieve current statement count
		 * @return statement count
		 */
		size_t get_count(void);

		/*
		 * Retrieve current statement position
		 * @return statement position
		 */
		size_t get_position(void);
		
		/*
		 * Retrieve next statement status
		 * @return true if has next statement, false otherwise
		 */
		bool has_next(void);
		
		/*
		 * Retrieve previous statement status
		 * @return true if has previous statement, false otherwise
		 */
		bool has_previous(void);
		
		/*
		 * Retrieve statement status
		 * @return true if parser has statements, false otherwise
		 */
		bool has_statements(void);

		/*
		 * Import statements
		 * @param statements statement list object reference
		 */
		void import_statements(
			tok_stmt_list &statements
			);

		/*
		 * Initialize parser with given input
		 * @param input input string reference
		 * @param is_file true if input is file path, false otherwise
		 */
		void initialize(
			const std::string &input,
			bool is_file
			);

		/*
		 * Move to next statement
		 */
		void move_next(void);
		
		/*
		 * Move to previous statement
		 */
		void move_previous(void);

		/*
		 * Reset parser
		 */
		void reset(void);

		/*
		 * Set parser action type
		 * @param action action callback
		 * @param action type
		 */
		void set_action(
			par_action action,
			size_t type
			);

		/*
		 * Set parser enumeration action type
		 * @param action enumeration action callback
		 * @param type enumeration action type
		 */
		void set_enumeration_action(
			par_enum_action action,
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
		 * Statement position
		 */
		size_t _position;

		/*
		 * Statement list
		 */
		tok_stmt_list _statements;

		/*
		 * Parser action types
		 */
		par_action _actions[MAX_PAR_ACTION_TYPE + 1];
		par_enum_action _enum_actions[MAX_PAR_ENUM_ACTION_TYPE + 1];

		/*
		 * Invoke parser action callback
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
			tok_stmt &statement,
			size_t type
			);

} par, *par_ptr;

#endif