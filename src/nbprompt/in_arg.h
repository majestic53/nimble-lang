/*
 * in_arg.h
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

#ifndef IN_ARG_H_
#define IN_ARG_H_

#include <string>
#include <vector>
#include "arg.h"

/*
 * Input argument class
 */
typedef class _in_arg {

	public:

		/*
		 * Input argument constructor
		 */
		_in_arg(void);

		/*
		 * Input argument constructor
		 * @param input string reference
		 * @param is_file input string as a file
		 */
		_in_arg(
			const std::string &input,
			bool is_file
			);

		/*
		 * Input argument constructor
		 * @param other input argument object reference
		 */
		_in_arg(
			const _in_arg &other
			);

		/*
		 * Input argument destructor
		 */
		virtual ~_in_arg(void);

		/*
		 * Input argument assignment operator
		 * @param other argument object reference
		 * @return argument object reference
		 */
		_in_arg &operator=(
			const _in_arg &other
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
		 * Clear input argument
		 */
		void clear(void);

		/*
		 * Clear input argument with input string
		 * @param input input string reference
		 * @param is_file input string as a file
		 */
		void clear(
			const std::string &input,
			bool is_file
			);

		/*
		 * Retrieve argument at a given index
		 * @param index argument index
		 * @return argument at a given index
		 */
		arg &get_argument(
			size_t index
			);

		/*
		 * Retrieve registered argument count
		 * @return registered argument count
		 */
		size_t get_argument_count(void);

		/*
		 * Retrieve input string
		 * @return input string reference
		 */
		std::string &get_input(void);

		/*
		 * Retrieve input argument signal
		 * @return input argument signal
		 */
		size_t get_signal(void);

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
		 * Retrieve input string file status
		 * @return true if file, false otherwise
		 */
		bool is_file(void);

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
		 * Insert argument at a given index
		 * @param argument argument reference
		 * @param index argument index
		 */
		void set_argument(
			const arg &argument,
			size_t index
			);

		/*
		 * Set input string as file
		 * @param is_file true if file, false otherwise
		 */
		void set_as_file(
			bool is_file
			);

		/*
		 * Set input string
		 * @parma input string reference
		 */
		void set_input(
			const std::string &input
			);

		/*
		 * Set input signal
		 * @param signal input signal
		 */
		void set_signal(
			size_t signal
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
		 * Argument list
		 */
		std::vector<arg> _arguments;

		/*
		 * Input string
		 */
		std::string _input;

		/*
		 * Input string file status
		 */
		bool _is_file;

		/*
		 * Input signal
		 */
		size_t _signal;

} in_arg, *in_arg_ptr;

#endif