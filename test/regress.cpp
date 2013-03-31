/*
 * regress.cpp
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

#include <iostream>
#include <map>
#include <stdexcept>
#include "..\src\nblang\exe.h"
#include "..\src\nblang\exe_ext.h"
#include "..\src\nblang\lang.h"
#include "..\src\nblang\lex.h"
#include "..\src\nblang\lex_ext.h"
#include "..\src\nblang\par.h"
#include "..\src\nblang\par_ext.h"
#include "regress.h"

bool
exe_regress_test_helper(
	const std::string &name,
	const std::string &input,
	size_t test_id,
	bool is_file
	)
{
	exe exec;
	bool result = true;
	tok_vector arguments;
	
	std::cout << "[" << test_id << "] " << name << std::endl << "------------------------" << std::endl;
	
	try {
		exec.set_action(exe_config_parser, EXE_ACTION_CONFIG_PARSER);
		exec.set_evaluation_action(exe_eval_statement, EXE_EVAL_ACTION_STATEMENT);
		exec.initialize(input, arguments, is_file, true);

		while(exec.has_next()) {
			exec.step();
		}
	} catch(std::runtime_error &exc) {
		std::cerr << "------------------------" << std::endl << "Exception: " << exc.what() << std::endl;
		result = false;
	}
	std::cout << "------------------------" << std::endl << "RESULT: " << (result ? "PASS" : "FAIL") << std::endl << std::endl;

	return result;
}

bool
lex_regress_test_helper(
	const std::string &name,
	const std::string &input,
	size_t test_id,
	bool is_file
	)
{
	lex lx;
	bool result = true;
	
	std::cout << "[" << test_id << "] " << name << std::endl << "------------------------" << std::endl;
	
	try {
		lx.set_action(lex_skip_whitespace, LEX_ACTION_SKIP_WHITESPACE);
		lx.set_enumeration_action(lex_enum_alpha, LEX_ENUM_ACTION_ALPHA);
		lx.set_enumeration_action(lex_enum_number, LEX_ENUM_ACTION_NUMBER);
		lx.set_enumeration_action(lex_enum_symbol, LEX_ENUM_ACTION_SYMBOL);	
		lx.initialize(input, is_file);
		lx.enumerate();

		while(lx.has_next()) {
			std::cout << lx.to_string(true) << std::endl;
			lx.move_next();
		}
		std::cout << lx.to_string(true) << std::endl;
		std::cout << "------------------------" << std::endl << "COUNT: " << lx.get_count() << std::endl;
	} catch(std::runtime_error &exc) {
		std::cerr << "------------------------" << std::endl << "Exception: " << exc.what() << std::endl;
		result = false;
	}
	std::cout << "------------------------" << std::endl << "RESULT: " << (result ? "PASS" : "FAIL") << std::endl << std::endl << std::endl;

	return result;
}

bool
par_regress_test_helper(
	const std::string &name,
	const std::string &input,
	size_t test_id,
	bool is_file
	)
{
	par pr;
	bool result = true;
	
	std::cout << "[" << test_id << "] " << name << std::endl << "------------------------" << std::endl;
	
	try {
		pr.set_action(par_config_lexer, PAR_ACTION_CONFIG_LEXER);
		pr.set_enumeration_action(par_enum_statement, PAR_ENUM_ACTION_STATEMENT);
		pr.initialize(input, is_file);
		pr.enumerate();

		while(pr.has_next()) {
			std::cout << pr.to_string(true) << std::endl;
			pr.move_next();
		}
		std::cout << pr.to_string(true) << std::endl;
		std::cout << "------------------------" << std::endl << "COUNT: " << pr.get_count() << std::endl;
	} catch(std::runtime_error &exc) {
		std::cerr << "------------------------" << std::endl << "Exception: " << exc.what() << std::endl;
		result = false;
	}
	std::cout << "------------------------" << std::endl << "RESULT: " << (result ? "PASS" : "FAIL") << std::endl << std::endl << std::endl;

	return result;
}

bool 
regress_test(
	size_t test_type,
	size_t test_id
	)
{
	bool result = true;
	int pass = 0, total = 1;
	std::map<size_t, std::string> fail_case;
	std::map<size_t, std::string>::iterator entry;

	switch(test_type) {
		case EXE_REGRESS_TEST:
			result = exe_regress_test_helper(REG_TEST_STRING(test_type, test_id), REG_TEST_PATH(test_type, test_id), test_id, true);
			break;
		case LEX_REGRESS_TEST:
			result = lex_regress_test_helper(REG_TEST_STRING(test_type, test_id), REG_TEST_PATH(test_type, test_id), test_id, true);
			break;
		case PAR_REGRESS_TEST:
			result = par_regress_test_helper(REG_TEST_STRING(test_type, test_id), REG_TEST_PATH(test_type, test_id), test_id, true);
			break;
		default:
			std::cerr << "Unknown regression test type" << std::endl;
			break;
	}

	if(!result) {
		fail_case.insert(std::pair<size_t, std::string>(test_id, REG_TEST_STRING(test_type, test_id)));
	} else {
		++pass;
	}
	std::cout << "DONE. PASS RATE: " << (pass / (float) total) * 100.f << "% (" << pass << "/" << total << ")" << std::endl;

	if(pass != total) {
		std::cout << "------------------------" << std::endl << "ID\t| NAME" << std::endl << "------------------------" << std::endl;
		
		for(entry = fail_case.begin(); entry != fail_case.end(); ++entry) {
			std::cout << entry->first << "\t| " << entry->second << std::endl;
		}
		std::cout << std::endl << "------------------------" << std::endl;
	}

	return (pass == total);
}

bool 
regress_test_all(
	size_t test_type
	)
{
	bool result = true;
	std::map<size_t, std::string> fail_case;
	std::map<size_t, std::string>::iterator entry;
	int i = 0, pass = 0, total = REG_TEST_LENGTH(test_type);

	for(i = 0; i < total; ++i) {

		switch(test_type) {
			case EXE_REGRESS_TEST:
				result = exe_regress_test_helper(REG_TEST_STRING(test_type, i), REG_TEST_PATH(test_type, i), i, true);
				break;
			case LEX_REGRESS_TEST:
				result = lex_regress_test_helper(REG_TEST_STRING(test_type, i), REG_TEST_PATH(test_type, i), i, true);
				break;
			case PAR_REGRESS_TEST:
				result = par_regress_test_helper(REG_TEST_STRING(test_type, i), REG_TEST_PATH(test_type, i), i, true);
				break;
			default:
				std::cerr << "Unknown regression test type" << std::endl;
				break;
		}

		if(!result) {
			fail_case.insert(std::pair<size_t, std::string>(i, REG_TEST_STRING(test_type, i)));
			continue;	
		}
		++pass;
	}
	std::cout << "DONE. PASS RATE: " << (pass / (float) total) * 100.f << "% (" << pass << "/" << total << ")" << std::endl;

	if(pass != total) {
		std::cout << "------------------------" << std::endl << "ID\t| NAME" << std::endl << "------------------------" << std::endl;
		
		for(entry = fail_case.begin(); entry != fail_case.end(); ++entry) {
			std::cout << entry->first << "\t| " << entry->second << std::endl;
		}
		std::cout << std::endl << "------------------------" << std::endl;
	}

	return (pass == total);
}

bool 
exe_regress_test(
	size_t test
	)
{
	return regress_test(EXE_REGRESS_TEST, test);
}

bool 
exe_regress_test_all(void)
{
	return regress_test_all(EXE_REGRESS_TEST);
}

bool 
lex_regress_test(
	size_t test
	)
{
	return regress_test(LEX_REGRESS_TEST, test);
}

bool
lex_regress_test_all(void)
{
	return regress_test_all(LEX_REGRESS_TEST);
}

bool 
par_regress_test(
	size_t test
	)
{
	return regress_test(PAR_REGRESS_TEST, test);
}

bool
par_regress_test_all(void)
{
	return regress_test_all(PAR_REGRESS_TEST);
}