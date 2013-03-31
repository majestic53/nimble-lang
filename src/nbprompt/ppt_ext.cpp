/*
 * ppt_ext.cpp
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
#include <sstream>
#include <stdexcept>
#include <string>
#include "arg_type.h"
#include "in_arg.h"
#include "ppt.h"
#include "ppt_ext.h"
#include "ppt_ext_type.h"
#include "..\nblang\par_ext.h"

//#define PROMPT_EXT_TRACING
#ifndef PROMPT_EXT_TRACING
#define TRACE_EVENT(_M_, _T_)
#else
#define TRACE_HEADER "prompt_ext"
#include "..\nblang\trace.h"
#endif

/*
 * Prompt extension executor instance
 */
static exe ppt_exe;

in_arg 
ppt_ext_hdl_arguments(
	ppt_ptr prompt,
	std::vector<std::string> &arguments
	)
{
	TRACE_EVENT("+ppt_ext_hdl_arguments", TRACE_TYPE_INFORMATION);

	size_t type;
	in_arg result;
	std::string output, input;
	bool done = false, flag[MAX_PPT_ARG_TYPE + 1];
	std::vector<std::string>::const_iterator arg_iter = arguments.begin();

	if(prompt) {
		memset(flag, false, sizeof(bool) * (MAX_PPT_ARG_TYPE + 1));

		if(arguments.empty()) {
			result.set_signal(PPT_ARG_SIG_INTERACT);
		} else {

			for(; arg_iter != arguments.end(); ++arg_iter) {
				input = *arg_iter;

				if(IS_ARGUMENT_0(input)) {
					input = input.substr(ARG_FLAG_0_LEN);
				} else if(IS_ARGUMENT_1(input)) {
					input = input.substr(ARG_FLAG_1_LEN);
				} else {
					THROW_PPT_EXT_EXC_W_MESS(input, PPT_EXT_EXC_INVAL_ARG);
				}
				type = ppt_find_argument(input);

				switch(type) {
					case PPT_ARG_FILE_IN:
					case PPT_ARG_IN:
						++arg_iter;

						if(arg_iter == arguments.end()) {
							THROW_PPT_EXT_EXC_W_MESS(ARG_FLAG_0 << PPT_ARG_STRING_0(type) << " (" << ARG_FLAG_1 << PPT_ARG_STRING_1(type) << ")", PPT_EXT_EXC_MISSING_INPUT);
						}
						result.set_as_file(type == PPT_ARG_FILE_IN);
						result.set_input(*arg_iter);
						result.set_signal(PPT_ARG_SIG_INPUT);
						++arg_iter;

						while(arg_iter != arguments.end()) {
							input = *arg_iter;

							if(arg_iter == arguments.end()
									|| IS_ARGUMENT(input)) {
								break;
							} else {
								result.add_argument_back(arg(*arg_iter, ARG_TYPE_STRING));
							}
							++arg_iter;
						}
						done = true;
						break;
					case PPT_ARG_HELP:
						std::cout << ppt_prompt_arg_help() << std::endl;
						done = true;
						break;
					case PPT_ARG_INTERACT:
						result.set_signal(PPT_ARG_SIG_INTERACT);
						done = true;
						break;
					case PPT_ARG_VERSION:
						PROMPT_VERSION(output);
						std::cout << output << std::endl;
						done = true;
						break;
					default:
						THROW_PPT_EXT_EXC_W_MESS(type, PPT_EXT_EXC_INVAL_ARG_TYPE);
				}

				if(done) {
					break;
				}
			}
		}
	} else {
		TRACE_EVENT("Invalid prompt parameter!", TRACE_TYPE_ERROR);
		THROW_PPT_EXT_EXC(PPT_EXT_EXC_INVAL_PARAM);
	}

	TRACE_EVENT("-ppt_ext_hdl_arguments", TRACE_TYPE_INFORMATION);

	return result;
}

void 
ppt_ext_hdl_copyr_command(
	ppt_ptr prompt,
	std::vector<arg> &arguments
	)
{
	TRACE_EVENT("+ppt_ext_hdl_copyr_command", TRACE_TYPE_INFORMATION);

	std::string copyright;

	UNREF_PARAM(arguments);

	if(prompt) {
		PROMPT_COPYRIGHT(copyright);
		std::cout << copyright << std::endl << std::endl;
	} else {
		TRACE_EVENT("Invalid prompt parameter!", TRACE_TYPE_ERROR);
		THROW_PPT_EXT_EXC(PPT_EXT_EXC_INVAL_PARAM);
	}

	TRACE_EVENT("-ppt_ext_hdl_copyr_command", TRACE_TYPE_INFORMATION);
}

void 
ppt_ext_hdl_exceptions(
	ppt_ptr prompt,
	std::runtime_error &exc
	)
{
	TRACE_EVENT("+ppt_ext_hdl_exceptions", TRACE_TYPE_INFORMATION);

	UNREF_PARAM(exc);

	if(prompt) {
		ppt_exe.initialize(std::string(), tok_vector(), false, false);
	} else {
		TRACE_EVENT("Invalid prompt parameter!", TRACE_TYPE_ERROR);
		THROW_PPT_EXT_EXC(PPT_EXT_EXC_INVAL_PARAM);
	}

	TRACE_EVENT("-ppt_ext_hdl_exceptions", TRACE_TYPE_INFORMATION);
}

void 
ppt_ext_hdl_exit_command(
	ppt_ptr prompt,
	std::vector<arg> &arguments
	)
{
	TRACE_EVENT("+ppt_ext_hdl_exit_command", TRACE_TYPE_INFORMATION);

	UNREF_PARAM(arguments);

	if(prompt) {
		prompt->stop();
	} else {
		TRACE_EVENT("Invalid prompt parameter!", TRACE_TYPE_ERROR);
		THROW_PPT_EXT_EXC(PPT_EXT_EXC_INVAL_PARAM);
	}

	TRACE_EVENT("-ppt_ext_hdl_exit_command", TRACE_TYPE_INFORMATION);
}

void 
ppt_ext_hdl_gen_command(
	ppt_ptr prompt,
	std::vector<arg> &arguments
	)
{
	TRACE_EVENT("+ppt_ext_hdl_gen_command", TRACE_TYPE_INFORMATION);

	if(prompt) {
		ppt_exe.initialize(arguments.front().get_text(), false, false);
		ppt_exe.evaluate();
	} else {
		TRACE_EVENT("Invalid prompt parameter!", TRACE_TYPE_ERROR);
		THROW_PPT_EXT_EXC(PPT_EXT_EXC_INVAL_PARAM);
	}

	TRACE_EVENT("-ppt_ext_hdl_gen_command", TRACE_TYPE_INFORMATION);
}

void 
ppt_ext_hdl_help_command(
	ppt_ptr prompt,
	std::vector<arg> &arguments
	)
{
	TRACE_EVENT("+ppt_ext_hdl_help_command", TRACE_TYPE_INFORMATION);

	std::string help;

	UNREF_PARAM(arguments);

	if(prompt) {
		PROMPT_HELP(help);
		std::cout << help << std::endl << std::endl;
	} else {
		TRACE_EVENT("Invalid prompt parameter!", TRACE_TYPE_ERROR);
		THROW_PPT_EXT_EXC(PPT_EXT_EXC_INVAL_PARAM);
	}

	TRACE_EVENT("-ppt_ext_hdl_help_command", TRACE_TYPE_INFORMATION);
}

void 
ppt_ext_hdl_lic_command(
	ppt_ptr prompt,
	std::vector<arg> &arguments
	)
{
	TRACE_EVENT("+ppt_ext_hdl_lic_command", TRACE_TYPE_INFORMATION);

	std::string license;

	UNREF_PARAM(arguments);

	if(prompt) {
		PROMPT_LICENSE(license);
		std::cout << license << std::endl << std::endl;
	} else {
		TRACE_EVENT("Invalid prompt parameter!", TRACE_TYPE_ERROR);
		THROW_PPT_EXT_EXC(PPT_EXT_EXC_INVAL_PARAM);
	}

	TRACE_EVENT("-ppt_ext_hdl_lic_command", TRACE_TYPE_INFORMATION);
}

void 
ppt_ext_hdl_load_command(
	ppt_ptr prompt,
	std::vector<arg> &arguments
	)
{
	TRACE_EVENT("+ppt_ext_hdl_load_command", TRACE_TYPE_INFORMATION);

	tok token;
	size_t i = 1;
	tok_vector tok_vec;
	std::string path = arguments.front().get_text();
	std::vector<arg> file_args(arguments.begin() + 1, arguments.end());

	if(prompt) {

		for(; i < arguments.size(); ++i) {
			token.set_as_token(TOKEN_TYPE_VAR_STRING, INVALID_TYPE);
			token.get_text() = arguments.at(i).get_text();
			tok_vec.push_back(token);
		}
		ppt_exe.initialize(path, tok_vec, true, false);
		ppt_exe.evaluate();
	} else {
		TRACE_EVENT("Invalid prompt parameter!", TRACE_TYPE_ERROR);
		THROW_PPT_EXT_EXC(PPT_EXT_EXC_INVAL_PARAM);
	}

	TRACE_EVENT("-ppt_ext_hdl_load_command", TRACE_TYPE_INFORMATION);
}

void 
ppt_ext_hdl_res_command(
	ppt_ptr prompt,
	std::vector<arg> &arguments
	)
{
	TRACE_EVENT("+ppt_ext_hdl_res_command", TRACE_TYPE_INFORMATION);

	UNREF_PARAM(arguments);

	if(prompt) {
		ppt_exe.reset(true);
	} else {
		TRACE_EVENT("Invalid prompt parameter!", TRACE_TYPE_ERROR);
		THROW_PPT_EXT_EXC(PPT_EXT_EXC_INVAL_PARAM);
	}

	TRACE_EVENT("-ppt_ext_hdl_res_command", TRACE_TYPE_INFORMATION);
}

void 
ppt_ext_hdl_ver_command(
	ppt_ptr prompt,
	std::vector<arg> &arguments
	)
{
	TRACE_EVENT("+ppt_ext_hdl_ver_command", TRACE_TYPE_INFORMATION);

	std::string version;

	UNREF_PARAM(arguments);

	if(prompt) {
		PROMPT_VERSION(version);
		std::cout << version << std::endl << std::endl;
	} else {
		TRACE_EVENT("Invalid prompt parameter!", TRACE_TYPE_ERROR);
		THROW_PPT_EXT_EXC(PPT_EXT_EXC_INVAL_PARAM);
	}

	TRACE_EVENT("-ppt_ext_hdl_ver_command", TRACE_TYPE_INFORMATION);
}

void 
ppt_ext_init_exe(
	ppt_ptr prompt
	)
{
	TRACE_EVENT("+ppt_ext_init_exe", TRACE_TYPE_INFORMATION);

	if(prompt) {
		ppt_exe.set_action(exe_config_parser, EXE_ACTION_CONFIG_PARSER);
		ppt_exe.set_evaluation_action(exe_eval_statement, EXE_EVAL_ACTION_STATEMENT);
		ppt_exe.initialize(std::string(), tok_vector(), false, true);
	} else {
		TRACE_EVENT("Invalid prompt parameter!", TRACE_TYPE_ERROR);
		THROW_PPT_EXT_EXC(PPT_EXT_EXC_INVAL_PARAM);
	}

	TRACE_EVENT("-ppt_ext_init_exe", TRACE_TYPE_INFORMATION);
}