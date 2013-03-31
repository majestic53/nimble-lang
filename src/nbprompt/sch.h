/*
 * sch.h
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

#ifndef SCH_H_
#define SCH_H_

#include <vector>
#include "arg.h"
#include "ppt_type.h"

/*
 * Command schema class
 */
typedef class _sch {

	public:

		/*
		 * Command schema constructor
		 */
		_sch(void);

		/*
		 * Command schema constructor
		 * @param command command string reference
		 */
		_sch(
			const std::string &command
			);

		/*
		 * Command schema constructor
		 * @param other command schema object reference
		 */
		_sch(
			const _sch &other
			);

		/*
		 * Command schema destructor
		 */
		virtual ~_sch(void);

		/*
		 * Command schema assignment operator
		 * @param other command schema object reference
		 * @return command schema object refence
		 */
		_sch &operator=(
			const _sch &other
			);

		/*
		 * Command schema equals operator
		 * @param other command schema object reference
		 * @return true if equal, false otherwise
		 */
		bool operator==(
			const _sch &other
			);

		/*
		 * Command schema equals operator
		 * @param other command schema object reference
		 * @return true if not equal, false otherwise
		 */
		bool operator!=(
			const _sch &other
			);

		/*
		 * Insert an argument at the end of the argumenet list
		 * @param argument argument reference
		 */
		void add_argument_back(
			const arg &argument
			);

		/*
		 * Insert an argument at the begining of the argumenet list
		 * @param argument argument reference
		 */
		void add_argument_front(
			const arg &argument
			);

		/*
		 * Clear schema
		 */
		void clear(void);

		/*
		 * Clear schema with command string
		 * @param command command string reference
		 */
		void clear(
			const std::string &command
			);

		/*
		 * Retrieve argument at a given index
		 * @param index argument index
		 * @return argument at a given index
		 */
		arg get_argument(
			size_t index
			);

		/*
		 * Retrieve registered argument count
		 * @return registered argument count
		 */
		size_t get_argument_count(void);

		/*
		 * Retrieve command string
		 * @return command string
		 */
		std::string get_command(void);

		/*
		 * Retrieve prompt action status
		 * @return true if assigned, false otherwise
		 */
		bool has_action(void);

		/*
		 * Retrieve argument list status
		 * @return true if not empty, false otherwise
		 */
		bool has_arguments(void);

		/*
		 * Insert an argument after a given index
		 * @param argument argument reference
		 * @param index argument index
		 */
		void insert_argument(
			const arg &argument,
			size_t index
			);

		/*
		 * Invoke prompt action
		 * @param prom prompt object pointer
		 * @param arguments argument list reference
		 */
		void invoke_action(
			ppt_ptr prom,
			std::vector<arg> &arguments
			);

		/*
		 * Retrieve wildcard status
		 * @return true if wildcard, false otherwise
		 */
		bool is_wildcard(void);

		/*
		 * Match command against schema
		 * @param command command string reference
		 * @param arguments argument list reference
		 * @return true if match, false otherwise
		 */
		bool match(
			const std::string &command,
			const std::vector<arg> &arguments
			);

		/*
		 * Unregister all arguments
		 */
		void remove_all_arguments(void);

		/* 
		 * Unregister argument at a given index
		 * @param index argument index
		 */
		void remove_argument(
			size_t index
			);

		/*
		 * Unregister an argument at the end of the argumenet list
		 */
		void remove_argument_back(void);

		/*
		 * Unregister an argument at the begining of the argumenet list
		 */
		void remove_argument_front(void);

		/*
		 * Assign prompt action
		 * @parma action prompt action
		 */
		void set_action(
			ppt_action action
			);

		/*
		 * Insert argument at a given index
		 * @param argument argument reference
		 * @param index argument index
		 */
		void set_argument(
			const arg &argument,
			size_t index
			);

		/*
		 * Set wildcard status
		 * @param wildcard wildcard status
		 */
		void set_as_wildcard(
			bool wildcard
			);

		/*
		 * Set command string
		 * @param command string reference
		 */
		void set_command(
			const std::string &command
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
		 * Prompt action
		 */
		ppt_action _action;

		/*
		 * Argument list
		 */
		std::vector<arg> _arguments;

		/*
		 * Command string
		 */
		std::string _command;

		/*
		 * Wildcard status
		 */
		bool _wildcard;

} sch, *sch_ptr;

#endif