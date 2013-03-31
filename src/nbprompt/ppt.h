/*
 * ppt.h
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

#ifndef PPT_H_
#define PPT_H_

#include "sch.h"
#include "..\nblang\exe.h"
#include "..\nblang\exe_ext.h"

/*
 * Prompt class
 */
typedef class _ppt {

	public:
		
		/*
		 * Prompt constructor
		 */
		_ppt(void);

		/*
		 * Prompt constructor
		 * @param other prompt object reference
		 */
		_ppt(
			const _ppt &other
			);

		/*
		 * Prompt destructor
		 */
		virtual ~_ppt(void);

		/*
		 * Prompt assignment operator
		 * @param other prompt object reference
		 * @return prompt object refence
		 */
		_ppt &operator=(
			const _ppt &other
			);

		/*
		 * Clear prompt
		 */
		void clear(void);

		/*
		 * Retrieve registered command schema count
		 * @return registered command schema count
		 */
		size_t get_schema_count(void);

		/*
		 * Retrieve argument action status
		 * @return true if assigned, false otherwise
		 */
		bool has_argument_action(void);

		/*
		 * Retrieve exception action status
		 * @return true if assigned, false otherwise
		 */
		bool has_exeception_action(void);

		/*
		 * Retrieve generic action status
		 * @return true if assigned, false otherwise
		 */
		bool has_generic_action(void);

		/*
		 * Retrieve registered command schema status
		 * @return true if registered, false otherwise
		 */
		bool has_schemas(void);

		/*
		 * Prompt active status
		 * @return true if active, false otherwise
		 */
		bool is_active(void);

		/*
		 * Register command schema
		 * @param schema command schema reference
		 */
		void register_schema(
			sch &schema
			);

		/*
		 * Assign argument action
		 * @param ppt_arg_action argument action
		 */
		void set_argument_action(
			ppt_arg_action argument_action
			);

		/*
		 * Assign exception action
		 * @param ppt_exc_action exception action
		 */
		void set_exception_action(
			ppt_exc_action exception_action
			);

		/*
		 * Assign generic action
		 * @param generic_action generic action
		 */
		void set_generic_action(
			ppt_action generic_action
			);

		/*
		 * Start prompt
		 * @param arguments command line arguments
		 * @param initialization initialization action
		 */
		void start(
			std::vector<std::string> &arguments,
			ppt_init_action initialization
			);

		/*
		 * Stop prompt
		 */
		void stop(void);

		/*
		 * Retrieve string represenation
		 * @param verbose verbose string status
		 * @return string represenation
		 */
		std::string to_string(
			bool verbose
			);

		/*
		 * Unregister all command schemas
		 */
		void unregister_all_schema(void);

		/*
		 * Unregister command schema
		 * @param schema command schema reference
		 */
		void unregister_schema(
			sch &schema
			);

	protected:

		/*
		 * Retrieve registered command schema
		 * @param schema command schema reference
		 */
		std::vector<sch>::iterator _get_registered_schema(
			const sch &schema
			);

		/*
		 * Retrieve registered command schema status
		 * @param schema command schema reference
		 * @return true if registered, false otherwise
		 */
		bool _has_registered_schema(
			const sch &schema
			);

		/*
		 * Invoke executor instance
		 * @param arguments argument list reference
		 */
		void _invoke_executor(
			in_arg &arguments
			);

		/*
		 * Invoke executor instance
		 * @param input command input reference
		 * @param arguments argument list reference
		 */
		void _invoke_executor(
			const std::string &input,
			const tok_vector &arguments,
			bool is_file
			);

		/*
		 * Invoke prompt instance
		 * @param arguments argument list reference
		 */
		void _invoke_prompt(
			std::vector<std::string> &arguments
			);

		/*
		 * Parse command input
		 * @param input command input reference
		 * @param arguments argument list reference
		 */
		void _parse_command_input(
			const std::string &input,
			std::vector<arg> &arguments
			);

		/*
		 * Prompt active status
		 */
		bool _active;

		/*
		 * Prompt actions
		 */
		ppt_arg_action _argument_action;
		ppt_exc_action _exception_action;
		ppt_action _generic_action;

		/*
		 * Prompt registered command schema list
		 */
		std::vector<sch> _schema;

} ppt, *ppt_ptr;

#endif