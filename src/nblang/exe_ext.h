/*
 * exe_ext.h
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

#ifndef EXE_EXT_H_
#define EXE_EXT_H_

#include "exe_type.h"

/*
 * Executor extension helper routines
 */
extern void exe_advance_parser(
	par_ptr parser
	);

extern void exe_advance_parser(
	par_ptr parser,
	bool exp_eos
	);

extern void exe_config_parser(
	par_ptr parser
	);

extern tok_stmt_vector exe_cont_get_value(
	const std::string &key,
	cont_tree &context,
	bool force_global
	);

extern tok_stmt_vector_ptr exe_cont_get_value(
	const std::string &key,
	cont_node *context,
	bool force_global,
	bool root
	);

extern bool exe_cont_has_value(
	const std::string &key,
	cont_tree &context,
	bool force_global
	);

extern bool exe_cont_has_value(
	const std::string &key,
	cont_node *context,
	bool force_global
	);

extern void exe_cont_set_value(
	const std::string &key,
	const tok_stmt &value,
	cont_tree &context,
	size_t index,
	bool force_global
	);

extern bool exe_cont_set_value(
	const std::string &key,
	const tok_stmt &value,
	cont_node *context,
	size_t index,
	bool force_global,
	bool root
	);

extern void exe_cont_set_value(
	const std::string &key,
	const tok_stmt_vector &value,
	cont_tree &context,
	bool force_global
	);

extern bool exe_cont_set_value(
	const std::string &key,
	const tok_stmt_vector &value,
	cont_node *context,
	bool force_global,
	bool root
	);

extern tok_stmt_vector exe_create_statement_list(
	const tok &token
	);

extern tok_stmt_vector exe_create_statement_list(
	const tok_stmt &statement
	);

/*
 * Executor extension evaluation routines
 */
extern void exe_eval_assignment(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack
	);

extern void exe_eval_call(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack
	);

extern size_t exe_eval_conditional(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack
	);

extern bool exe_eval_conditional_statement(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack
	);

extern bool exe_eval_conditional_statement_list(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack,
	size_t cond_index,
	size_t sep_index
	);

extern size_t exe_eval_control(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack
	);

extern void exe_eval_expression(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack
	);

extern void exe_eval_expression_helper(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack,
	tok_stack &operator_stack
	);

extern void exe_eval_identifier_operator(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack
	);

extern void exe_eval_in_list(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack
	);

extern void exe_eval_io(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack
	);

extern void exe_eval_iterator(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack,
	bool to_stack
	);

extern void exe_eval_operation(
	tok_stack &stack,
	tok &left_operand,
	tok &right_operand,
	tok &oper
	);

extern void exe_eval_out_list(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack
	);

extern bool exe_eval_range_statement(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack
	);

extern size_t exe_eval_statement(
	const tok_vector &arguments,
	par_ptr parser,
	cont_tree &context,
	tok_stack &stack
	);

extern size_t exe_eval_statement(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack,
	size_t index,
	bool root
	);

extern size_t exe_eval_statement_list(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack
	);

extern void exe_eval_subroutine(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack,
	size_t index
	);

extern void exe_eval_using(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack,
	size_t index,
	bool root
	);

#endif