/*
 * main.cpp
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
#include <vector>
#include "arg_type.h"
#include "ppt.h"
#include "ppt_type.h"
#include "ppt_ext.h"
#include "ppt_ext_type.h"

/*
 * Register prompt command
 * @param prompt prompt object reference
 * @param schema command schema object reference
 * @param command command string
 * @param arguments command arguments
 * @param action command action callback
 * @param wildcard command wildcard status
 */
void
register_command(
	ppt &prompt,
	sch &schema,
	const std::string &command,
	const std::vector<arg> &arguments,
	ppt_action action,
	bool wildcard
	)
{
	std::vector<arg>::const_iterator arg_iter = arguments.begin();

	schema.set_action(action);
	schema.set_as_wildcard(wildcard);
	schema.set_command(command);

	for(; arg_iter != arguments.end(); ++arg_iter) {
		schema.add_argument_back(*arg_iter);
	}
	prompt.register_schema(schema);
}

/*
 * Register prompt command
 * @param prompt prompt object reference
 * @param schema command schema object reference
 * @param command command string
 * @param action command action callback
 * @param wildcard command wildcard status
 */
void
register_command(
	ppt &prompt,
	sch &schema,
	const std::string &command,
	ppt_action action,
	bool wildcard
	)
{
	register_command(prompt, schema, command, std::vector<arg>(), action, wildcard);
}

/*
 * Main
 */
int 
main(
	int argc,
	char **argv
	) 
{
	ppt prompt;
	int result = 0;
	std::vector<arg> load_arguments;
	std::vector<std::string> arguments;
	sch cmd_schema[MAX_PPT_CMD_TYPE + 1];

	if(argc > MIN_PROMPT_INPUT) {
		arguments.insert(arguments.begin(), argv + 1, argv + argc);
	}

	try {
		load_arguments.push_back(arg(ARG_TYPE_STRING));
		register_command(prompt, cmd_schema[PPT_CMD_COPYRIGHT], PPT_CMD_STRING(PPT_CMD_COPYRIGHT), ppt_ext_hdl_copyr_command, false);
		register_command(prompt, cmd_schema[PPT_CMD_EXIT], PPT_CMD_STRING(PPT_CMD_EXIT), ppt_ext_hdl_exit_command, false);
		register_command(prompt, cmd_schema[PPT_CMD_HELP], PPT_CMD_STRING(PPT_CMD_HELP), ppt_ext_hdl_help_command, false);
		register_command(prompt, cmd_schema[PPT_CMD_LICENSE], PPT_CMD_STRING(PPT_CMD_LICENSE), ppt_ext_hdl_lic_command, false);
		register_command(prompt, cmd_schema[PPT_CMD_LOAD], PPT_CMD_STRING(PPT_CMD_LOAD), load_arguments, ppt_ext_hdl_load_command, true);
		register_command(prompt, cmd_schema[PPT_CMD_RESET], PPT_CMD_STRING(PPT_CMD_RESET), ppt_ext_hdl_res_command, false);
		register_command(prompt, cmd_schema[PPT_CMD_VERSION], PPT_CMD_STRING(PPT_CMD_VERSION), ppt_ext_hdl_ver_command, false);
		prompt.set_argument_action(ppt_ext_hdl_arguments);
		prompt.set_exception_action(ppt_ext_hdl_exceptions);
		prompt.set_generic_action(ppt_ext_hdl_gen_command);
		prompt.start(arguments, ppt_ext_init_exe);
	} catch(std::runtime_error &exc) {
		std::cerr << "Exception: " << exc.what() << std::endl << std::endl;
		result = 1;
	}

	return result;
}