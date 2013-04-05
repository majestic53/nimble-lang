/*
 * exe.h
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

#ifndef EXE_H_
#define EXE_H_

#include <vector>
#include "exe_type.h"
#include "par.h"

/*
 * Executor class
 */
typedef class _exe :
		public par {

	public:

		/*
		 * Executor constructor
		 */
		_exe(void);

		/*
		 * Executor constructor
		 * @param other executor object reference
		 */
		_exe(
			const _exe &other
			);

		/*
		 * Executor destructor
		 */
		virtual ~_exe(void);

		/*
		 * Executor assignment operator
		 * @param other executor object reference
		 * @return executor object reference
		 */
		_exe &operator=(
			const _exe &other
			);

		/*
		 * Clear stack
		 */
		void clear_stack(void);

		/*
		 * Evaluate all statements
		 */
		void evaluate(void);

		/*
		 * Retrieve current executor context
		 * @return context object reference
		 */
		cont &get(void);

		/*
		 * Retrieve argument as a given index
		 * @param index argument index
		 * @return token object reference
		 */
		tok &get_argument(
			size_t index
			);

		/*
		 * Retrieve argument count
		 * @return argument count
		 */
		size_t get_argument_count(void);

		/*
		 * Retrieve arguments
		 * @return argument vector object reference
		 */
		tok_vector &get_arguments(void);

		/*
		 * Retrieve stack
		 * @return stack object reference
		 */
		tok_stack &get_stack(void);

		/*
		 * Retrieve next statement status
		 * @return true if has next statement, false otherwise
		 */
		bool has_next(void);

		/*
		 * Initialize executor with given input
		 * @param input input string reference
		 * @param is_file true if input is file path, false otherwise
		 * @param clear_scope true to clear executor scope, false otherwise
		 */
		void initialize(
			const std::string &input,
			bool is_file,
			bool clear_scope
			);

		/*
		 * Initialize executor with given input
		 * @param input input string reference
		 * @param arguments input arguments
		 * @param is_file true if input is file path, false otherwise
		 * @param clear_scope true to clear executor scope, false otherwise
		 */
		void initialize(
			const std::string &input,
			const tok_vector &arguments,
			bool is_file,
			bool clear_scope
			);

		/*
		 * Reset executor
		 * @param clear_scope true to clear executor scope, false otherwise
		 */
		void reset(
			bool clear_scope
			);

		/*
		 * Set executor action type
		 * @param action action callback
		 * @param action type
		 */
		void set_action(
			exe_action action,
			size_t type
			);

		/*
		 * Set executor evaluation action type
		 * @param action evaluation action callback
		 * @param type enumeration action type
		 */
		void set_evaluation_action(
			exe_eval_action action,
			size_t type
			);

		/*
		 * Evaluate next statement
		 * @return control signal
		 */
		size_t step(void);

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
		 * Evaluation scope tree
		 */
		cont_tree _scope;

		/*
		 * Evaluation stack
		 */
		tok_stack _stack;

		/*
		 * Input arguments
		 */
		tok_vector _arguments;

		/*
		 * Executor action types
		 */
		exe_action _actions[MAX_EXE_ACTION_TYPE + 1];
		exe_eval_action _eval_actions[MAX_EXE_EVAL_ACTION_TYPE + 1];

		/*
		 * Invoke executor action callback
		 * @param type action type
		 */
		void _invoke_action(
			size_t type
			);

		/*
		 * Invoke executor evaluation action callback
		 * @param type action type
		 * @return control signal
		 */
		size_t _invoke_evaluation_action(
			size_t type
			);

} exe, *exe_ptr;

#endif