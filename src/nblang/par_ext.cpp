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

#include <sstream>
#include <stdexcept>
#include "lang.h"
#include "lex.h"
#include "lex_ext.h"
#include "par_ext.h"
#include "par_ext_type.h"
#include "tok.h"

//#define PARSER_EXT_TRACING
#ifndef PARSER_EXT_TRACING
#define TRACE_EVENT(_M_, _T_)
#else
#define TRACE_HEADER "parser_ext"
#include "trace.h"
#endif

void
par_add_class_token(
	lex_ptr lexer,
	tok_stmt &statement,
	size_t type,
	bool root,
	bool back
	)
{
	TRACE_EVENT("+par_add_class_token", TRACE_TYPE_VERBOSE);

	if(lexer) {
		lexer->insert_class_token(type);
		par_add_token(lexer, statement, root, back);
		lexer->move_next();
	} else {
		TRACE_EVENT("Invalid parser parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_PARAM);
	}

	TRACE_EVENT("-par_add_class_token", TRACE_TYPE_VERBOSE);
}

void
par_add_class_token_back(
	lex_ptr lexer,
	tok_stmt &statement,
	size_t type
	)
{
	par_add_class_token(lexer, statement, type, false, true);
}

void
par_add_class_token_front(
	lex_ptr lexer,
	tok_stmt &statement,
	size_t type
	)
{
	par_add_class_token(lexer, statement, type, false, false);
}

void 
par_add_token(
	lex_ptr lexer,
	tok_stmt &statement,
	bool root,
	bool back
	)
{
	TRACE_EVENT("+par_add_token", TRACE_TYPE_VERBOSE);

	if(lexer) {

		if(root) {
			statement.get_root() = GET_TOKEN_L(lexer);
		} else if(back) {
			statement.add_child_back(GET_TOKEN_L(lexer));
		} else {
			statement.add_child_front(GET_TOKEN_L(lexer));
		}
	} else {
		TRACE_EVENT("Invalid parser parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_PARAM);
	}

	TRACE_EVENT("-par_add_token", TRACE_TYPE_VERBOSE);
}

void 
par_add_token_back(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	par_add_token(lexer, statement, false, true);
}

void 
par_add_token_front(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	par_add_token(lexer, statement, false, false);
}

void 
par_advance_lexer(
	lex_ptr lexer
	)
{
	par_advance_lexer(lexer, false);
}

void 
par_advance_lexer(
	lex_ptr lexer,
	bool exp_eos
	)
{
	TRACE_EVENT("+par_advance_lexer", TRACE_TYPE_VERBOSE);

	if(lexer) {

		if(lexer->has_next()) {
			lexer->move_next();
		} else if(!exp_eos) {
			TRACE_EVENT("Unexpected end of stream detected in parser!", TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC(PAR_EXT_EXC_UNEXPECT_EOS);
		}
	} else {
		TRACE_EVENT("Invalid parser parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_PARAM);
	}

	TRACE_EVENT("-par_advance_lexer", TRACE_TYPE_VERBOSE);
}

void 
par_config_lexer(
	lex_ptr lexer
	)
{
	TRACE_EVENT("+par_config_lexer", TRACE_TYPE_INFORMATION);

	if(lexer) {
		lexer->set_action(lex_skip_whitespace, LEX_ACTION_SKIP_WHITESPACE);
		lexer->set_enumeration_action(lex_enum_alpha, LEX_ENUM_ACTION_ALPHA);
		lexer->set_enumeration_action(lex_enum_number, LEX_ENUM_ACTION_NUMBER);
		lexer->set_enumeration_action(lex_enum_symbol, LEX_ENUM_ACTION_SYMBOL);
	} else {
		TRACE_EVENT("Invalid parser action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ACTION_PARAM);
	}

	TRACE_EVENT("-par_config_lexer", TRACE_TYPE_INFORMATION);
}

void 
par_enum_assignment(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_assignment", TRACE_TYPE_INFORMATION);

	if(lexer) {
		
		if(!IS_TYPE_L(lexer, TOKEN_TYPE_ASSIGNMENT)) {
			TRACE_EVENT("Invalid assignment token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_ASSIGNMENT);
		}
		par_add_token_back(lexer, statement);
		statement.move_child_back();
		par_advance_lexer(lexer);

		if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_STATEMENT_SEPERATOR)) {
			TRACE_EVENT("Invalid statement seperator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_STATEMENT_SEPERATOR);
		}
		par_advance_lexer(lexer);
		par_enum_identifier_list(lexer, statement);

		if(IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_OPEN_BRACE)) {
			par_add_class_token_back(lexer, statement, CLASS_TOKEN_TYPE_LIST_INDEX);
			statement.move_child_back();
			par_advance_lexer(lexer);
			par_enum_expression_root(lexer, statement);

			if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_CLOSE_BRACE)) {
				TRACE_EVENT("Invalid close brace token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
				THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_CLOSE_BRACE);
			}
			par_advance_lexer(lexer);
			statement.move_parent();
		}

		if(IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_LIST_SEPERATOR)) {
			par_advance_lexer(lexer);

			if(IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_OPEN_BRACE)) {
				par_add_class_token_back(lexer, statement, CLASS_TOKEN_TYPE_COMPOUND_ASSIGNMENT);
				statement.move_child_back();
				par_advance_lexer(lexer);
				par_enum_expression_list(lexer, statement);

				if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_CLOSE_BRACE)) {
					TRACE_EVENT("Invalid close brace token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
					THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_CLOSE_BRACE);
				}
				par_advance_lexer(lexer);
				statement.move_parent();
			} else {
				par_add_class_token_back(lexer, statement, CLASS_TOKEN_TYPE_SIMPLE_ASSIGNMENT);
				statement.move_child_back();
				par_enum_expression_root(lexer, statement);
				statement.move_parent();
			}
		} else if(IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_OPEN_PARENTHESIS)) {
			par_add_class_token_back(lexer, statement, CLASS_TOKEN_TYPE_FUNCTION_ASSIGNMENT);
			statement.move_child_back();
			par_advance_lexer(lexer);
			par_enum_identifier_parameter_list(lexer, statement);

			if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_CLOSE_PARENTHESIS)) {
				TRACE_EVENT("Invalid close parenthesis token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
				THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_CLOSE_PARENTHESIS);
			}
			par_advance_lexer(lexer);
			par_enum_statement_list(lexer, statement, false);
			par_enum_expression_parameter_list(lexer, statement);
			statement.move_parent();
		} else {
			TRACE_EVENT("Invalid list seperator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_LIST_SEPERATOR);
		}

		if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_TERMINATOR)) {
			TRACE_EVENT("Invalid terminator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_TERMINATOR);
		}
		par_advance_lexer(lexer);
		statement.move_parent();
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_assignment", TRACE_TYPE_INFORMATION);
}

void 
par_enum_argument(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_argument", TRACE_TYPE_INFORMATION);

	if(lexer) {

		if(!IS_TYPE_L(lexer, TOKEN_TYPE_ARGUMENT)) {
			TRACE_EVENT("Invalid argument token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_ARGUMENT);
		}
		par_add_token_back(lexer, statement);
		statement.move_child_back();
		par_advance_lexer(lexer);

		switch(statement.get().get_subtype()) {
			case ARGUMENT_TYPE_ARG:

				if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_STATEMENT_SEPERATOR)) {
					TRACE_EVENT("Invalid statement seperator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
					THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_STATEMENT_SEPERATOR);
				}
				par_advance_lexer(lexer);
				par_enum_expression_root(lexer, statement);
				break;
			case ARGUMENT_TYPE_ARG_COUNT:
				break;
			default:
				TRACE_EVENT("Invalid argument token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
				THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_ARGUMENT);
		}
		statement.move_parent();
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_argument", TRACE_TYPE_INFORMATION);
}

void 
par_enum_call(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_call", TRACE_TYPE_INFORMATION);
	
	if(lexer) {

		if(!IS_TYPE_L(lexer, TOKEN_TYPE_CALL)) {
			TRACE_EVENT("Invalid call statement token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_CALL);
		}
		par_add_token_back(lexer, statement);
		statement.move_child_back();
		par_advance_lexer(lexer);

		if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_STATEMENT_SEPERATOR)) {
			TRACE_EVENT("Invalid statement seperator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_STATEMENT_SEPERATOR);			
		}
		par_advance_lexer(lexer);
		par_enum_expression_root(lexer, statement);

		if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_TERMINATOR)) {
			TRACE_EVENT("Invalid terminator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_TERMINATOR);
		}
		par_advance_lexer(lexer);
		statement.move_parent();
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_call", TRACE_TYPE_INFORMATION);
}

void 
par_enum_conditional(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_conditional", TRACE_TYPE_INFORMATION);

	size_t subtype;
	
	if(lexer) {

		if(!IS_TYPE_L(lexer, TOKEN_TYPE_CONDITIONAL)) {
			TRACE_EVENT("Invalid conditional statement token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_CONDITIONAL);
		}
		par_add_token_back(lexer, statement);
		statement.move_child_back();
		subtype = statement.get().get_subtype();
		par_advance_lexer(lexer);

		if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_STATEMENT_SEPERATOR)) {
			TRACE_EVENT("Invalid statement seperator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_STATEMENT_SEPERATOR);			
		}
		par_advance_lexer(lexer);

		switch(subtype) {
			case CONDITIONAL_TYPE_IF:
			case CONDITIONAL_TYPE_WHILE:
				par_enum_conditional_statement_list(lexer, statement, 0, 1);
				par_enum_statement_list(lexer, statement, false);
				break;
			case CONDITIONAL_TYPE_IFELSE:
				par_enum_conditional_statement_list(lexer, statement, 0, 1);
				par_enum_statement_list(lexer, statement, false);
				par_enum_statement_list(lexer, statement, true);
				break;
			case CONDITIONAL_TYPE_RANGE:
				par_enum_range_statement(lexer, statement);
				par_enum_statement_list(lexer, statement, false);
				break;
			case CONDITIONAL_TYPE_UNTIL:
				par_enum_statement_list(lexer, statement, false);
				par_enum_conditional_statement_list(lexer, statement, 1, 2);

				if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_TERMINATOR)) {
					TRACE_EVENT("Invalid terminator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
					THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_TERMINATOR);
				}
				par_advance_lexer(lexer);
				break;
			default:
				TRACE_EVENT("Invalid conditional token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
				THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_CONDITIONAL);
		}
		statement.move_parent();
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_conditional", TRACE_TYPE_INFORMATION);
}

void 
par_enum_conditional_statement(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_conditional_statement", TRACE_TYPE_INFORMATION);

	if(lexer) {
		par_add_class_token_back(lexer, statement, CLASS_TOKEN_TYPE_CONDITIONAL_STATEMENT);
		statement.move_child_back();

		if(!IS_TYPE_L(lexer, TOKEN_TYPE_COMPARATOR)) {
			TRACE_EVENT("Invalid comparator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_COMPARATOR);
		}
		par_add_token_back(lexer, statement);
		par_advance_lexer(lexer);

		if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_COMPARATOR)) {
			TRACE_EVENT("Invalid comparator seperator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_COMPARATOR_SEPERATOR);
		}
		par_advance_lexer(lexer);
		par_enum_expression_root(lexer, statement);

		if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_LIST_SEPERATOR)) {
			TRACE_EVENT("Invalid list seperator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_LIST_SEPERATOR);
		}
		par_advance_lexer(lexer);
		par_enum_expression_root(lexer, statement);
		statement.move_parent();
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_conditional_statement", TRACE_TYPE_INFORMATION);
}

void 
par_enum_conditional_statement_list(
	lex_ptr lexer,
	tok_stmt &statement,
	size_t cond_index,
	size_t sep_index
	)
{
	TRACE_EVENT("+par_enum_conditional_statement_list", TRACE_TYPE_INFORMATION);

	if(lexer) {
		par_add_class_token_back(lexer, statement, CLASS_TOKEN_TYPE_CONDITIONAL_STATEMENT_LIST);
		par_add_class_token_back(lexer, statement, CLASS_TOKEN_TYPE_CONDITIONAL_SEPERATOR_LIST);
		statement.move_child(cond_index);

		if(IS_TYPE_L(lexer, TOKEN_TYPE_COMPARATOR)) {
			par_enum_conditional_statement(lexer, statement);
		} else if(IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_OPEN_PARENTHESIS)) {
			par_advance_lexer(lexer);
			par_enum_conditional_statement_list_helper(lexer, statement, cond_index, sep_index);

			if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_CLOSE_PARENTHESIS)) {
				TRACE_EVENT("Invalid close parenthesis token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
				THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_CLOSE_PARENTHESIS);
			}
			par_advance_lexer(lexer);
		} else {
			TRACE_EVENT("Invalid comparator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_COMPARATOR);
		}
		statement.move_parent();
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_conditional_statement_list", TRACE_TYPE_INFORMATION);
}

void 
par_enum_conditional_statement_list_helper(
	lex_ptr lexer,
	tok_stmt &statement,
	size_t cond_index,
	size_t sep_index
	)
{
	TRACE_EVENT("+par_enum_conditional_statement_list_helper", TRACE_TYPE_INFORMATION);

	if(lexer) {

		if(!IS_TYPE_L(lexer, TOKEN_TYPE_COMPARATOR)) {
			TRACE_EVENT("Invalid comparator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_COMPARATOR);
		}
		par_enum_conditional_statement(lexer, statement);

		if(IS_TYPE_L(lexer, TOKEN_TYPE_CONDITIONAL_SEPERATOR)) {
			statement.move_parent();
			statement.move_child(sep_index);
			par_add_token_back(lexer, statement);
			statement.move_parent();
			statement.move_child(cond_index);
			par_advance_lexer(lexer);
			par_enum_conditional_statement_list_helper(lexer, statement, cond_index, sep_index);
		}
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_conditional_statement_list_helper", TRACE_TYPE_INFORMATION);
}

void 
par_enum_control(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_control", TRACE_TYPE_INFORMATION);

	if(lexer) {

		if(!IS_TYPE_L(lexer, TOKEN_TYPE_CONTROL)) {
			TRACE_EVENT("Invalid control token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_CONTROL);
		}
		par_add_token_back(lexer, statement);
		par_advance_lexer(lexer);

		if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_TERMINATOR)) {
			TRACE_EVENT("Invalid terminator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_TERMINATOR);
		}
		par_advance_lexer(lexer);
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_control", TRACE_TYPE_INFORMATION);
}

void 
par_enum_expression_depth_0(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_expression_depth_0", TRACE_TYPE_VERBOSE);

	if(lexer) {
		par_enum_expression_depth_1(lexer, statement);
		par_enum_expression_depth_0_terminal(lexer, statement);
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_expression_depth_0", TRACE_TYPE_VERBOSE);
}

void 
par_enum_expression_depth_0_terminal(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_expression_depth_0_terminal", TRACE_TYPE_VERBOSE);

	if(lexer) {
		
		if(IS_TYPE_L(lexer, TOKEN_TYPE_BINARY_OPERATOR)) {
			par_add_token_back(lexer, statement);
			statement.move_child_back();
			par_add_class_token_back(lexer, statement, CLASS_TOKEN_TYPE_EXPRESSION);
			statement.move_child_back();
			par_advance_lexer(lexer);
			par_enum_expression_depth_1(lexer, statement);
			par_enum_expression_depth_0_terminal(lexer, statement);
			statement.move_parent();
			statement.move_parent();
		}
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_expression_depth_0_terminal", TRACE_TYPE_VERBOSE);
}

void 
par_enum_expression_depth_1(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_expression_depth_1", TRACE_TYPE_VERBOSE);

	if(lexer) {
		par_enum_expression_depth_2(lexer, statement);
		par_enum_expression_depth_1_terminal(lexer, statement);
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_expression_depth_1", TRACE_TYPE_VERBOSE);
}

void 
par_enum_expression_depth_1_terminal(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_expression_depth_1_terminal", TRACE_TYPE_VERBOSE);

	if(lexer) {

		if(IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_ARITHMETIC_SUBTRACT)) {
			par_add_token_back(lexer, statement);
			statement.move_child_back();
			par_add_class_token_back(lexer, statement, CLASS_TOKEN_TYPE_EXPRESSION);
			statement.move_child_back();
			par_advance_lexer(lexer);
			par_enum_expression_depth_2(lexer, statement);
			par_enum_expression_depth_1_terminal(lexer, statement);
			statement.move_parent();
			statement.move_parent();
		}
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_expression_depth_1_terminal", TRACE_TYPE_VERBOSE);
}

void 
par_enum_expression_depth_2(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_expression_depth_2", TRACE_TYPE_VERBOSE);

	if(lexer) {
		par_enum_expression_depth_3(lexer, statement);
		par_enum_expression_depth_2_terminal(lexer, statement);
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_expression_depth_2", TRACE_TYPE_VERBOSE);
}

void 
par_enum_expression_depth_2_terminal(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_expression_depth_2_terminal", TRACE_TYPE_VERBOSE);

	if(lexer) {

		if(IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_ARITHMETIC_ADD)) {
			par_add_token_back(lexer, statement);
			statement.move_child_back();
			par_add_class_token_back(lexer, statement, CLASS_TOKEN_TYPE_EXPRESSION);
			statement.move_child_back();
			par_advance_lexer(lexer);
			par_enum_expression_depth_3(lexer, statement);
			par_enum_expression_depth_2_terminal(lexer, statement);
			statement.move_parent();
			statement.move_parent();
		}
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_expression_depth_2_terminal", TRACE_TYPE_VERBOSE);
}

void 
par_enum_expression_depth_3(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_expression_depth_3", TRACE_TYPE_VERBOSE);

	if(lexer) {
		par_enum_expression_depth_4(lexer, statement);
		par_enum_expression_depth_3_terminal(lexer, statement);
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_expression_depth_3", TRACE_TYPE_VERBOSE);
}

void 
par_enum_expression_depth_3_terminal(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_expression_depth_3_terminal", TRACE_TYPE_VERBOSE);

	if(lexer) {

		if(IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_ARITHMETIC_DIVIDE)
				|| IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_ARITHMETIC_MODULUS)) {
			par_add_token_back(lexer, statement);
			statement.move_child_back();
			par_add_class_token_back(lexer, statement, CLASS_TOKEN_TYPE_EXPRESSION);
			statement.move_child_back();
			par_advance_lexer(lexer);
			par_enum_expression_depth_4(lexer, statement);
			par_enum_expression_depth_3_terminal(lexer, statement);
			statement.move_parent();
			statement.move_parent();
		}
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_expression_depth_3_terminal", TRACE_TYPE_VERBOSE);
}

void 
par_enum_expression_depth_4(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_expression_depth_4", TRACE_TYPE_VERBOSE);

	if(lexer) {
		par_enum_expression_leaf(lexer, statement);
		par_enum_expression_depth_4_terminal(lexer, statement);
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_expression_depth_4", TRACE_TYPE_VERBOSE);
}

void 
par_enum_expression_depth_4_terminal(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_expression_depth_4_terminal", TRACE_TYPE_VERBOSE);

	if(lexer) {

		if(IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_ARITHMETIC_MULTIPLY)) {
			par_add_token_back(lexer, statement);
			statement.move_child_back();
			par_add_class_token_back(lexer, statement, CLASS_TOKEN_TYPE_EXPRESSION);
			statement.move_child_back();
			par_advance_lexer(lexer);
			par_enum_expression_leaf(lexer, statement);
			par_enum_expression_depth_4_terminal(lexer, statement);
			statement.move_parent();
			statement.move_parent();
		}
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_expression_depth_4_terminal", TRACE_TYPE_VERBOSE);
}

void 
par_enum_expression_leaf(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_expression_leaf", TRACE_TYPE_VERBOSE);

	if(lexer) {
		
		switch(lexer->get().get_type()) {
			case TOKEN_TYPE_ACCESS:
			case TOKEN_TYPE_ITERATOR_OPERATOR:
			case TOKEN_TYPE_OUT_LIST_OPERATOR:
				par_enum_identifier_operator(lexer, statement);
				break;
			case TOKEN_TYPE_ARGUMENT:
				par_enum_argument(lexer, statement);
				break;
			case TOKEN_TYPE_CONSTANT:
			case TOKEN_TYPE_FLOAT:
			case TOKEN_TYPE_INTEGER:
			case TOKEN_TYPE_VAR_STRING:
				par_add_token_back(lexer, statement);
				par_advance_lexer(lexer);
				break;
			case TOKEN_TYPE_IDENTIFIER:
				par_enum_identifier(lexer, statement);
				break;
			case TOKEN_TYPE_IO_OPERATOR:
				par_enum_io(lexer, statement, false);
				break;
			case TOKEN_TYPE_CONVERSION_OPERATOR:
			case TOKEN_TYPE_LOGICAL_OPERATOR:
				par_enum_expression_operator(lexer, statement);
				break;
			case TOKEN_TYPE_SYMBOL:
				par_enum_unary(lexer, statement);
				break;
			default:
				TRACE_EVENT("Invalid expression token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
				THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_EXPRESSION);
		}
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_expression_leaf", TRACE_TYPE_VERBOSE);
}

void 
par_enum_expression_list(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_expression_list", TRACE_TYPE_INFORMATION);

	if(lexer) {
		par_add_class_token_back(lexer, statement, CLASS_TOKEN_TYPE_EXPRESSION_LIST);
		statement.move_child_back();
		par_enum_expression_root(lexer, statement);

		while(IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_LIST_SEPERATOR)) {
			par_advance_lexer(lexer);
			par_enum_expression_root(lexer, statement);
		}
		statement.move_parent();
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_expression_list", TRACE_TYPE_INFORMATION);
}

void 
par_enum_expression_operator(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_expression_operator", TRACE_TYPE_INFORMATION);

	if(lexer) {

		switch(lexer->get().get_type()) {
			case TOKEN_TYPE_CONVERSION_OPERATOR:
			case TOKEN_TYPE_LOGICAL_OPERATOR:
				par_add_token_back(lexer, statement);
				statement.move_child_back();
				par_advance_lexer(lexer);

				if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_STATEMENT_SEPERATOR)) {
					TRACE_EVENT("Invalid statement seperator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
					THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_STATEMENT_SEPERATOR);
				}
				par_advance_lexer(lexer);
				par_enum_expression_root(lexer, statement);
				statement.move_parent();
				break;
			default:
				TRACE_EVENT("Invalid expression operator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
				THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_EXPRESSION_OPERATOR);
				break;
		}
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_expression_operator", TRACE_TYPE_INFORMATION);
}

void 
par_enum_expression_parameter_list(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_expression_parameter_list", TRACE_TYPE_INFORMATION);

	if(lexer) {
		par_add_class_token_back(lexer, statement, CLASS_TOKEN_TYPE_EXPRESSION_PARAMETER_LIST);
		statement.move_child_back();
		
		switch(lexer->get().get_type()) {
			case TOKEN_TYPE_ACCESS:
			case TOKEN_TYPE_ARGUMENT:
			case TOKEN_TYPE_CONSTANT:
			case TOKEN_TYPE_CONVERSION_OPERATOR:
			case TOKEN_TYPE_FLOAT:
			case TOKEN_TYPE_IDENTIFIER:
			case TOKEN_TYPE_INTEGER:
			case TOKEN_TYPE_IO_OPERATOR:
			case TOKEN_TYPE_ITERATOR_OPERATOR:
			case TOKEN_TYPE_LOGICAL_OPERATOR:
			case TOKEN_TYPE_OUT_LIST_OPERATOR:
			case TOKEN_TYPE_VAR_STRING:
				par_enum_expression_list(lexer, statement);
				break;
			case TOKEN_TYPE_SYMBOL:

				switch(lexer->get().get_subtype()) {
					case SYMBOL_TYPE_OPEN_PARENTHESIS:
					case SYMBOL_TYPE_UNARY_NEGATION:
						par_enum_expression_list(lexer, statement);
						break;
				}
				break;
		}
		statement.move_parent();
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_expression_parameter_list", TRACE_TYPE_INFORMATION);
}

void 
par_enum_expression_root(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_expression_root", TRACE_TYPE_INFORMATION);

	if(lexer) {
		par_add_class_token_back(lexer, statement, CLASS_TOKEN_TYPE_EXPRESSION);
		statement.move_child_back();
		par_enum_expression_depth_0(lexer, statement);
		statement.move_parent();
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_expression_root", TRACE_TYPE_INFORMATION);
}

void 
par_enum_file_stream_list(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_file_stream_list", TRACE_TYPE_INFORMATION);

	if(lexer) {
		par_add_class_token_back(lexer, statement, CLASS_TOKEN_TYPE_FILE_STREAM_LIST);
		statement.move_child_back();

		if(IS_TYPE_L(lexer, TOKEN_TYPE_FILE_STREAM_OPERATOR)) {
			par_add_token_back(lexer, statement);
			par_advance_lexer(lexer);
		} else if(IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_OPEN_PARENTHESIS)) {
			par_advance_lexer(lexer);
			par_enum_file_stream_list_helper(lexer, statement);

			if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_CLOSE_PARENTHESIS)) {
				TRACE_EVENT("Invalid close parenthesis token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
				THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_CLOSE_PARENTHESIS);
			}
			par_advance_lexer(lexer);
		} else {
			TRACE_EVENT("Invalid file stream operator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_FILE_STREAM_OPERATOR);
		}
		statement.move_parent();
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_file_stream_list", TRACE_TYPE_INFORMATION);
}

void 
par_enum_file_stream_list_helper(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_file_stream_list_helper", TRACE_TYPE_VERBOSE);

	if(lexer) {
		
		if(!IS_TYPE_L(lexer, TOKEN_TYPE_FILE_STREAM_OPERATOR)) {
			TRACE_EVENT("Invalid file stream operator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_FILE_STREAM_OPERATOR);
		}
		par_add_token_back(lexer, statement);
		par_advance_lexer(lexer);

		if(IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_LIST_SEPERATOR)) {
			par_advance_lexer(lexer);
			par_enum_file_stream_list_helper(lexer, statement);
		}
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_file_stream_list_helper", TRACE_TYPE_VERBOSE);
}

void 
par_enum_identifier(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_identifier", TRACE_TYPE_INFORMATION);

	if(lexer) {
		
		if(!IS_TYPE_L(lexer, TOKEN_TYPE_IDENTIFIER)) {
			TRACE_EVENT("Invalid identifier token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_IDENTIFIER);
		}
		par_add_token_back(lexer, statement);
		statement.move_child_back();
		par_advance_lexer(lexer);

		if(IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_OPEN_PARENTHESIS)) {
			par_add_class_token_back(lexer, statement, CLASS_TOKEN_TYPE_FUNCTION_PARAMETER_LIST);
			statement.move_child_back();
			par_advance_lexer(lexer);
			par_enum_expression_parameter_list(lexer, statement);

			if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_CLOSE_PARENTHESIS)) {
				TRACE_EVENT("Invalid close parenthesis token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
				THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_CLOSE_PARENTHESIS);
			}
			par_advance_lexer(lexer);
			statement.move_parent();
		} else if(IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_OPEN_BRACE)) {
			par_add_class_token_back(lexer, statement, CLASS_TOKEN_TYPE_LIST_INDEX);
			statement.move_child_back();
			par_advance_lexer(lexer);
			par_enum_expression_root(lexer, statement);

			if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_CLOSE_BRACE)) {
				TRACE_EVENT("Invalid close brace token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
				THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_CLOSE_BRACE);
			}
			par_advance_lexer(lexer);
			statement.move_parent();

			if(IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_OPEN_PARENTHESIS)) {
				par_add_class_token_front(lexer, statement, CLASS_TOKEN_TYPE_FUNCTION_PARAMETER_LIST);
				statement.move_child_front();
				par_advance_lexer(lexer);
				par_enum_expression_parameter_list(lexer, statement);

				if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_CLOSE_PARENTHESIS)) {
					TRACE_EVENT("Invalid close parenthesis token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
					THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_CLOSE_PARENTHESIS);
				}
				par_advance_lexer(lexer);
				statement.move_parent();
			}
		}
		statement.move_parent();
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_identifier", TRACE_TYPE_INFORMATION);
}

void 
par_enum_identifier_list(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_identifier_list", TRACE_TYPE_INFORMATION);

	if(lexer) {
		par_add_class_token_back(lexer, statement, CLASS_TOKEN_TYPE_IDENTIFIER_LIST);
		statement.move_child_back();

		if(IS_TYPE_L(lexer, TOKEN_TYPE_IDENTIFIER)) {
			par_add_token_back(lexer, statement);
			par_advance_lexer(lexer);
		} else if(IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_OPEN_PARENTHESIS)) {
			par_advance_lexer(lexer);
			par_enum_identifier_list_helper(lexer, statement);

			if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_CLOSE_PARENTHESIS)) {
				TRACE_EVENT("Invalid close parenthesis token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
				THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_CLOSE_PARENTHESIS);
			}
			par_advance_lexer(lexer);
		} else {
			TRACE_EVENT("Invalid identifier token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_IDENTIFIER);
		}
		statement.move_parent();
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_identifier_list", TRACE_TYPE_INFORMATION);
}

void 
par_enum_identifier_list_helper(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_identifier_list_helper", TRACE_TYPE_VERBOSE);

	if(lexer) {
		
		if(!IS_TYPE_L(lexer, TOKEN_TYPE_IDENTIFIER)) {
			TRACE_EVENT("Invalid identifier token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_IDENTIFIER);
		}
		par_add_token_back(lexer, statement);
		par_advance_lexer(lexer);

		if(IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_LIST_SEPERATOR)) {
			par_advance_lexer(lexer);
			par_enum_identifier_list_helper(lexer, statement);
		}
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_identifier_list_helper", TRACE_TYPE_VERBOSE);
}

void 
par_enum_identifier_operator(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_identifier_operator", TRACE_TYPE_INFORMATION);

	if(lexer) {

		switch(lexer->get().get_type()) {
			case TOKEN_TYPE_ACCESS:
				par_add_token_back(lexer, statement);
				statement.move_child_back();
				par_advance_lexer(lexer);

				if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_STATEMENT_SEPERATOR)) {
					TRACE_EVENT("Invalid statement seperator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
					THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_STATEMENT_SEPERATOR);
				}
				par_advance_lexer(lexer);

				if(!IS_TYPE_L(lexer, TOKEN_TYPE_IDENTIFIER)) {
					TRACE_EVENT("Invalid identifier token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
					THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_IDENTIFIER);
				}
				par_add_token_back(lexer, statement);
				par_advance_lexer(lexer);
				statement.move_parent();
				break;
			case TOKEN_TYPE_OUT_LIST_OPERATOR:
				par_enum_out_list(lexer, statement, false);
				break;
			case TOKEN_TYPE_ITERATOR_OPERATOR:
				par_enum_iterator(lexer, statement, false);
				break;
			default:
				TRACE_EVENT("Invalid identifier operator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
				THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_IDENTIFIER_OPERATOR);
		}
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_identifier_operator", TRACE_TYPE_INFORMATION);
}

void 
par_enum_identifier_parameter_list(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_identifier_parameter_list", TRACE_TYPE_INFORMATION);

	if(lexer) {
		par_add_class_token_back(lexer, statement, CLASS_TOKEN_TYPE_IDENTIFIER_PARAMETER_LIST);
		statement.move_child_back();

		if(IS_TYPE_L(lexer, TOKEN_TYPE_IDENTIFIER)) {
			par_enum_identifier_list_helper(lexer, statement);
		}
		statement.move_parent();
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_identifier_parameter_list", TRACE_TYPE_INFORMATION);
}

void 
par_enum_in_list(
	lex_ptr lexer,
	tok_stmt &statement,
	bool terminate
	)
{
	TRACE_EVENT("+par_enum_in_list", TRACE_TYPE_INFORMATION);

	if(lexer) {

		if(!IS_TYPE_L(lexer, TOKEN_TYPE_IN_LIST_OPERATOR)) {
			TRACE_EVENT("Invalid in-list operator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_IN_LIST_OPERATOR);
		}
		par_add_token_back(lexer, statement);
		statement.move_child_back();
		par_advance_lexer(lexer);

		if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_STATEMENT_SEPERATOR)) {
			TRACE_EVENT("Invalid statement seperator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_STATEMENT_SEPERATOR);
		}
		par_advance_lexer(lexer);
		par_enum_identifier_list(lexer, statement);

		if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_LIST_SEPERATOR)) {
			TRACE_EVENT("Invalid list seperator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_LIST_SEPERATOR);
		}
		par_advance_lexer(lexer);
		par_enum_expression_root(lexer, statement);

		if(terminate) {
		
			if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_TERMINATOR)) {
				TRACE_EVENT("Invalid terminator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
				THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_TERMINATOR);
			}
			par_advance_lexer(lexer);
		}
		statement.move_parent();
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_in_list", TRACE_TYPE_INFORMATION);
}

void 
par_enum_io(
	lex_ptr lexer,
	tok_stmt &statement,
	bool direction_both
	)
{
	TRACE_EVENT("+par_enum_io", TRACE_TYPE_INFORMATION);

	if(lexer) {

		if(!IS_TYPE_L(lexer, TOKEN_TYPE_IO_OPERATOR)) {
			TRACE_EVENT("Invalid io operator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_IO_OPERATOR);
		}

		if(!direction_both
				&& !IS_SUBTYPE_L(lexer, IO_OPERATOR_TYPE_IN)) {
			TRACE_EVENT("Invalid input io operator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_INPUT_IO_OPERATOR);			
		}
		par_add_token_back(lexer, statement);
		statement.move_child_back();
		par_advance_lexer(lexer);

		if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_STATEMENT_SEPERATOR)) {
			TRACE_EVENT("Invalid statement seperator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_STATEMENT_SEPERATOR);
		}
		par_advance_lexer(lexer);

		switch(statement.get().get_subtype()) {
			case IO_OPERATOR_TYPE_IN:
				par_enum_stream_statement(lexer, statement);
				break;
			case IO_OPERATOR_TYPE_OUT:
				par_enum_stream_statement(lexer, statement);

				if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_LIST_SEPERATOR)) {
					TRACE_EVENT("Invalid list seperator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
					THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_LIST_SEPERATOR);
				}
				par_advance_lexer(lexer);
				par_enum_expression_root(lexer, statement);
				break;
			case IO_OPERATOR_TYPE_EXECUTE:
				par_enum_expression_root(lexer, statement);
				break;
			default:
				TRACE_EVENT("Invalid io operator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
				THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_IO_OPERATOR);
		}

		if(direction_both) {
			if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_TERMINATOR)) {
				TRACE_EVENT("Invalid terminator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
				THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_TERMINATOR);
			}
			par_advance_lexer(lexer);
		}
		statement.move_parent();
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_io", TRACE_TYPE_INFORMATION);
}

void 
par_enum_iterator(
	lex_ptr lexer,
	tok_stmt &statement,
	bool terminate
	)
{
	TRACE_EVENT("+par_enum_iterator", TRACE_TYPE_INFORMATION);

	if(lexer) {

		if(!IS_TYPE_L(lexer, TOKEN_TYPE_ITERATOR_OPERATOR)) {
			TRACE_EVENT("Invalid iterator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_ITERATOR);
		}
		par_add_token_back(lexer, statement);
		statement.move_child_back();
		par_advance_lexer(lexer);

		if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_STATEMENT_SEPERATOR)) {
			TRACE_EVENT("Invalid statement seperator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_STATEMENT_SEPERATOR);
		}
		par_advance_lexer(lexer);
		par_enum_identifier_list(lexer, statement);

		if(IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_OPEN_BRACE)) {
			par_add_class_token_back(lexer, statement, CLASS_TOKEN_TYPE_LIST_INDEX);
			statement.move_child_back();
			par_advance_lexer(lexer);
			par_enum_expression_root(lexer, statement);

			if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_CLOSE_BRACE)) {
				TRACE_EVENT("Invalid close brace token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
				THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_CLOSE_BRACE);
			}
			par_advance_lexer(lexer);
			statement.move_parent();
		}

		if(terminate) {
		
			if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_TERMINATOR)) {
				TRACE_EVENT("Invalid terminator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
				THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_TERMINATOR);
			}
			par_advance_lexer(lexer);
		}
		statement.move_parent();
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_iterator", TRACE_TYPE_INFORMATION);
}

void 
par_enum_out_list(
	lex_ptr lexer,
	tok_stmt &statement,
	bool terminate
	)
{
	TRACE_EVENT("+par_enum_out_list", TRACE_TYPE_INFORMATION);

	if(lexer) {
		
		if(!IS_TYPE_L(lexer, TOKEN_TYPE_OUT_LIST_OPERATOR)) {
			TRACE_EVENT("Invalid out-list operator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_OUT_LIST_OPERATOR);
		}
		par_add_token_back(lexer, statement);
		statement.move_child_back();
		par_advance_lexer(lexer);

		if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_STATEMENT_SEPERATOR)) {
			TRACE_EVENT("Invalid statement seperator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_STATEMENT_SEPERATOR);
		}
		par_advance_lexer(lexer);
		par_enum_identifier_list(lexer, statement);

		if(terminate) {
		
			if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_TERMINATOR)) {
				TRACE_EVENT("Invalid terminator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
				THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_TERMINATOR);
			}
			par_advance_lexer(lexer);
		}
		statement.move_parent();
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_out_list", TRACE_TYPE_INFORMATION);
}

void 
par_enum_range_statement(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_range_statement", TRACE_TYPE_INFORMATION);

	if(lexer) {
		par_add_class_token_back(lexer, statement, CLASS_TOKEN_TYPE_RANGE_STATEMENT);
		statement.move_child_back();
		par_enum_conditional_statement_list(lexer, statement, 0, 1);

		if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_LIST_SEPERATOR)) {
			TRACE_EVENT("Invalid list seperator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_LIST_SEPERATOR);
		}
		par_advance_lexer(lexer);
		par_add_class_token_back(lexer, statement, CLASS_TOKEN_TYPE_STATEMENT_LIST);
		statement.move_child_back();
		par_enum_statement_list_helper(lexer, statement);
		statement.move_parent();
		statement.move_parent();
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_range_statement", TRACE_TYPE_INFORMATION);
}

void par_enum_statement(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	if(lexer) {
		par_enum_statement(lexer, statement, true);
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}
}

void 
par_enum_statement(
	lex_ptr lexer,
	tok_stmt &statement,
	bool root
	)
{
	TRACE_EVENT("+par_enum_statement", TRACE_TYPE_INFORMATION);

	if(lexer) {
		par_add_class_token(lexer, statement, CLASS_TOKEN_TYPE_STATEMENT, root, true);

		if(!root) {
			statement.move_child_back();
		}

		if(IS_CLASS_TYPE_L(lexer, CLASS_TOKEN_TYPE_TOKEN)) {

			switch(lexer->get().get_type()) {
				case TOKEN_TYPE_ASSIGNMENT:
					par_enum_assignment(lexer, statement);
					break;
				case TOKEN_TYPE_CALL:
					par_enum_call(lexer, statement);
					break;
				case TOKEN_TYPE_CONDITIONAL:
					par_enum_conditional(lexer, statement);
					break;
				case TOKEN_TYPE_CONTROL:
					par_enum_control(lexer, statement);
					break;
				case TOKEN_TYPE_IO_OPERATOR:
					par_enum_io(lexer, statement, true);
					break;
				case TOKEN_TYPE_IN_LIST_OPERATOR:
					par_enum_in_list(lexer, statement, true);
					break;
				case TOKEN_TYPE_ITERATOR_OPERATOR:
					par_enum_iterator(lexer, statement, true);
					break;
				case TOKEN_TYPE_OUT_LIST_OPERATOR:
					par_enum_out_list(lexer, statement, true);
					break;
				case TOKEN_TYPE_USING:
					par_enum_using(lexer, statement);
					break;
				default:
					TRACE_EVENT("Invalid statement token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
					THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_STATEMENT);
			}

			if(!root) {
				statement.move_parent();
			}
		} else {
			TRACE_EVENT("Invalid token class type: " << lexer->get().get_class_type(), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->get().get_class_type(), PAR_EXT_EXC_INVAL_CLASS_TOKEN_TYPE);
		}
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	if(root) {
		statement.move_root();
	}

	TRACE_EVENT("-par_enum_statement", TRACE_TYPE_INFORMATION);
}

void 
par_enum_statement_list(
	lex_ptr lexer,
	tok_stmt &statement,
	bool else_clause
	)
{
	TRACE_EVENT("+par_enum_statement_list", TRACE_TYPE_INFORMATION);

	if(lexer) {
		par_add_class_token_back(lexer, statement, CLASS_TOKEN_TYPE_STATEMENT_LIST);
		statement.move_child_back();

		if(!else_clause) {

			if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_OPEN_CURLY_BRACE)) {
				TRACE_EVENT("Invalid open statement brace token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
				THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_OPEN_STATEMENT_BRACE);
			}
			par_advance_lexer(lexer);
		}
		par_enum_statement_list_helper(lexer, statement);

		if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_CLOSE_CURLY_BRACE)) {
			TRACE_EVENT("Invalid close statement brace token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_CLOSE_STATEMENT_BRACE);
		}
		par_advance_lexer(lexer);
		statement.move_parent();
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_statement_list", TRACE_TYPE_INFORMATION);
}

void 
par_enum_statement_list_helper(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_statement_list_helper", TRACE_TYPE_VERBOSE);

	if(lexer) {

		switch(lexer->get().get_type()) {
			case TOKEN_TYPE_ASSIGNMENT:
			case TOKEN_TYPE_CALL:
			case TOKEN_TYPE_CONDITIONAL:
			case TOKEN_TYPE_CONTROL:
			case TOKEN_TYPE_IN_LIST_OPERATOR:
			case TOKEN_TYPE_IO_OPERATOR:
			case TOKEN_TYPE_ITERATOR_OPERATOR:
			case TOKEN_TYPE_OUT_LIST_OPERATOR:
			case TOKEN_TYPE_USING:
				par_enum_statement(lexer, statement, false);
				par_enum_statement_list_helper(lexer, statement);
				break;
		}
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_statement_list_helper", TRACE_TYPE_VERBOSE);
}

void 
par_enum_stream_statement(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_stream_statement", TRACE_TYPE_INFORMATION);

	if(lexer) {
		par_add_class_token_back(lexer, statement, CLASS_TOKEN_TYPE_STREAM_STATEMENT);
		statement.move_child_back();

		if(!IS_TYPE_L(lexer, TOKEN_TYPE_STREAM)){ 
			TRACE_EVENT("Invalid stream token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_STREAM);
		}
		par_add_token_back(lexer, statement);
		statement.move_child_back();
		par_advance_lexer(lexer);

		switch(statement.get().get_subtype()) {
			case STREAM_TYPE_FILE:

				if(IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_LIST_SEPERATOR)) {
					par_advance_lexer(lexer);
					par_enum_expression_root(lexer, statement);
				} else if(IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_STATEMENT_SEPERATOR)) {
					par_advance_lexer(lexer);
					par_enum_file_stream_list(lexer, statement);

					if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_LIST_SEPERATOR)) {
						TRACE_EVENT("Invalid list seperator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
						THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_LIST_SEPERATOR);
					}
					par_advance_lexer(lexer);
					par_enum_expression_root(lexer, statement);
				} else {
					TRACE_EVENT("Invalid stream token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
					THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_STREAM);
				}
				break;
			case STREAM_TYPE_STDIN:
			case STREAM_TYPE_STDOUT:
				break;
			default:
				TRACE_EVENT("Invalid stream token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
				THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_STREAM);
		}
		statement.move_parent();
		statement.move_parent();
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_stream_statement", TRACE_TYPE_INFORMATION);
}

void 
par_enum_unary(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_unary", TRACE_TYPE_INFORMATION);

	if(lexer) {
				
		if(IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_OPEN_PARENTHESIS)) {
			par_advance_lexer(lexer);
			par_enum_expression_root(lexer, statement);

			if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_CLOSE_PARENTHESIS)) {
				TRACE_EVENT("Invalid close parenthesis token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
				THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_CLOSE_PARENTHESIS);
			}
			par_advance_lexer(lexer);
		} else if(IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_UNARY_NEGATION)) {
			par_add_token_back(lexer, statement);
			statement.move_child_back();
			par_advance_lexer(lexer);
			par_enum_expression_leaf(lexer, statement);
			statement.move_parent();
		} else {
			TRACE_EVENT("Invalid unary token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_EXPRESSION);
		}
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_unary", TRACE_TYPE_INFORMATION);
}

void 
par_enum_using(
	lex_ptr lexer,
	tok_stmt &statement
	)
{
	TRACE_EVENT("+par_enum_using", TRACE_TYPE_INFORMATION);

	if(lexer) {

		if(!IS_TYPE_L(lexer, TOKEN_TYPE_USING)) {
			TRACE_EVENT("Invalid using token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_USING);
		}
		par_add_token_back(lexer, statement);
		statement.move_child_back();
		par_advance_lexer(lexer);
		
		if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_STATEMENT_SEPERATOR)) {
			TRACE_EVENT("Invalid statement seperator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_STATEMENT_SEPERATOR);
		}
		par_advance_lexer(lexer);
		par_enum_expression_root(lexer, statement);

		if(!IS_TOKEN_SYMBOL_TYPE_L(lexer, SYMBOL_TYPE_TERMINATOR)) {
			TRACE_EVENT("Invalid terminator token: " << lexer->to_string(true), TRACE_TYPE_ERROR);
			THROW_PAR_EXT_EXC_W_MESS(lexer->to_string(true), PAR_EXT_EXC_EXPECT_TERMINATOR);
		}
		par_advance_lexer(lexer);
		statement.move_parent();
	} else {
		TRACE_EVENT("Invalid parser enumeration action parameter!", TRACE_TYPE_ERROR);
		THROW_PAR_EXT_EXC(PAR_EXT_EXC_INVAL_ENUM_ACTION_PARAM);
	}

	TRACE_EVENT("-par_enum_using", TRACE_TYPE_INFORMATION);
}