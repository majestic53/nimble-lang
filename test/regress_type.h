/*
 * regress_type.h
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

#ifndef REGRESS_TYPE_H_
#define REGRESS_TYPE_H_

/*
 * Regression types
 */
enum {
	EXE_REGRESS_TEST = 0,
	LEX_REGRESS_TEST,
	PAR_REGRESS_TEST,
};

/*
 * Regression test types
 */
enum {
	EXE_SET_TEST = 0,
};

enum {
	LEX_ACCESS_OPERATOR_TEST = 0,
	LEX_ARGUMENT_TEST,
	LEX_ASSIGNMENT_TEST,
	LEX_BINARY_OPERATOR_TEST,
	LEX_CALL_TEST,
	LEX_COMPARATOR_OPERATOR_TEST,
	LEX_CONDITIONAL_SEPERATOR_TEST,
	LEX_CONDITIONAL_TEST,
	LEX_CONSTANT_TEST,
	LEX_CONTROL_TEST,
	LEX_CONVERSION_OPERATOR_TEST,
	LEX_FILE_STREAM_OPERATOR_TEST,
	LEX_IN_LIST_OPERATOR_TEST,
	LEX_IO_OPERATOR_TEST,
	LEX_ITERATOR_OPERATOR_TEST,
	LEX_LOGICAL_OPERATOR_TEST,
	LEX_OUT_LIST_OPERATOR_TEST,
	LEX_STREAM_OPERATOR_TEST,
	LEX_USING_OPERATOR_TEST,
};

enum {
	PAR_ACCESS_TEST = 0,
	PAR_ARGUMENT_TEST,
	PAR_BINARY_OPERATOR_TEST,
	PAR_CALL_TEST,
	PAR_COMPARATOR_STATEMENT_TEST,
	PAR_COMPOUND_ASSIGNMENT_TEST,
	PAR_CONDITIONAL_STATEMENT_TEST,
	PAR_CONSTANT_TEST,
	PAR_CONTROL_TEST,
	PAR_CONVERSION_TEST,
	PAR_EXPRESSION_TEST,
	PAR_FUNCTION_ASSIGNMENT_TEST,
	PAR_IN_LIST_OPERATOR_TEST,
	PAR_IO_OPERATOR_TEST,
	PAR_ITERATOR_OPERATOR_TEST,
	PAR_LOGICAL_OPERATOR_TEST,
	PAR_OUT_LIST_OPERATOR_TEST,
	PAR_SIMPLE_ASSIGNMENT_TEST,
	PAR_USING_STATEMENT_TEST,
};

/*
 * Max regression test types
 */
#define MAX_REGRESS_TEST PAR_REGRESS_TEST
#define MAX_EXE_REGRESS_TEST EXE_SET_TEST 
#define MAX_LEX_REGRESS_TEST LEX_USING_OPERATOR_TEST
#define MAX_PAR_REGRESS_TEST PAR_USING_STATEMENT_TEST

/*
 * Regression test strings
 */
static const std::string EXE_REGRESS_TEST_STR[] = {
	"EXE_SET_TEST",
};

static const std::string LEX_REGRESS_TEST_STR[] = {
	"LEX_ACCESS_OPERATOR_TEST",
	"LEX_ARGUMENT_TEST",
	"LEX_ASSIGNMENT_TEST",
	"LEX_BINARY_OPERATOR_TEST",
	"LEX_CALL_TEST",
	"LEX_COMPARATOR_OPERATOR_TEST",
	"LEX_CONDITIONAL_SEPERATOR_TEST",
	"LEX_CONDITIONAL_TEST",
	"LEX_CONSTANT_TEST",
	"LEX_CONTROL_TEST",
	"LEX_CONVERSION_OPERATOR_TEST",
	"LEX_FILE_STREAM_OPERATOR_TEST",
	"LEX_IN_LIST_OPERATOR_TEST",
	"LEX_IO_OPERATOR_TEST",
	"LEX_ITERATOR_OPERATOR_TEST",
	"LEX_LOGICAL_OPERATOR_TEST",
	"LEX_OUT_LIST_OPERATOR_TEST",
	"LEX_STREAM_OPERATOR_TEST",
	"LEX_USING_OPERATOR_TEST",
};

static const std::string PAR_REGRESS_TEST_STR[] = {
	"PAR_ACCESS_TEST",
	"PAR_ARGUMENT_TEST",
	"PAR_BINARY_OPERATOR_TEST",
	"PAR_CALL_TEST",
	"PAR_COMPARATOR_STATEMENT_TEST",
	"PAR_COMPOUND_ASSIGNMENT_TEST",
	"PAR_CONDITIONAL_STATEMENT_TEST",
	"PAR_CONSTANT_TEST",
	"PAR_CONTROL_TEST",
	"PAR_CONVERSION_TEST",
	"PAR_EXPRESSION_TEST",
	"PAR_FUNCTION_ASSIGNMENT_TEST",
	"PAR_IN_LIST_OPERATOR_TEST",
	"PAR_IO_OPERATOR_TEST",
	"PAR_ITERATOR_OPERATOR_TEST",
	"PAR_LOGICAL_OPERATOR_TEST",
	"PAR_OUT_LIST_OPERATOR_TEST",
	"PAR_SIMPLE_ASSIGNMENT_TEST",
	"PAR_USING_STATEMENT_TEST",
};

static const std::string EXE_REGRESS_TEST_PATH[] = {
	"../../../test/exe_regress_test/exe_regress_set_test.nb",
};

static const std::string LEX_REGRESS_TEST_PATH[] = {
	"../../../test/lex_regress_test/lex_regress_access_operator_test.nb",
	"../../../test/lex_regress_test/lex_regress_argument_test.nb",
	"../../../test/lex_regress_test/lex_regress_assignment_test.nb",
	"../../../test/lex_regress_test/lex_regress_binary_operator_test.nb",
	"../../../test/lex_regress_test/lex_regress_call_test.nb",
	"../../../test/lex_regress_test/lex_regress_comparator_operator_test.nb",
	"../../../test/lex_regress_test/lex_regress_conditional_seperator_test.nb",
	"../../../test/lex_regress_test/lex_regress_conditional_test.nb",
	"../../../test/lex_regress_test/lex_regress_constant_test.nb",
	"../../../test/lex_regress_test/lex_regress_control_test.nb",
	"../../../test/lex_regress_test/lex_regress_conversion_operator_test.nb",
	"../../../test/lex_regress_test/lex_regress_file_stream_operator_test.nb",
	"../../../test/lex_regress_test/lex_regress_in_list_operator_test.nb",
	"../../../test/lex_regress_test/lex_regress_io_operator_test.nb",
	"../../../test/lex_regress_test/lex_regress_iterator_operator_test.nb",
	"../../../test/lex_regress_test/lex_regress_logical_operator_test.nb",
	"../../../test/lex_regress_test/lex_regress_out_list_operator_test.nb",
	"../../../test/lex_regress_test/lex_regress_stream_operator_test.nb",
	"../../../test/lex_regress_test/lex_regress_using_operator_test.nb",
};

static const std::string PAR_REGRESS_TEST_PATH[] = {
	"../../../test/par_regress_test/par_regress_access_test.nb",
	"../../../test/par_regress_test/par_regress_argument_test.nb",
	"../../../test/par_regress_test/par_regress_binary_operator_test.nb",
	"../../../test/par_regress_test/par_regress_call_test.nb",
	"../../../test/par_regress_test/par_regress_comparator_statement_test.nb",
	"../../../test/par_regress_test/par_regress_compound_assignment_test.nb",
	"../../../test/par_regress_test/par_regress_conditional_statement_test.nb",
	"../../../test/par_regress_test/par_regress_constant_test.nb",
	"../../../test/par_regress_test/par_regress_control_test.nb",
	"../../../test/par_regress_test/par_regress_conversion_test.nb",
	"../../../test/par_regress_test/par_regress_expression_test.nb",
	"../../../test/par_regress_test/par_regress_function_assignment_test.nb",
	"../../../test/par_regress_test/par_regress_in_list_operator_test.nb",
	"../../../test/par_regress_test/par_regress_io_operator_test.nb",
	"../../../test/par_regress_test/par_regress_iterator_operator_test.nb",
	"../../../test/par_regress_test/par_regress_logical_operator_test.nb",
	"../../../test/par_regress_test/par_regress_out_list_operator_test.nb",
	"../../../test/par_regress_test/par_regress_simple_assignment_test.nb",
	"../../../test/par_regress_test/par_regress_using_statement_test.nb",
};

/*
 * Regression test string lookup macros
 */
#define EXE_TEST_STRING(_T_) (_T_ > MAX_EXE_REGRESS_TEST ? "Unknown executor regression test" : EXE_REGRESS_TEST_STR[_T_])
#define EXE_TEST_PATH_STRING(_T_) (_T_ > MAX_EXE_REGRESS_TEST ? "Unknown executor regression test path" : EXE_REGRESS_TEST_PATH[_T_])
#define LEX_TEST_STRING(_T_) (_T_ > MAX_LEX_REGRESS_TEST ? "Unknown lexer regression test" : LEX_REGRESS_TEST_STR[_T_])
#define LEX_TEST_PATH_STRING(_T_) (_T_ > MAX_LEX_REGRESS_TEST ? "Unknown lexer regression test path" : LEX_REGRESS_TEST_PATH[_T_])
#define PAR_TEST_STRING(_T_) (_T_ > MAX_PAR_REGRESS_TEST ? "Unknown parser regression test" : PAR_REGRESS_TEST_STR[_T_])
#define PAR_TEST_PATH_STRING(_T_) (_T_ > MAX_PAR_REGRESS_TEST ? "Unknown parser regression test path" : PAR_REGRESS_TEST_PATH[_T_])

/*
 * Regression test helper macros
 */
#define REG_TEST_LENGTH(_T_)\
	(_T_ == EXE_REGRESS_TEST ? (MAX_EXE_REGRESS_TEST + 1) :\
	(_T_ == LEX_REGRESS_TEST ? (MAX_LEX_REGRESS_TEST + 1) :\
	(_T_ == PAR_REGRESS_TEST ? (MAX_PAR_REGRESS_TEST + 1) : 0)))

#define REG_TEST_STRING(_T_, _TY_)\
	(_T_ == EXE_REGRESS_TEST ? EXE_TEST_STRING(_TY_) :\
	(_T_ == LEX_REGRESS_TEST ? LEX_TEST_STRING(_TY_) :\
	(_T_ == PAR_REGRESS_TEST ? PAR_TEST_STRING(_TY_) : "Unknown regression test")))

#define REG_TEST_PATH(_T_, _TY_)\
	(_T_ == EXE_REGRESS_TEST ? EXE_TEST_PATH_STRING(_TY_) :\
	(_T_ == LEX_REGRESS_TEST ? LEX_TEST_PATH_STRING(_TY_) :\
	(_T_ == PAR_REGRESS_TEST ? PAR_TEST_PATH_STRING(_TY_) : "Unknown regression test")))

#endif