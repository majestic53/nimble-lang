/*
 * exe_ext.cpp
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

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "exe.h"
#include "exe_ext.h"
#include "exe_ext_type.h"
#include "par_ext.h"

//#define EXECUTOR_EXT_TRACING
#ifndef EXECUTOR_EXT_TRACING
#define TRACE_EVENT(_M_, _T_)
#else
#define TRACE_HEADER "executor_ext"
#include "trace.h"
#endif

/*
 * Executor extension arguments
 */
static tok_vector exe_args;

/* 
 * Executor extension parser
 */
static par_ptr exe_par;

void 
exe_advance_parser(
	par_ptr parser
	)
{
	exe_advance_parser(parser, false);
}

void 
exe_advance_parser(
	par_ptr parser,
	bool exp_eos
	)
{
	TRACE_EVENT("+exe_advance_parser", TRACE_TYPE_VERBOSE);

	if(parser) {

		if(parser->has_next()) {
			parser->move_next();
		} else if(!exp_eos) {
			TRACE_EVENT("Unexpected end of stream detected in executor!", TRACE_TYPE_ERROR);
			THROW_EXE_EXT_EXC(EXE_EXT_EXC_UNEXPECT_EOS);
		}
	} else {
		TRACE_EVENT("Invalid executor parameter!", TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC(EXE_EXT_EXC_INVAL_PARAM);
	}

	TRACE_EVENT("-exe_advance_parser", TRACE_TYPE_VERBOSE);
}

void 
exe_config_parser(
	par_ptr parser
	)
{
	TRACE_EVENT("+exe_config_parser", TRACE_TYPE_INFORMATION);

	if(parser) {
		parser->set_action(par_config_lexer, PAR_ACTION_CONFIG_LEXER);
		parser->set_enumeration_action(par_enum_statement, PAR_ENUM_ACTION_STATEMENT);
	} else {
		TRACE_EVENT("Invalid executor action parameter!", TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC(EXE_EXT_EXC_INVAL_ACTION_PARAM);
	}

	TRACE_EVENT("-exe_config_parser", TRACE_TYPE_INFORMATION);
}

tok_stmt_vector 
exe_cont_get_value(
	const std::string &key,
	cont_tree &context,
	bool force_global
	)
{
	return *exe_cont_get_value(key, context.get_position_node(), force_global, true);
}

tok_stmt_vector_ptr
exe_cont_get_value(
	const std::string &key,
	cont_node *context,
	bool force_global,
	bool root
	)
{
	TRACE_EVENT("+exe_cont_get_value", TRACE_TYPE_VERBOSE);

	tok_stmt_vector_ptr result = NULL;

	if(!context) {
		TRACE_EVENT("Invalid context node", TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC(EXE_EXT_EXC_INVAL_CONTEXT_NODE);
	}

	if(force_global
			&& context->has_parent()) {
		result = exe_cont_get_value(key, context->get_parent(), false, false);
	} else {

		if(context->get().has_value(key)) {
			result = &context->get().get_value(key);
		} else if(context->has_parent()) {
			result = exe_cont_get_value(key, context->get_parent(), false, false);
		}
	}

	if(root
			&& !result) {
		TRACE_EVENT("Key does not exist in this context: " << key, TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC_W_MESS(key, EXE_EXT_EXC_KEY_NOT_FOUND);
	}

	TRACE_EVENT("-exe_cont_get_value", TRACE_TYPE_VERBOSE);

	return result;
}

bool 
exe_cont_has_value(
	const std::string &key,
	cont_tree &context,
	bool force_global
	)
{
	return exe_cont_has_value(key, context.get_position_node(), force_global);
}

bool 
exe_cont_has_value(
	const std::string &key,
	cont_node *context,
	bool force_global
	)
{
	TRACE_EVENT("+exe_cont_has_value", TRACE_TYPE_VERBOSE);

	bool result = false;

	if(!context) {
		TRACE_EVENT("Invalid context node", TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC(EXE_EXT_EXC_INVAL_CONTEXT_NODE);
	}

	if(force_global
			&& context->has_parent()) {
		result = exe_cont_has_value(key, context->get_parent(), false);
	} else {
		result = context->get().has_value(key);

		if(!result
				&& context->has_parent()) {
			result = exe_cont_has_value(key, context->get_parent(), false);
		}
	}

	TRACE_EVENT("-exe_cont_has_value", TRACE_TYPE_VERBOSE);

	return result;
}

void 
exe_cont_set_value(
	const std::string &key,
	const tok_stmt &value,
	cont_tree &context,
	size_t index,
	bool force_global
	)
{
	exe_cont_set_value(key, value, context.get_position_node(), index, force_global, true);
}

bool 
exe_cont_set_value(
	const std::string &key,
	const tok_stmt &value,
	cont_node *context,
	size_t index,
	bool force_global,
	bool root
	)
{
	TRACE_EVENT("+exe_cont_set_value", TRACE_TYPE_VERBOSE);

	bool result = false;

	if(!context) {
		TRACE_EVENT("Invalid context node", TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC(EXE_EXT_EXC_INVAL_CONTEXT_NODE);
	}

	if(force_global
			&& context->has_parent()) {
		result = exe_cont_set_value(key, value, context->get_parent(), index, false, false);
	} else {

		if(context->get().has_value(key)) {
			context->get().set_value(key, value, index);
			result = true;
		} else if(context->has_parent()) {
			result = exe_cont_set_value(key, value, context->get_parent(), index, false, false);
		}
	}

	if(root
			&& !result) {
		context->get().set_value(key, value, index);
	}

	TRACE_EVENT("-exe_cont_set_value", TRACE_TYPE_VERBOSE);

	return result;
}

void 
exe_cont_set_value(
	const std::string &key,
	const tok_stmt_vector &value,
	cont_tree &context,
	bool force_global
	)
{
	exe_cont_set_value(key, value, context.get_position_node(), force_global, true);
}

bool 
exe_cont_set_value(
	const std::string &key,
	const tok_stmt_vector &value,
	cont_node *context,
	bool force_global,
	bool root
	)
{
	TRACE_EVENT("+exe_cont_set_value", TRACE_TYPE_VERBOSE);

	bool result = false;

	if(!context) {
		TRACE_EVENT("Invalid context node", TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC(EXE_EXT_EXC_INVAL_CONTEXT_NODE);
	}

	if(force_global
			&& context->has_parent()) {
		result = exe_cont_set_value(key, value, context->get_parent(), false, false);
	} else {

		if(context->get().has_value(key)) {
			context->get().set_value(key, value);
			result = true;
		} else if(context->has_parent()) {
			result = exe_cont_set_value(key, value, context->get_parent(), false, false);
		}
	}

	if(root
			&& !result) {
		context->get().set_value(key, value);
	}

	TRACE_EVENT("-exe_cont_set_value", TRACE_TYPE_VERBOSE);

	return result;
}

tok_stmt_vector 
exe_create_statement_list(
	const tok &token
	)
{
	return exe_create_statement_list(tok_stmt(token));
}

tok_stmt_vector 
exe_create_statement_list(
	const tok_stmt &statement
	)
{
	TRACE_EVENT("+exe_create_statement_list", TRACE_TYPE_VERBOSE);

	tok_stmt_vector result;

	result.push_back(statement);

	TRACE_EVENT("+exe_create_statement_list", TRACE_TYPE_VERBOSE);

	return result;
}

void 
exe_eval_assignment(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack
	)
{
	TRACE_EVENT("+exe_eval_assignment", TRACE_TYPE_INFORMATION);

	bool index_ref, single_assign = false;
	size_t ident_position = 0, expr_position = 0, offset = 0;

	if(!IS_TYPE_T(statement.get(), TOKEN_TYPE_ASSIGNMENT)) {
		TRACE_EVENT("Invalid assignment token: " << statement.get()to_string(true), TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC_W_MESS(statement.get().to_string(true), EXE_EXT_EXC_INVAL_ASSIGNMENT_TOKEN);
	}
	index_ref = (statement.get_child_count() == ASSIGNMENT_MAX_CHILD_COUNT);
	statement.move_child_front();
	ident_position = statement.get_child_count();
	statement.move_parent();

	if(index_ref) {
		statement.move_child(ASSIGNMENT_INDEX_CHILD);
		statement.move_child_front();
		exe_eval_expression(statement, context, stack);
		offset = stack.top().front().get().to_integer();
		stack.pop();
		statement.move_parent();
		statement.move_parent();
	}
	statement.move_child(index_ref ? (ASSIGNMENT_TYPE_CHILD + 1) : ASSIGNMENT_TYPE_CHILD);

	switch(statement.get().get_class_type()) {
		case CLASS_TOKEN_TYPE_COMPOUND_ASSIGNMENT:
			statement.move_child_front();

			if(statement.get_child_count() != ident_position) {
				TRACE_EVENT("Invalid assignment count: " << ident_position << " of " << statement.get_child_count(), TRACE_TYPE_ERROR);
				THROW_EXE_EXT_EXC_W_MESS(ident_position << " of " << statement.get_child_count(), EXE_EXT_EXC_INVAL_ASSIGNMENT_COUNT);
			}
			
			for(; expr_position < statement.get_child_count(); ++expr_position) {
				statement.move_child(expr_position);
				exe_eval_expression(statement, context, stack);
				statement.move_parent();
			}
			statement.move_parent();
			break;
		case CLASS_TOKEN_TYPE_FUNCTION_ASSIGNMENT:
			single_assign = true;
			stack.push(exe_create_statement_list(tok_stmt(statement, true)));
			break;
		case CLASS_TOKEN_TYPE_SIMPLE_ASSIGNMENT:
			statement.move_child_front();
			exe_eval_expression(statement, context, stack);
			single_assign = (stack.size() <= MIN_CHILD_COUNT);
			statement.move_parent();
			break;
	}
	statement.move_parent();
	statement.move_child_front();
	--ident_position;

	for(; (long) ident_position >= 0; --ident_position) {
		statement.move_child(ident_position);

		if(index_ref) {
			exe_cont_set_value(statement.get().get_text(), stack.top().front(), context, offset, false);
		} else {
			exe_cont_set_value(statement.get().get_text(), stack.top(), context, false);
		}

		if(!single_assign
				|| !ident_position) {
			stack.pop();
		}
		statement.move_parent();
	}
	statement.move_parent();

	TRACE_EVENT("-exe_eval_assignment", TRACE_TYPE_INFORMATION);
}

extern void exe_eval_call(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack
	)
{
	TRACE_EVENT("+exe_eval_call", TRACE_TYPE_INFORMATION);
		
	if(!IS_TYPE_T(statement.get(), TOKEN_TYPE_CALL)) {
		TRACE_EVENT("Invalid call token: " << statement.get().to_string(true), TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC_W_MESS(statement.get().to_string(true), EXE_EXT_EXC_INVAL_CALL_TOKEN);
	}

	statement.move_child_front();
	exe_eval_expression(statement, context, stack);

	while(!stack.empty()) {
		stack.pop();
	}
	statement.move_parent();

	TRACE_EVENT("-exe_eval_call", TRACE_TYPE_INFORMATION);
}

size_t 
exe_eval_conditional(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack
	)
{
	TRACE_EVENT("+exe_eval_conditional", TRACE_TYPE_INFORMATION);

	size_t signal = EXE_SIG_NONE;
		
	if(!IS_TYPE_T(statement.get(), TOKEN_TYPE_CONDITIONAL)) {
		TRACE_EVENT("Invalid conditional token: " << statement.get().to_string(true), TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC_W_MESS(statement.get().to_string(true), EXE_EXT_EXC_INVAL_CONDITIONAL_TOKEN);
	}

	switch(statement.get().get_subtype()) {
		case CONDITIONAL_TYPE_IF:
			statement.move_child(IF_COND_STMT_CHILD);

			if(exe_eval_conditional_statement_list(statement, context, stack, IF_COND_STMT_CHILD, COND_STMT_SEPERATOR_CHILD)) {
				statement.move_parent();
				statement.move_child(IF_COND_STMT_LIST_CHILD);
				context.add_child_back(cont());
				context.move_child_back();
				signal = exe_eval_statement_list(statement, context, stack);
				context.move_parent();
				context.remove_children();
			}
			statement.move_parent();
			break;
		case CONDITIONAL_TYPE_IFELSE:
			statement.move_child(IFELSE_COND_STMT_CHILD);

			if(exe_eval_conditional_statement_list(statement, context, stack, IFELSE_COND_STMT_CHILD, COND_STMT_SEPERATOR_CHILD)) {
				statement.move_parent();
				statement.move_child(IFELSE_COND_STMT0_LIST_CHILD);
				context.add_child_back(cont());
				context.move_child_back();
				signal = exe_eval_statement_list(statement, context, stack);
				context.move_parent();
				context.remove_children();
			} else {
				statement.move_parent();
				statement.move_child(IFELSE_COND_STMT1_LIST_CHILD);
				context.add_child_back(cont());
				context.move_child_back();
				signal = exe_eval_statement_list(statement, context, stack);
				context.move_parent();
				context.remove_children();
			}
			statement.move_parent();
			break;
		case CONDITIONAL_TYPE_RANGE:
			statement.move_child(RANGE_COND_STMT_CHILD);

			while(exe_eval_range_statement(statement, context, stack)) {
				statement.move_parent();
				statement.move_child(RANGE_COND_STMT_LIST_CHILD);
				context.add_child_back(cont());
				context.move_child_back();
				signal = exe_eval_statement_list(statement, context, stack);
				context.move_parent();
				context.remove_children();
				
				if(signal == EXE_SIG_BREAK
						|| signal == EXE_SIG_EXIT) {
					break;
				} else if(signal == EXE_SIG_CONTINUE) {
					continue;
				}
				statement.move_parent();
				statement.move_child(RANGE_COND_STMT_CHILD);
				statement.move_child(RANGE_STMT_COND_EXPR_LIST_CHILD);
				signal = exe_eval_statement_list(statement, context, stack);

				if(signal == EXE_SIG_BREAK
						|| signal == EXE_SIG_EXIT) {
					break;
				} else if(signal == EXE_SIG_CONTINUE) {
					continue;
				}
				statement.move_parent();
				statement.move_parent();
				statement.move_child(RANGE_COND_STMT_CHILD);
			}
			signal = EXE_SIG_NONE;
			statement.move_parent();
			break;
		case CONDITIONAL_TYPE_WHILE:
			statement.move_child(WHILE_COND_STMT_CHILD);

			while(exe_eval_conditional_statement_list(statement, context, stack, WHILE_COND_STMT_CHILD, COND_STMT_SEPERATOR_CHILD)) {
				statement.move_parent();
				statement.move_child(WHILE_COND_STMT_LIST_CHILD);
				context.add_child_back(cont());
				context.move_child_back();
				signal = exe_eval_statement_list(statement, context, stack);
				context.move_parent();
				context.remove_children();

				if(signal == EXE_SIG_BREAK
						|| signal == EXE_SIG_EXIT) {
					break;
				} else if(signal == EXE_SIG_CONTINUE) {
					statement.move_parent();
					statement.move_child(WHILE_COND_STMT_CHILD);
					continue;
				}
				statement.move_parent();
				statement.move_child(WHILE_COND_STMT_CHILD);
			}
			signal = EXE_SIG_NONE;
			statement.move_parent();
			break;
		case CONDITIONAL_TYPE_UNTIL:
			statement.move_child(UNTIL_COND_STMT_CHILD);

			do {
				statement.move_parent();
				statement.move_child(UNTIL_COND_STMT_LIST_CHILD);
				context.add_child_back(cont());
				context.move_child_back();
				signal = exe_eval_statement_list(statement, context, stack);
				context.move_parent();
				context.remove_children();

				if(signal == EXE_SIG_BREAK
						|| signal == EXE_SIG_EXIT) {
					break;
				} else if(signal == EXE_SIG_CONTINUE) {
					statement.move_parent();
					statement.move_child(UNTIL_COND_STMT_CHILD);
					continue;
				}
				statement.move_parent();
				statement.move_child(UNTIL_COND_STMT_CHILD);
			} while(exe_eval_conditional_statement_list(statement, context, stack, UNTIL_COND_STMT_CHILD, UNTIL_STMT_SEPERATOR_CHILD));
			signal = EXE_SIG_NONE;
			statement.move_parent();
			break;
	}

	TRACE_EVENT("-exe_eval_conditional", TRACE_TYPE_INFORMATION);

	return signal;
}

bool 
exe_eval_conditional_statement(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack
	)
{
	TRACE_EVENT("+exe_eval_conditional_statement", TRACE_TYPE_INFORMATION);

	bool result = false;
	size_t comparator_type;
	tok_stmt left_operand, right_operand;
	
	if(!IS_CLASS_TYPE_T(statement.get(), CLASS_TOKEN_TYPE_CONDITIONAL_STATEMENT)) {
		TRACE_EVENT("Invalid conditional statement token: " << statement.get().to_string(true), TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC_W_MESS(statement.get().to_string(true), EXE_EXT_EXC_INVAL_CONDITIONAL_STMT_TOKEN);
	}
	statement.move_child(COND_STMT_COMPARATOR_CHILD);

	if(!IS_TYPE_T(statement.get(), TOKEN_TYPE_COMPARATOR)) {
		TRACE_EVENT("Invalid comparator token: " << statement.get().to_string(true), TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC_W_MESS(statement.get().to_string(true), EXE_EXT_EXC_INVAL_COMPARATOR_TOKEN);
	}
	comparator_type = statement.get().get_subtype();
	statement.move_parent();
	statement.move_child(COND_STMT_EXPRESSION_CHILD_0);
	exe_eval_expression(statement, context, stack);
	statement.move_parent();
	statement.move_child(COND_STMT_EXPRESSION_CHILD_1);
	exe_eval_expression(statement, context, stack);
	statement.move_parent();
	right_operand = stack.top().front();
	stack.pop();
	left_operand = stack.top().front();
	stack.pop();

	switch(comparator_type) {
		case COMPARATOR_TYPE_AND:
			result = (left_operand.get().to_float() && right_operand.get().to_float());
			break;
		case COMPARATOR_TYPE_EQUAL:

			if(IS_TYPE_T(left_operand.get(), TOKEN_TYPE_VAR_STRING)
					|| IS_TYPE_T(right_operand.get(), TOKEN_TYPE_VAR_STRING)) {
				result = (left_operand.get().get_text() == right_operand.get().get_text());
			} else {
				result = (left_operand.get().to_float() == right_operand.get().to_float());
			}
			break;
		case COMPARATOR_TYPE_GREATER:
			result = (left_operand.get().to_float() > right_operand.get().to_float());
			break;
		case COMPARATOR_TYPE_GREATER_EQUAL:
			result = (left_operand.get().to_float() >= right_operand.get().to_float());
			break;
		case COMPARATOR_TYPE_LESSER:
			result = (left_operand.get().to_float() < right_operand.get().to_float());
			break;
		case COMPARATOR_TYPE_LESSER_EQUAL:
			result = (left_operand.get().to_float() <= right_operand.get().to_float());
			break;
		case COMPARATOR_TYPE_NOT_EQUAL:

			if(IS_TYPE_T(left_operand.get(), TOKEN_TYPE_VAR_STRING)
					|| IS_TYPE_T(right_operand.get(), TOKEN_TYPE_VAR_STRING)) {
				result = (left_operand.get().get_text() != right_operand.get().get_text());
			} else {
				result = (left_operand.get().to_float() != right_operand.get().to_float());
			}
			break;
		case COMPARATOR_TYPE_OR:
			result = (left_operand.get().to_float() || right_operand.get().to_float());
			break;
	}

	TRACE_EVENT("-exe_eval_conditional_statement", TRACE_TYPE_INFORMATION);

	return result;
}

bool 
exe_eval_conditional_statement_list(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack,
	size_t cond_index,
	size_t sep_index
	)
{
	TRACE_EVENT("+exe_eval_conditional_statement_list", TRACE_TYPE_INFORMATION);

	bool result = false;
	size_t i = 0, type = CONDITIONAL_SEPERATOR_AND;

	UNREF_PARAM(context);
	UNREF_PARAM(stack);

	if(!IS_CLASS_TYPE_T(statement.get(), CLASS_TOKEN_TYPE_CONDITIONAL_STATEMENT_LIST)) {
		TRACE_EVENT("Invalid conditional statement list token: " << statement.get().to_string(true), TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC_W_MESS(statement.get().to_string(true), EXE_EXT_EXC_INVAL_CONDITIONAL_STMT_LIST_TOKEN);
	}

	for(; i < statement.get_child_count(); ++i) {

		if(i) {
			statement.move_parent();
			statement.move_child(sep_index);
			statement.move_child(i - 1);
			type = statement.get().get_subtype();
			statement.move_parent();
			statement.move_parent();
			statement.move_child(cond_index);
		}
		statement.move_child(i);
		result = !i ? exe_eval_conditional_statement(statement, context, stack) : 
			(type == CONDITIONAL_SEPERATOR_AND ? (result && exe_eval_conditional_statement(statement, context, stack)) :
			(result || exe_eval_conditional_statement(statement, context, stack)));
		statement.move_parent();
	}

	TRACE_EVENT("-exe_eval_conditional_statement_list", TRACE_TYPE_INFORMATION);

	return result;
}

size_t 
exe_eval_control(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack
	)
{
	TRACE_EVENT("+exe_eval_control", TRACE_TYPE_INFORMATION);

	size_t signal = EXE_SIG_NONE;
	
	UNREF_PARAM(context);
	UNREF_PARAM(stack);

	if(!IS_TYPE_T(statement.get(), TOKEN_TYPE_CONTROL)) {
		TRACE_EVENT("Invalid control token: " << statement.get().to_string(true), TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC_W_MESS(statement.get().to_string(true), EXE_EXT_EXC_INVAL_CONTROL_TOKEN);
	}

	switch(statement.get().get_subtype()) {
		case CONTROL_TYPE_BREAK:
			signal = EXE_SIG_BREAK;
			break;
		case CONTROL_TYPE_CONTINUE:
			signal = EXE_SIG_CONTINUE;
			break;
		case CONTROL_TYPE_EXIT:
			signal = EXE_SIG_EXIT;
			//exit(EXE_SIG_EXIT);
			break;
	}

	TRACE_EVENT("-exe_eval_control", TRACE_TYPE_INFORMATION);

	return signal;
}

void 
exe_eval_expression(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack
	)
{
	TRACE_EVENT("+exe_eval_expression", TRACE_TYPE_INFORMATION);
		
	tok_stack operator_stack;

	if(!IS_CLASS_TYPE_T(statement.get(), CLASS_TOKEN_TYPE_EXPRESSION)) {
		TRACE_EVENT("Invalid expression class token: " << statement.get().to_string(true), TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC_W_MESS(statement.get().to_string(true), EXE_EXT_EXC_INVAL_EXPRESSION_CLASS_TOKEN);
	}
	exe_eval_expression_helper(statement, context, stack, operator_stack);

	TRACE_EVENT("-exe_eval_expression", TRACE_TYPE_INFORMATION);
}

extern void exe_eval_expression_helper(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack,
	tok_stack &operator_stack
	)
{
	TRACE_EVENT("+exe_eval_expression_helper", TRACE_TYPE_VERBOSE);

	std::stringstream ss;
	size_t i = 0, offset = 0;
	tok_stmt_vector value_list;
	tok value_tok, left_operand, right_operand;

	if(statement.has_children()) {

		if(IS_CLASS_TYPE_T(statement.get(), CLASS_TOKEN_TYPE_EXPRESSION)) {
				
			if(statement.get_child_count() == MIN_CHILD_COUNT) {
				statement.move_child_front();
				exe_eval_expression_helper(statement, context, stack, operator_stack);
				statement.move_parent();
			} else {

				for(; i < statement.get_child_count(); ++i) {
					statement.move_child(i);
					exe_eval_expression_helper(statement, context, stack, operator_stack);
					statement.move_parent();
				}
			}
		} else {

			switch(statement.get().get_type()) {
				case TOKEN_TYPE_ACCESS:
				case TOKEN_TYPE_ITERATOR_OPERATOR:
				case TOKEN_TYPE_OUT_LIST_OPERATOR:
					exe_eval_identifier_operator(statement, context, stack);
					break;
				case TOKEN_TYPE_ARGUMENT:
					statement.move_child_front();
					exe_eval_expression_helper(statement, context, stack, operator_stack);
					statement.move_parent();
					offset = stack.top().front().get().to_integer();
					stack.pop();
					if(offset >= exe_args.size()) {
						TRACE_EVENT("Attempting to reference non-existent argument: " << offset, TRACE_TYPE_ERROR);
						THROW_EXE_EXT_EXC_W_MESS(offset, EXE_EXT_EXC_REF_TO_NON_EXISTENT_ARG);
					}
					stack.push(exe_create_statement_list(exe_args.at(offset)));
					break;
				case TOKEN_TYPE_CONVERSION_OPERATOR:
					statement.move_child_front();
					exe_eval_expression_helper(statement, context, stack, operator_stack);
					statement.move_parent();

					switch(statement.get().get_subtype()) {
						case CONVERSION_OPERATOR_TYPE_FLOAT:
							ss << stack.top().front().get().get_text();
							EXE_SET_TOKEN_TYPE(value_tok, TOKEN_TYPE_FLOAT, ss.str());
							break;
						case CONVERSION_OPERATOR_TYPE_INT:
							ss << stack.top().front().get().get_text();
							EXE_SET_TOKEN_TYPE(value_tok, TOKEN_TYPE_INTEGER, ss.str());
							break;
					}
					stack.pop();
					stack.push(exe_create_statement_list(value_tok));
					break;
				case TOKEN_TYPE_BINARY_OPERATOR:
					statement.move_child_front();
					exe_eval_expression_helper(statement, context, stack, operator_stack);
					statement.move_parent();
					right_operand = stack.top().front().get();
					stack.pop();
					left_operand = stack.top().front().get();
					stack.pop();
					exe_eval_operation(stack, left_operand, right_operand, statement.get());
					break;
				case TOKEN_TYPE_IDENTIFIER:

					switch(statement.get_child(IDENTIFIER_TYPE_CHILD).get_class_type()) {
						case CLASS_TOKEN_TYPE_FUNCTION_PARAMETER_LIST:

							if(statement.get_child_count() == FUNCT_LIST_INDEX_CHILD_COUNT) {
								statement.move_child(FUNCT_LIST_INDEX_CHILD);
								statement.move_child_front();
								statement.move_child_front();
								exe_eval_expression_helper(statement, context, stack, operator_stack);
								statement.move_parent();
								statement.move_parent();
								statement.move_parent();
								offset = stack.top().front().get().to_integer();
								stack.pop();
							}
							exe_eval_subroutine(statement, context, stack, offset);
							break;
						case CLASS_TOKEN_TYPE_LIST_INDEX:
							statement.move_child_front();
							statement.move_child_front();
							exe_eval_expression_helper(statement, context, stack, operator_stack);
							statement.move_parent();
							statement.move_parent();
							offset = stack.top().front().get().to_integer();
							stack.pop();
							value_list = exe_cont_get_value(statement.get().get_text(), context, false);

							if(offset >= value_list.size()) {
								TRACE_EVENT("Attempting to reference non-existent index: " << offset, TRACE_TYPE_ERROR);
								THROW_EXE_EXT_EXC_W_MESS(statement.get().get_text() << "[" << offset << "]", EXE_EXT_EXC_REF_TO_NON_EXISTENT_INDEX);
							}
							exe_eval_expression_helper(value_list.at(offset), context, stack, operator_stack);
							break;
					}
					break;
				case TOKEN_TYPE_IO_OPERATOR:
					exe_eval_io(statement, context, stack);
					break;
				case TOKEN_TYPE_LOGICAL_OPERATOR:
					statement.move_child_front();
					exe_eval_expression_helper(statement, context, stack, operator_stack);
					statement.move_parent();

					switch(statement.get().get_subtype()) {
						case LOGICAL_OPERATOR_TYPE_CEILING:
							stack.top().front().get().ceiling();
							break;
						case LOGICAL_OPERATOR_TYPE_FLOOR:
							stack.top().front().get().floor();
							break;
						case LOGICAL_OPERATOR_TYPE_RANDOM:
							stack.top().front().get().randomize(stack.top().front().get().to_integer());
							break;
						case LOGICAL_OPERATOR_TYPE_ROUND:
							stack.top().front().get().round();
							break;
						case LOGICAL_OPERATOR_TYPE_SHIFT_LEFT:
							stack.top().front().get().shift_left(1);
							break;
						case LOGICAL_OPERATOR_TYPE_SHIFT_RIGHT:
							stack.top().front().get().shift_right(1);
							break;
					}
					break;
				case TOKEN_TYPE_SYMBOL:

					switch(statement.get().get_subtype()) {
						case SYMBOL_TYPE_ARITHMETIC_ADD:
						case SYMBOL_TYPE_ARITHMETIC_DIVIDE:
						case SYMBOL_TYPE_ARITHMETIC_MODULUS:
						case SYMBOL_TYPE_ARITHMETIC_MULTIPLY:
						case SYMBOL_TYPE_ARITHMETIC_SUBTRACT:
							statement.move_child_front();
							exe_eval_expression_helper(statement, context, stack, operator_stack);
							statement.move_parent();
							right_operand = stack.top().front().get();
							stack.pop();
							left_operand = stack.top().front().get();
							stack.pop();
							exe_eval_operation(stack, left_operand, right_operand, statement.get());
							break;
						case SYMBOL_TYPE_UNARY_NEGATION:
							statement.move_child_front();
							exe_eval_expression_helper(statement, context, stack, operator_stack);
							statement.move_parent();
							stack.top().front().get().negate();
							break;
					}
					break;
			}
		}
	} else {

		switch(statement.get().get_type()) {
			case TOKEN_TYPE_ARGUMENT:
				ss << exe_args.size();
				EXE_SET_TOKEN_TYPE(value_tok, TOKEN_TYPE_INTEGER, ss.str());
				stack.push(exe_create_statement_list(value_tok));
				break;
			case TOKEN_TYPE_CONSTANT:
				
				switch(statement.get().get_subtype()) {
					case CONSTANT_TYPE_EMPTY:
						EXE_SET_TOKEN_TYPE(value_tok, TOKEN_TYPE_VAR_STRING, std::string());
					case CONSTANT_TYPE_ENDLINE:
						ss << WS_NEWLINE;
						EXE_SET_TOKEN_TYPE(value_tok, TOKEN_TYPE_VAR_STRING, ss.str());
						break;
					case CONSTANT_TYPE_FALSE:
					case CONSTANT_TYPE_NULL:
					case CONSTANT_TYPE_TRUE:
						ss << (statement.get().get_subtype() == CONSTANT_TYPE_TRUE ? 1 : 0);
						EXE_SET_TOKEN_TYPE(value_tok, TOKEN_TYPE_INTEGER, ss.str());
						break;
				}
				stack.push(exe_create_statement_list(value_tok));

				if(IS_TYPE_T(stack.top().front().get(), TOKEN_TYPE_VAR_STRING)
						&& IS_SUBTYPE_T(statement.get(), CONSTANT_TYPE_EMPTY)) {
					stack.top().front().get().set_value(std::string());
				}
				break;
			case TOKEN_TYPE_FLOAT:
			case TOKEN_TYPE_INTEGER:
			case TOKEN_TYPE_VAR_STRING:
				stack.push(exe_create_statement_list(statement.get()));
				break;
			case TOKEN_TYPE_IDENTIFIER:
				stack.push(exe_cont_get_value(statement.get().get_text(), context, false));
				break;
		}
	}

	TRACE_EVENT("-exe_eval_expression_helper", TRACE_TYPE_VERBOSE);
}

void 
exe_eval_identifier_operator(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack
	)
{
	TRACE_EVENT("+exe_eval_identifier_operator", TRACE_TYPE_INFORMATION);

	if(!IS_TYPE_T(statement.get(), TOKEN_TYPE_ACCESS)
			&& !IS_TYPE_T(statement.get(), TOKEN_TYPE_ITERATOR_OPERATOR)
			&& !IS_TYPE_T(statement.get(), TOKEN_TYPE_OUT_LIST_OPERATOR)) {
		TRACE_EVENT("Invalid identifier operator token: " << statement.get().to_string(true), TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC_W_MESS(statement.get().to_string(true), EXE_EXT_EXC_INVAL_IDENT_OPERATOR_TOKEN);
	}

	switch(statement.get().get_type()) {
		case TOKEN_TYPE_ACCESS:
			statement.move_child_front();
			stack.push(exe_cont_get_value(statement.get().get_text(), context, true));
			statement.move_parent();
			break;
		case TOKEN_TYPE_ITERATOR_OPERATOR:
			exe_eval_iterator(statement, context, stack, true);
			break;
		case TOKEN_TYPE_OUT_LIST_OPERATOR:
			exe_eval_out_list(statement, context, stack);
			break;
	}

	TRACE_EVENT("-exe_eval_identifier_operator", TRACE_TYPE_INFORMATION);
}

void 
exe_eval_in_list(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack
	)
{
	TRACE_EVENT("+exe_eval_in_list", TRACE_TYPE_INFORMATION);

	tok value_tok;
	std::stringstream ss;
	size_t i = 0, subtype;
	tok_stmt_vector value_stmt_vec;

	if(!IS_TYPE_T(statement.get(), TOKEN_TYPE_IN_LIST_OPERATOR)) {
		TRACE_EVENT("Invalid in-list operator token: " << statement.get().to_string(true), TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC_W_MESS(statement.get().to_string(true), EXE_EXT_EXC_INVAL_IN_LIST_TOKEN);
	}
	subtype = statement.get().get_subtype();
	statement.move_child(IN_LIST_EXPRESSION_CHILD);
	exe_eval_expression(statement, context, stack);
	statement.move_parent();
	statement.move_child_front();

	for(; i < statement.get_child_count(); ++i) {
		statement.move_child(i);
		value_stmt_vec = exe_cont_get_value(statement.get().get_text(), context, false);

		switch(subtype) {
			case IN_LIST_OPERATOR_TYPE_PUSH_END:
				value_stmt_vec.push_back(stack.top().front());
				exe_cont_set_value(statement.get().get_text(), value_stmt_vec, context, false);
				break;
			case IN_LIST_OPERATOR_TYPE_PUSH_FRONT:
				value_stmt_vec.insert(value_stmt_vec.begin(), stack.top().front());
				exe_cont_set_value(statement.get().get_text(), value_stmt_vec, context, false);
				break;
		}
		stack.pop();
		statement.move_parent();
	}
	statement.move_parent();

	TRACE_EVENT("-exe_eval_in_list", TRACE_TYPE_INFORMATION);
}

void 
exe_eval_io(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack
	)
{
	TRACE_EVENT("+exe_eval_io", TRACE_TYPE_INFORMATION);

	tok input_tok;
	std::stringstream ss;
	size_t child_position = 0;
	std::string input, filename;
	bool is_append = false, is_binary = false, is_trunc = false;

	if(!IS_TYPE_T(statement.get(), TOKEN_TYPE_IO_OPERATOR)) {
		TRACE_EVENT("Invalid io token: " << statement.get().to_string(true), TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC_W_MESS(statement.get().to_string(true), EXE_EXT_EXC_INVAL_IO_TOKEN);
	}

	switch(statement.get().get_subtype()) {
		case IO_OPERATOR_TYPE_EXECUTE:
			statement.move_child_front();
			exe_eval_expression(statement, context, stack);
			system(stack.top().front().get().get_text().c_str());		
			stack.pop();
			statement.move_parent();
			break;
		case IO_OPERATOR_TYPE_IN:
		case IO_OPERATOR_TYPE_OUT:
			statement.move_child_front();
			statement.move_child_front();

			switch(statement.get().get_subtype()) {
				case STREAM_TYPE_FILE:
				
					if(statement.get_child_count() == MIN_CHILD_COUNT) {
						statement.move_child_front();
						exe_eval_expression(statement, context, stack);
						filename = stack.top().front().get().get_text();
						stack.pop();
					} else {
						statement.move_child_front();

						for(; child_position < statement.get_child_count(); ++child_position) {
							statement.move_child(child_position);

							switch(statement.get().get_subtype()) {
								case FILE_STREAM_OPERATOR_TYPE_APPEND:
									is_append = true;
									break;
								case FILE_STREAM_OPERATOR_TYPE_BIN:
									is_binary = true;
									break;
								case FILE_STREAM_OPERATOR_TYPE_TRUNC:
									is_trunc = true;
									break;
							}
							statement.move_parent();
						}
						statement.move_parent();
						statement.move_child(FILE_STREAM_EXPRESSION_CHILD);
						exe_eval_expression(statement, context, stack);
						filename = stack.top().front().get().get_text();
						stack.pop();
					}
					statement.move_parent();
					statement.move_parent();
					statement.move_parent();

					if(IS_SUBTYPE_T(statement.get(), IO_OPERATOR_TYPE_IN)) {
						std::ifstream file(filename.c_str(), std::ios::in | (is_binary ? std::ios::binary : 0));

						if(file) {
							ss << file.rdbuf();
							input = ss.str();
							file.close();
							EXE_SET_TOKEN_TYPE(input_tok, TOKEN_TYPE_VAR_STRING, input);
							stack.push(exe_create_statement_list(input_tok));
						}
					} else {
						std::ofstream file(filename.c_str(), std::ios::out
								| (is_append ? std::ios::app : 0)
								| (is_binary ? std::ios::binary : 0)
								| (is_trunc ? std::ios::trunc : 0));

						if(file) {
							statement.move_child(IO_OUT_EXPRESSION_CHILD);
							exe_eval_expression(statement, context, stack);
							file << stack.top().front().get().get_text();
							file.close();
							stack.pop();
							statement.move_parent();
						}
					}
					break;
				case STREAM_TYPE_STDIN:
					statement.move_parent();
					statement.move_parent();

					if(!IS_SUBTYPE_T(statement.get(), IO_OPERATOR_TYPE_IN)) {
						TRACE_EVENT("Invalid stream token: " << statement.get().to_string(true), TRACE_TYPE_ERROR);
						THROW_EXE_EXT_EXC_W_MESS(statement.get().to_string(true), EXE_EXT_EXC_INVAL_STREAM_TOKEN);
					}
					std::getline(std::cin, input);
					EXE_SET_TOKEN_TYPE(input_tok, TOKEN_TYPE_VAR_STRING, input);
					stack.push(exe_create_statement_list(input_tok));
					break;
				case STREAM_TYPE_STDOUT:
					statement.move_parent();
					statement.move_parent();

					if(!IS_SUBTYPE_T(statement.get(), IO_OPERATOR_TYPE_OUT)) {
						TRACE_EVENT("Invalid stream token: " << statement.get().to_string(true), TRACE_TYPE_ERROR);
						THROW_EXE_EXT_EXC_W_MESS(statement.get().to_string(true), EXE_EXT_EXC_INVAL_STREAM_TOKEN);
					}
					
					statement.move_child(IO_OUT_EXPRESSION_CHILD);
					exe_eval_expression(statement, context, stack);
					std::cout << stack.top().front().get().get_text();
					stack.pop();
					statement.move_parent();
					break;
			}
			break;
	}

	TRACE_EVENT("-exe_eval_io", TRACE_TYPE_INFORMATION);
}

void 
exe_eval_iterator(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack,
	bool to_stack
	)
{
	TRACE_EVENT("+exe_eval_iterator", TRACE_TYPE_INFORMATION);

	bool index_ref;
	long i_val = 0;
	size_t offset = 0;
	double f_val = 0.0;
	std::stringstream ss;
	size_t i = 0, subtype, type;
	tok_stmt_vector value_stmt_vec;

	if(!IS_TYPE_T(statement.get(), TOKEN_TYPE_ITERATOR_OPERATOR)) {
		TRACE_EVENT("Invalid iterator token: " << statement.get().to_string(true), TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC_W_MESS(statement.get().to_string(true), EXE_EXT_EXC_INVAL_ITERATOR_TOKEN);
	}
	index_ref = (statement.get_child_count() == ITER_MAX_CHILD_COUNT);

	if(index_ref) {
		statement.move_child(ITER_LIST_INDEX_CHILD);
		statement.move_child_front();
		exe_eval_expression(statement, context, stack);
		offset = stack.top().front().get().to_integer();
		stack.pop();
		statement.move_parent();
		statement.move_parent();
	}
	subtype = statement.get().get_subtype();
	statement.move_child_front();

	if(offset >= statement.get_child_count()) {
		TRACE_EVENT("Invalid index: " << statement.get().to_string(true) << "[" << offset << "]", TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC_W_MESS(statement.get().to_string(true) << "[" << offset << "]", EXE_EXT_EXC_INVAL_TOKEN_INDEX);		
	}

	for(; i < statement.get_child_count(); ++i) {
		statement.move_child(i);
		value_stmt_vec = exe_cont_get_value(statement.get().get_text(), context, false);

		if(!IS_CLASS_TYPE_T(value_stmt_vec.at(offset).get(), CLASS_TOKEN_TYPE_TOKEN)) {
			TRACE_EVENT("Invalid iteratable class token: " << statement.get().to_string(true), TRACE_TYPE_ERROR);
			THROW_EXE_EXT_EXC_W_MESS(statement.get().to_string(true), EXE_EXT_EXC_INVAL_ITERATABLE_CLASS_TOKEN);
		}
		type = value_stmt_vec.at(offset).get().get_type();

		switch(type) {
			case TOKEN_TYPE_FLOAT:
				f_val = value_stmt_vec.at(offset).get().to_float();
				break;
			case TOKEN_TYPE_INTEGER:
				i_val = value_stmt_vec.at(offset).get().to_integer();
				break;
			default:
				TRACE_EVENT("Invalid iteratable token: " << statement.get().to_string(true), TRACE_TYPE_ERROR);
				THROW_EXE_EXT_EXC_W_MESS(statement.get().to_string(true), EXE_EXT_EXC_INVAL_ITERATABLE_TOKEN);
		}

		switch(subtype) {
			case ITERATOR_OPERATOR_TYPE_POST_DECREMENT:
			case ITERATOR_OPERATOR_TYPE_PRE_DECREMENT:

				if(to_stack
						&& subtype == ITERATOR_OPERATOR_TYPE_POST_DECREMENT) {
					ss << (type == TOKEN_TYPE_FLOAT ? f_val : i_val);
					value_stmt_vec.at(offset).get().set_value(ss.str());
					stack.push(value_stmt_vec);
				}
				(type == TOKEN_TYPE_FLOAT ? --f_val : --i_val);
						
				if(!to_stack
						|| subtype == ITERATOR_OPERATOR_TYPE_PRE_DECREMENT) {
					ss << (type == TOKEN_TYPE_FLOAT ? f_val : i_val);
					value_stmt_vec.at(offset).get().set_value(ss.str());
					stack.push(value_stmt_vec);
				}
				break;
			case ITERATOR_OPERATOR_TYPE_POST_INCREMENT:
			case ITERATOR_OPERATOR_TYPE_PRE_INCREMENT:

				if(to_stack
						&& subtype == ITERATOR_OPERATOR_TYPE_POST_INCREMENT) {
					ss << (type == TOKEN_TYPE_FLOAT ? f_val : i_val);
					value_stmt_vec.at(offset).get().set_value(ss.str());
					stack.push(value_stmt_vec);
				}
				(type == TOKEN_TYPE_FLOAT ? ++f_val : ++i_val);
						
				if(!to_stack
						|| subtype == ITERATOR_OPERATOR_TYPE_PRE_INCREMENT) {
					ss << (type == TOKEN_TYPE_FLOAT ? f_val : i_val);
					value_stmt_vec.at(offset).get().set_value(ss.str());
					stack.push(value_stmt_vec);
				}
				break;
		}
		ss.clear();
		ss.str(std::string());
		ss << (type == TOKEN_TYPE_FLOAT ? f_val : i_val);
		value_stmt_vec.at(offset).get().set_value(ss.str());

		if(index_ref) {
			exe_cont_set_value(statement.get().get_text(), value_stmt_vec.at(offset), context, offset, false);
		} else {
			exe_cont_set_value(statement.get().get_text(), value_stmt_vec, context, false);
		}
		value_stmt_vec.clear();
		statement.move_parent();
	}
	statement.move_parent();

	TRACE_EVENT("-exe_eval_iterator", TRACE_TYPE_INFORMATION);
}

void 
exe_eval_operation(
	tok_stack &stack,
	tok &left_operand,
	tok &right_operand,
	tok &oper
	)
{
	TRACE_EVENT("+exe_eval_operation", TRACE_TYPE_INFORMATION);

	tok value_tok;
	long i_val = 0;
	double f_val = 0.0;
	std::string s_val;
	std::stringstream ss;
	size_t type = TOKEN_TYPE_VAR_STRING;

	if(IS_TYPE_T(left_operand, TOKEN_TYPE_VAR_STRING)
			|| IS_TYPE_T(right_operand, TOKEN_TYPE_VAR_STRING)
			|| IS_TOKEN_TYPE_T(left_operand, TOKEN_TYPE_CONSTANT, CONSTANT_TYPE_ENDLINE)
			|| IS_TOKEN_TYPE_T(right_operand, TOKEN_TYPE_CONSTANT, CONSTANT_TYPE_ENDLINE)) {
		type = TOKEN_TYPE_VAR_STRING;
	} else if(IS_TYPE_T(left_operand, TOKEN_TYPE_FLOAT)
			|| IS_TYPE_T(right_operand, TOKEN_TYPE_FLOAT)) {
		type = TOKEN_TYPE_FLOAT;
	} else if(IS_TYPE_T(left_operand, TOKEN_TYPE_INTEGER)
			&& IS_TYPE_T(right_operand, TOKEN_TYPE_INTEGER)) {
		type = TOKEN_TYPE_INTEGER;
	}

	switch(oper.get_type()) {
		case TOKEN_TYPE_BINARY_OPERATOR:

			if(type != TOKEN_TYPE_INTEGER) {
				TRACE_EVENT("Expecting left/right integer operand token types: " 
					<< left_operand.to_string(true) << ", " << right_operand.to_string(true), TRACE_TYPE_ERROR);
				THROW_EXE_EXT_EXC_W_MESS(left_operand.to_string(true) << ", " << right_operand.to_string(true), EXE_EXT_EXC_INVAL_INT_OPER_TOKEN);
			}

			switch(oper.get_subtype()) {
				case BINARY_OPERATOR_TYPE_AND:
					i_val = left_operand.to_integer() & right_operand.to_integer();
					break;
				case BINARY_OPERATOR_TYPE_OR:
					i_val = left_operand.to_integer() | right_operand.to_integer();
					break;
				case BINARY_OPERATOR_TYPE_XOR:
					i_val = left_operand.to_integer() ^ right_operand.to_integer();
					break;
			}
			break;
		case TOKEN_TYPE_SYMBOL:

			if(!IS_SUBTYPE_T(oper, SYMBOL_TYPE_ARITHMETIC_ADD)
					&& type == TOKEN_TYPE_VAR_STRING) {
				TRACE_EVENT("Expecting left string operand token types: " << left_operand.to_string(true) << ", " << right_operand.to_string(true), TRACE_TYPE_ERROR);
				THROW_EXE_EXT_EXC_W_MESS(left_operand.to_string(true) << ", " << right_operand.to_string(true), EXE_EXT_EXC_INVAL_STRING_OPER_TOKEN);
			}

			switch(oper.get_subtype()) {
				case SYMBOL_TYPE_ARITHMETIC_ADD:

					switch(type) {
						case TOKEN_TYPE_FLOAT:
							f_val = left_operand.to_float() + right_operand.to_float();
							break;
						case TOKEN_TYPE_INTEGER:
							i_val = left_operand.to_integer() + right_operand.to_integer();
							break;
						case TOKEN_TYPE_VAR_STRING:
							s_val = left_operand.get_text() + right_operand.get_text();
							break;
					}
					break;
				case SYMBOL_TYPE_ARITHMETIC_DIVIDE:

					switch(type) {
						case TOKEN_TYPE_FLOAT:

							if(!left_operand.to_float()
									|| !right_operand.to_float()) {
								TRACE_EVENT("Attempting to divide by zero: " 
									<< left_operand.to_string(true) << ", " << right_operand.to_string(true), TRACE_TYPE_ERROR);
								THROW_EXE_EXT_EXC_W_MESS(left_operand.to_string(true) << ", " << right_operand.to_string(true), EXE_EXT_EXC_DIVIDE_BY_ZERO);
							}
							f_val = left_operand.to_float() / right_operand.to_float();
							break;
						case TOKEN_TYPE_INTEGER:

							if(!left_operand.to_integer()
									|| !right_operand.to_integer()) {
								TRACE_EVENT("Attempting to divide by zero: " 
									<< left_operand->to_string(true) << ", " << right_operand->to_string(true), TRACE_TYPE_ERROR);
								THROW_EXE_EXT_EXC_W_MESS(left_operand.to_string(true) << ", " << right_operand.to_string(true), EXE_EXT_EXC_DIVIDE_BY_ZERO);
							}
							i_val = left_operand.to_integer() / right_operand.to_integer();
							break;
					}
					break;
				case SYMBOL_TYPE_ARITHMETIC_MODULUS:

					if(type != TOKEN_TYPE_INTEGER) {
						TRACE_EVENT("Expecting left/right integer operand token types: " 
							<< left_operand.to_string(true) << ", " << right_operand.to_string(true), TRACE_TYPE_ERROR);
						THROW_EXE_EXT_EXC_W_MESS(left_operand.to_string(true) << ", " << right_operand.to_string(true), EXE_EXT_EXC_INVAL_INT_OPER_TOKEN);
					}
					i_val = left_operand.to_integer() % right_operand.to_integer();
					break;
				case SYMBOL_TYPE_ARITHMETIC_MULTIPLY:
						
					switch(type) {
						case TOKEN_TYPE_FLOAT:
							f_val = left_operand.to_float() * right_operand.to_float();
							break;
						case TOKEN_TYPE_INTEGER:
							i_val = left_operand.to_integer() * right_operand.to_integer();
							break;
					}
					break;
				case SYMBOL_TYPE_ARITHMETIC_SUBTRACT:

					switch(type) {
						case TOKEN_TYPE_FLOAT:
							f_val = left_operand.to_float() - right_operand.to_float();
							break;
						case TOKEN_TYPE_INTEGER:
							i_val = left_operand.to_integer() - right_operand.to_integer();
							break;
					}
					break;
			}
			break;
	}

	switch(type) {
		case TOKEN_TYPE_FLOAT:
			ss << f_val;
			break;
		case TOKEN_TYPE_INTEGER:
			ss << i_val;
			break;
		case TOKEN_TYPE_VAR_STRING:
			ss << s_val;
			break;
	}
	EXE_SET_TOKEN_TYPE(value_tok, type, ss.str());
	stack.push(exe_create_statement_list(value_tok));

	TRACE_EVENT("-exe_eval_operation", TRACE_TYPE_INFORMATION);
}

void 
exe_eval_out_list(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack
	)
{
	TRACE_EVENT("+exe_eval_out_list", TRACE_TYPE_INFORMATION);

	tok value_tok;
	tok_stmt value_stmt;
	std::stringstream ss;
	size_t i = 0, subtype;
	tok_stmt_vector value_stmt_vec;

	if(!IS_TYPE_T(statement.get(), TOKEN_TYPE_OUT_LIST_OPERATOR)) {
		TRACE_EVENT("Invalid out-list operator token: " << statement.get().to_string(true), TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC_W_MESS(statement.get().to_string(true), EXE_EXT_EXC_INVAL_OUT_LIST_TOKEN);
	}
	subtype = statement.get().get_subtype();
	statement.move_child_front();

	for(; i < statement.get_child_count(); ++i) {
		statement.move_child(i);
		value_stmt_vec = exe_cont_get_value(statement.get().get_text(), context, false);

		switch(subtype) {
			case OUT_LIST_OPERATOR_TYPE_POP_END:
				value_stmt = value_stmt_vec.back();
				value_stmt_vec.erase(value_stmt_vec.end() - 1);
				exe_cont_set_value(statement.get().get_text(), value_stmt_vec, context, false);
				stack.push(exe_create_statement_list(value_stmt));
				break;
			case OUT_LIST_OPERATOR_TYPE_POP_FRONT:
				value_stmt = value_stmt_vec.front();
				value_stmt_vec.erase(value_stmt_vec.begin());
				exe_cont_set_value(statement.get().get_text(), value_stmt_vec, context, false);
				stack.push(exe_create_statement_list(value_stmt));
				break;
			case OUT_LIST_OPERATOR_TYPE_SIZE:
				ss << value_stmt_vec.size();
				EXE_SET_TOKEN_TYPE(value_tok, TOKEN_TYPE_INTEGER, ss.str());
				stack.push(exe_create_statement_list(value_tok));
				break;
		}
		statement.move_parent();
	}
	statement.move_parent();

	TRACE_EVENT("-exe_eval_out_list", TRACE_TYPE_INFORMATION);
}

bool 
exe_eval_range_statement(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack
	)
{
	TRACE_EVENT("+exe_eval_range_statement", TRACE_TYPE_INFORMATION);

	bool result = false;

	if(!IS_CLASS_TYPE_T(statement.get(), CLASS_TOKEN_TYPE_RANGE_STATEMENT)) {
		TRACE_EVENT("Invalid range statement class token: " << statement.get().to_string(true), TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC_W_MESS(statement.get().to_string(true), EXE_EXT_EXC_INVAL_RANGE_STMT_CLASS_TOKEN);
	}
	statement.move_child(RANGE_STMT_COND_STMT_CHILD);
	result = exe_eval_conditional_statement_list(statement, context, stack, RANGE_STMT_COND_STMT_CHILD, COND_STMT_SEPERATOR_CHILD);
	statement.move_parent();

	TRACE_EVENT("-exe_eval_range_statement", TRACE_TYPE_INFORMATION);

	return result;
}

size_t 
exe_eval_statement(
	const tok_vector &arguments,
	par_ptr parser,
	cont_tree &context,
	tok_stack &stack
	)
{
	size_t signal = EXE_SIG_NONE;

	if(parser) {
		exe_args = arguments;
		exe_par = parser;
		signal = exe_eval_statement(parser->get(), context, stack, 0, true);
		exe_advance_parser(parser);
	} else {
		TRACE_EVENT("Invalid executor evaluation action parameter!", TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC(EXE_EXT_EXC_INVAL_EVAL_ACTION_PARAM);
	}

	return signal;
}

size_t 
exe_eval_statement(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack,
	size_t index,
	bool root
	)
{
	TRACE_EVENT("+exe_eval_statement", TRACE_TYPE_INFORMATION);

	size_t signal = EXE_SIG_NONE;
		
	if(!IS_CLASS_TYPE_T(statement.get(), CLASS_TOKEN_TYPE_STATEMENT)) {
		TRACE_EVENT("Invalid statement class token: " << statement.get().to_string(true), TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC_W_MESS(statement.get().to_string(true), EXE_EXT_EXC_INVAL_STMT_CLASS_TOKEN);
	}
	statement.move_child_front();

	if(!IS_CLASS_TYPE_T(statement.get(), CLASS_TOKEN_TYPE_TOKEN)) {
		TRACE_EVENT("Invalid token class token: " << statement.get().to_string(true), TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC_W_MESS(statement.get().to_string(true), EXE_EXT_EXC_INVAL_TOKEN_CLASS_TOKEN);
	}

	switch(statement.get().get_type()) {
		case TOKEN_TYPE_ASSIGNMENT:
			exe_eval_assignment(statement, context, stack);
			break;
		case TOKEN_TYPE_CALL:
			exe_eval_call(statement, context, stack);
			break;
		case TOKEN_TYPE_CONDITIONAL:
			signal = exe_eval_conditional(statement, context, stack);
			break;
		case TOKEN_TYPE_CONTROL:
			signal = exe_eval_control(statement, context, stack);
			break;
		case TOKEN_TYPE_IO_OPERATOR:
			exe_eval_io(statement, context, stack);
			break;
		case TOKEN_TYPE_IN_LIST_OPERATOR:
			exe_eval_in_list(statement, context, stack);
			break;
		case TOKEN_TYPE_ITERATOR_OPERATOR:
			exe_eval_iterator(statement, context, stack, false);
			break;
		case TOKEN_TYPE_OUT_LIST_OPERATOR:
			exe_eval_out_list(statement, context, stack);
			break;
		case TOKEN_TYPE_USING:
			exe_eval_using(statement, context, stack, index, root);
			break;
		default:
			TRACE_EVENT("Invalid statement token: " << statement.get().to_string(true), TRACE_TYPE_ERROR);
			THROW_EXE_EXT_EXC_W_MESS(statement.get().to_string(true), EXE_EXT_EXC_INVAL_STMT_TOKEN);
	}
	statement.move_parent();

	TRACE_EVENT("-exe_eval_statement", TRACE_TYPE_INFORMATION);

	return signal;
}

size_t 
exe_eval_statement_list(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack
	)
{
	TRACE_EVENT("+exe_eval_statement_list", TRACE_TYPE_INFORMATION);

	size_t i = 0, signal = EXE_SIG_NONE;

	if(!IS_CLASS_TYPE_T(statement.get(), CLASS_TOKEN_TYPE_STATEMENT_LIST)) {
		TRACE_EVENT("Invalid statement list class token: " << statement.get().to_string(true), TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC_W_MESS(statement.get().to_string(true), EXE_EXT_EXC_INVAL_STMT_LIST_CLASS_TOKEN);
	}

	for(; i < statement.get_child_count(); ++i) {
		statement.move_child(i);
		signal = exe_eval_statement(statement, context, stack, i, false);
		statement.move_parent();

		if(signal != EXE_SIG_NONE) {
			break;
		}
	}

	TRACE_EVENT("-exe_eval_statement_list", TRACE_TYPE_INFORMATION);

	return signal;
}

void 
exe_eval_subroutine(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack,
	size_t index
	)
{
	TRACE_EVENT("+exe_eval_subroutine", TRACE_TYPE_INFORMATION);
	
	cont sub_cont;
	size_t i, count = 0;
	tok_stmt funct_schema;
	tok_stmt_vector funct_schema_vec = exe_cont_get_value(statement.get().get_text(), context, false);

	UNREF_PARAM(index);

	if(index >= funct_schema_vec.size()) {
		TRACE_EVENT("Attempting to reference non-existent index: " << index, TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC_W_MESS(statement.get().get_text() << "[" << index << "]", EXE_EXT_EXC_REF_TO_NON_EXISTENT_INDEX);
	}
	funct_schema = funct_schema_vec.at(index);

	if(!funct_schema.has_children()) {
		TRACE_EVENT("Attempting to reference non-existent function: " << statement.get().to_string(true), TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC_W_MESS(statement.get().to_string(true), EXE_EXT_EXC_REF_TO_NON_EXISTENT_FUNCT);
	}
	statement.move_child_front();

	if(!IS_CLASS_TYPE_T(statement.get(), CLASS_TOKEN_TYPE_FUNCTION_PARAMETER_LIST)) {
		TRACE_EVENT("Invalid function parameter class token: " << statement.get().get_type, TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC_W_MESS(statement.get().get_type(), EXE_EXT_EXC_INVAL_FUNCT_PARAM_CLASS_TOKEN);
	}
	statement.move_child_front();
	funct_schema.move_child(FUNCT_SCHEMA_ID_PARAM_LIST_CHILD);

	if((!statement.has_children() && funct_schema.has_children())
			|| (!funct_schema.has_children() && statement.has_children())) {
		TRACE_EVENT("Invalid function parameter count: " << statement.get_child_count() << ", must be " << funct_schema.get_child_count(), TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC_W_MESS(statement.get_child_count() << ", must be " << funct_schema.get_child_count(), EXE_EXT_EXC_INVAL_FUNCT_PARAM_COUNT);
	} else if(statement.has_children()
			&& funct_schema.has_children()) {
		statement.move_child_front();
		count = statement.get_child_count();

		if(funct_schema.get_child_count() != count) {
			TRACE_EVENT("Invalid function parameter count: " << count << ", must be " << funct_schema.get_child_count(), TRACE_TYPE_ERROR);
			THROW_EXE_EXT_EXC_W_MESS(count << ", must be " << funct_schema.get_child_count(), EXE_EXT_EXC_INVAL_FUNCT_PARAM_COUNT);
		}

		for(i = 0; i < count; ++i) {
			statement.move_child(i);
			funct_schema.move_child(i);
			exe_eval_expression(statement, context, stack);
			sub_cont.set_value(funct_schema.get().get_text(), stack.top());
			stack.pop();
			funct_schema.move_parent();
			statement.move_parent();
		}
		statement.move_parent();
	}
	statement.move_parent();
	statement.move_parent();
	funct_schema.move_parent();
	context.add_child_back(sub_cont);
	context.move_child_back();
	funct_schema.move_child(FUNCT_SCHEMA_STMT_LIST_CHILD);
	exe_eval_statement_list(funct_schema, context, stack);
	funct_schema.move_parent();
	funct_schema.move_child(FUNCT_SCHEMA_EXPR_PARAM_LIST_CHILD);

	if(funct_schema.has_children()) {
		funct_schema.move_child_front();

		for(i = 0; i < funct_schema.get_child_count(); ++i) {
			funct_schema.move_child(i);
			exe_eval_expression(funct_schema, context, stack);
			funct_schema.move_parent();
		}
		funct_schema.move_parent();
	}
	funct_schema.move_parent();
	context.move_parent();
	context.remove_children();

	TRACE_EVENT("-exe_eval_subroutine", TRACE_TYPE_INFORMATION);
}

void 
exe_eval_using(
	tok_stmt &statement,
	cont_tree &context,
	tok_stack &stack,
	size_t index,
	bool root
	)
{
	TRACE_EVENT("+exe_eval_using", TRACE_TYPE_INFORMATION);

	size_t i = 1;
	par using_par;

	UNREF_PARAM(context);
	UNREF_PARAM(stack);

	if(exe_par) {

		if(!IS_TYPE_T(statement.get(), TOKEN_TYPE_USING)) {
			TRACE_EVENT("Invalid using token: " << statement.get().get_type, TRACE_TYPE_ERROR);
			THROW_EXE_EXT_EXC_W_MESS(statement.get().get_type(), EXE_EXT_EXC_INVAL_USING_TOKEN);
		}
		statement.move_child_front();
		using_par.set_action(par_config_lexer, PAR_ACTION_CONFIG_LEXER);
		using_par.set_enumeration_action(par_enum_statement, PAR_ENUM_ACTION_STATEMENT);
		exe_eval_expression(statement, context, stack);
		using_par.initialize(stack.top().front().get().get_text(), true);
		stack.pop();
		using_par.enumerate();
		statement.move_parent();

		if(root) {
			exe_par->import_statements(using_par.export_statements());
		} else {
			statement.move_parent();
			statement.move_parent();

			for(; i < using_par.export_statements().get_link_count() - 1; ++i) {
				statement.insert_child_node(using_par.export_statement(i).get_root_node(), index + i);
			}
			statement.move_child(index);
			statement.move_child_front();
		}
	} else {
		TRACE_EVENT("Invalid executor evaluation action parameter!", TRACE_TYPE_ERROR);
		THROW_EXE_EXT_EXC(EXE_EXT_EXC_INVAL_EVAL_ACTION_PARAM);
	}

	TRACE_EVENT("-exe_eval_using", TRACE_TYPE_INFORMATION);
}