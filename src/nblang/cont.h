/*
 * cont.h
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

#ifndef CONT_H_
#define CONT_H_

#include <map>
#include <vector>
#include "list.h"
#include "node.h"
#include "tok.h"
#include "tree.h"

/*
 * Common templated types
 */
typedef _tree<tok> tok_stmt, *tok_stmt_ptr;
typedef _node<tok> tok_node, *tok_node_ptr;
typedef _list<tok_stmt> tok_stmt_list, *tok_stmt_list_ptr;
typedef std::vector<tok_stmt> tok_stmt_vector, *tok_stmt_vector_ptr;

/*
 * Context class
 */
typedef class _cont {

	public:

		/*
		 * Context constructor
		 */
		_cont(void);

		/*
		 * Context constructor
		 * @param other context object reference
		 */
		_cont(
			const _cont &other
			);

		/*
		 * Context destructor
		 */
		virtual ~_cont(void);

		/*
		 * Context assignment operator
		 * @param other context object reference
		 */
		_cont &operator=(
			const _cont &other
			);

		/*
		 * Clear context
		 */
		void clear(void);

		/*
		 * Retrieve context value at a given key string
		 * @param key key string reference
		 * @return context value at a given key
		 */
		tok_stmt_vector &get_value(
			const std::string &key
			);

		/*
		 * Retrieve context value count
		 * @return context value count
		 */
		size_t get_value_count(void);

		/*
		 * Retrieve context value status
		 * @return true if not empty, false otherwise
		 */
		bool has_value(
			const std::string &key
			);

		/*
		 * Remove value at a given key string
		 * @param key key string reference
		 */
		void remove_value(
			const std::string &key
			);

		/*
		 * Set value at a given key string
		 * @param key key string reference
		 * @param value value object reference
		 */
		void set_value(
			const std::string &key,
			const tok_stmt &value
			);

		/*
		 * Set value at a given key string with a given index
		 * @param key key string reference
		 * @param value value object reference
		 * @param index value index
		 */
		void set_value(
			const std::string &key,
			const tok_stmt &value,
			size_t index
			);

		/*
		 * Set value at a given key string
		 * @param key key string reference
		 * @param value value object reference
		 */
		void set_value(
			const std::string &key,
			const tok_stmt_vector &value
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

		std::map<std::string, tok_stmt_vector> _values;

} cont, *cont_ptr;

typedef _node<cont> cont_node, *cont_node_ptr;
typedef _tree<cont> cont_tree, *cont_tree_ptr;

#endif