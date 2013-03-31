/*
 * par_ext.h
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

#ifndef PAR_EXC_H_
#define PAR_EXC_H_

#include "par_type.h"

/*
 * Parser extension helper routines
 */
extern void par_add_class_token(
	lex_ptr lexer,
	tok_stmt &statement,
	size_t type,
	bool root,
	bool back
	);

extern void par_add_class_token_back(
	lex_ptr lexer,
	tok_stmt &statement,
	size_t type
	);

extern void par_add_class_token_front(
	lex_ptr lexer,
	tok_stmt &statement,
	size_t type
	);

extern void par_add_token(
	lex_ptr lexer,
	tok_stmt &statement,
	bool root,
	bool back
	);

extern void par_add_token_back(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_add_token_front(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_advance_lexer(
	lex_ptr lexer
	);

extern void par_advance_lexer(
	lex_ptr lexer,
	bool exp_eos
	);

extern void par_config_lexer(
	lex_ptr lexer
	);

/*
 * Parser extension enumeration routines
 */
extern void par_enum_assignment(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_argument(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_call(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_conditional(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_conditional_statement(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_conditional_statement_list(
	lex_ptr lexer,
	tok_stmt &statement,
	size_t cond_index,
	size_t sep_index
	);

extern void par_enum_conditional_statement_list_helper(
	lex_ptr lexer,
	tok_stmt &statement,
	size_t cond_index,
	size_t sep_index
	);

extern void par_enum_control(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_expression_depth_0(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_expression_depth_0_terminal(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_expression_depth_1(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_expression_depth_1_terminal(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_expression_depth_2(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_expression_depth_2_terminal(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_expression_depth_3(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_expression_depth_3_terminal(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_expression_depth_4(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_expression_depth_4_terminal(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_expression_leaf(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_expression_list(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_expression_operator(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_expression_parameter_list(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_expression_root(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_file_stream_list(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_file_stream_list_helper(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_identifier(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_identifier_list(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_identifier_list_helper(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_identifier_operator(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_identifier_parameter_list(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_in_list(
	lex_ptr lexer,
	tok_stmt &statement,
	bool terminate
	);

extern void par_enum_io(
	lex_ptr lexer,
	tok_stmt &statement,
	bool direction_both
	);

extern void par_enum_iterator(
	lex_ptr lexer,
	tok_stmt &statement,
	bool terminate
	);

extern void par_enum_out_list(
	lex_ptr lexer,
	tok_stmt &statement,
	bool terminate
	);

extern void par_enum_range_statement(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_statement(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_statement(
	lex_ptr lexer,
	tok_stmt &statement,
	bool root
	);

extern void par_enum_statement_list(
	lex_ptr lexer,
	tok_stmt &statement,
	bool else_clause
	);

extern void par_enum_statement_list_helper(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_stream_statement(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_unary(
	lex_ptr lexer,
	tok_stmt &statement
	);

extern void par_enum_using(
	lex_ptr lexer,
	tok_stmt &statement
	);

#endif