/*
 * sanity.cpp
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
#include <stdexcept>
#include "regress.h"
#include "regress_type.h"
#include "sanity.h"
#include "..\src\nblang\exe.h"
#include "..\src\nblang\exe_ext.h"
#include "..\src\nblang\lang.h"
#include "..\src\nblang\lex.h"
#include "..\src\nblang\lex_ext.h"
#include "..\src\nblang\list.h"
#include "..\src\nblang\par.h"
#include "..\src\nblang\par_ext.h"

void
exe_test(
	const std::string &input,
	const tok_vector &arguments,
	bool is_file
	)
{
	exe exec;

	exec.set_action(exe_config_parser, EXE_ACTION_CONFIG_PARSER);
	exec.set_evaluation_action(exe_eval_statement, EXE_EVAL_ACTION_STATEMENT);
	exec.initialize(input, arguments, is_file, true);
	exec.evaluate();
}

void
lex_base_test(
	const std::string &input,
	bool is_file
	)
{
	lex_base base;

	base.initialize(input, is_file);

	while(base.has_next()) {
		std::cout << base.to_string(true) << std::endl;
		base.move_next();
	}
	std::cout << "COUNT: " << base.get_count() << std::endl;
}

void 
lex_test(
	const std::string &input,
	bool is_file
	) 
{
	lex lx;

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
	std::cout << "COUNT: " << lx.get_count() << std::endl;
}

void
par_test(
	const std::string &input,
	bool is_file
	)
{
	par pr;

	pr.set_action(par_config_lexer, PAR_ACTION_CONFIG_LEXER);
	pr.set_enumeration_action(par_enum_statement, PAR_ENUM_ACTION_STATEMENT);
	pr.initialize(input, is_file);
	pr.enumerate();

	while(pr.has_next()) {
		std::cout << pr.to_string(true) << std::endl;
		pr.move_next();
	}
	std::cout << pr.to_string(true) << std::endl;
	std::cout << "COUNT: " << pr.get_count() << std::endl;
}