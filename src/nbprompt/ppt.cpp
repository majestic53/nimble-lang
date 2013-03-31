/*
 * ppt.cpp
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

#include <csignal>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "in_arg.h"
#include "ppt.h"

//#define PROMPT_TRACING
#ifndef PROMPT_TRACING
#define TRACE_EVENT(_M_, _T_)
#else
#define TRACE_HEADER "prompt"
#include "..\nblang\trace.h"
#endif

_ppt::_ppt(void)
{
	TRACE_EVENT("+_ppt::_ppt", TRACE_TYPE_INFORMATION);

	clear();

	TRACE_EVENT("-_ppt::_ppt", TRACE_TYPE_INFORMATION);
}

_ppt::_ppt(
	const _ppt &other
	) :
		_active(other._active),
		_argument_action(other._argument_action),
		_generic_action(other._generic_action),
		_exception_action(other._exception_action),
		_schema(other._schema)
{
	TRACE_EVENT("+_ppt::_ppt", TRACE_TYPE_INFORMATION);
	TRACE_EVENT("-_ppt::_ppt", TRACE_TYPE_INFORMATION);
}

_ppt::~_ppt(void)
{
	TRACE_EVENT("+~_ppt::_ppt", TRACE_TYPE_INFORMATION);
	TRACE_EVENT("-~_ppt::_ppt", TRACE_TYPE_INFORMATION);
}

_ppt &
_ppt::operator=(
	const _ppt &other
	)
{
	TRACE_EVENT("+_ppt::operator=", TRACE_TYPE_INFORMATION);

	if(this != &other) {
		_active = other._active;
		_argument_action = other._argument_action;
		_generic_action = other._generic_action;
		_exception_action = other._exception_action;
		_schema = other._schema;
	}

	TRACE_EVENT("-_ppt::operator=", TRACE_TYPE_INFORMATION);

	return *this;
}

std::vector<_sch>::iterator
_ppt::_get_registered_schema(
	const _sch &schema
	)
{
	TRACE_EVENT("+_ppt::_get_registered_schema", TRACE_TYPE_VERBOSE);

	std::vector<_sch>::iterator schema_iter = _schema.begin();

	for(; schema_iter != _schema.end(); ++schema_iter) {

		if(*schema_iter == schema) {
			break;
		}
	}

	TRACE_EVENT("-_ppt::_get_registered_schema", TRACE_TYPE_VERBOSE);

	return schema_iter;
}

bool 
_ppt::_has_registered_schema(
	const sch &schema
	)
{
	TRACE_EVENT("+_ppt::_has_registered_schema", TRACE_TYPE_VERBOSE);

	bool result = _get_registered_schema(schema) != _schema.end();

	TRACE_EVENT("-_ppt::_has_registered_schema", TRACE_TYPE_VERBOSE);

	return result; 
}

void
_ppt::_invoke_executor(
	in_arg &arguments
	)
{
	size_t i = 0;
	tok_vector tok_args;

	for(; i < arguments.get_argument_count(); ++i) {
		tok_args.push_back(arguments.get_argument(i).to_token());
	}
	_invoke_executor(arguments.get_input(), tok_args, arguments.is_file());
}

void
_ppt::_invoke_executor(
	const std::string &input,
	const tok_vector &arguments,
	bool is_file
	)
{
	TRACE_EVENT("+_ppt::_invoke_executor", TRACE_TYPE_VERBOSE);

	exe exec;

	exec.set_action(exe_config_parser, EXE_ACTION_CONFIG_PARSER);
	exec.set_evaluation_action(exe_eval_statement, EXE_EVAL_ACTION_STATEMENT);
	exec.initialize(input, arguments, is_file, true);
	exec.evaluate();

	TRACE_EVENT("-_ppt::_invoke_executor", TRACE_TYPE_VERBOSE);
}

void 
_ppt::_invoke_prompt(
	std::vector<std::string> &arguments
	)
{
	TRACE_EVENT("+_ppt::_invoke_prompt", TRACE_TYPE_INFORMATION);

	bool found;
	in_arg args;
	std::string input;
	std::vector<sch>::iterator cmp_sch;
	std::vector<arg> input_args, sub_args;

	if(!_argument_action) {
		THROW_PPT_EXC_W_MESS(PPT_HDL_ACTION_STRING(PPT_HDL_ACTION_ARGUMENTS), PPT_EXC_INVAL_HDL_ACTION_TYPE);
	}
	args = _argument_action(this, arguments);
	
	switch(args.get_signal()) {
		case PPT_ARG_SIG_EXIT:
			break;
		case PPT_ARG_SIG_INPUT:
			_invoke_executor(args);
			break;
		case PPT_ARG_SIG_INTERACT:
			signal(SIGINT, SIG_IGN);
			PROMPT_HEADER(input);
			std::cout << input << std::endl << std::endl;

			while(_active) {
				found = false;
				input.clear();
				sub_args.clear();
				input_args.clear();
				std::cout << CURSOR;

				if(!std::cin) {
					THROW_PPT_EXC(PPT_EXC_INPUT_PIPE_CLOSED);
				}
				getline(std::cin, input);

				if(input.empty()) {
					continue;
				}

				if(IS_COMMAND(input)) {

					try {
						input.erase(input.begin(), input.begin() + 1);
						_parse_command_input(input, input_args);
						sub_args = std::vector<arg>(input_args.begin() + 1, input_args.end());

						for(cmp_sch = _schema.begin(); cmp_sch != _schema.end(); ++cmp_sch) {

							if(cmp_sch->match(input_args.front().get_text(), sub_args)) {

								if(cmp_sch->has_action()) {
									cmp_sch->invoke_action(this, sub_args);
								}
								found = true;
								break;
							}
						}

						if(!found) {
							THROW_PPT_EXC_W_MESS(input_args.front().get_text(), PPT_EXC_UNK_SCHEMA_CMD_TYPE);
						}
					} catch(std::runtime_error &exc) {
						std::cerr << "Exception: " << exc.what() << std::endl << std::endl;
					}
				} else {

					try {

						if(!_generic_action) {
							THROW_PPT_EXC_W_MESS(PPT_HDL_ACTION_STRING(PPT_HDL_ACTION_GENERIC), PPT_EXC_INVAL_HDL_ACTION_TYPE);
						}
						input_args.push_back(arg(input, ARG_TYPE_STRING));
						_generic_action(this, input_args);
					} catch(std::runtime_error &exc) {

						if(_exception_action) {
							_exception_action(this, exc);
						}
						std::cerr << "Exception: " << exc.what() << std::endl << std::endl;
					}
				}
			}
			break;
		default:
			THROW_PPT_EXC_W_MESS(args.get_signal(), PPT_EXC_INVAL_SIGNAL_TYPE);
	}

	TRACE_EVENT("-_ppt::_invoke_prompt", TRACE_TYPE_INFORMATION);
}

void 
_ppt::_parse_command_input(
	const std::string &input,
	std::vector<arg> &arguments
	)
{
	TRACE_EVENT("+_ppt::_parse_command_input", TRACE_TYPE_VERBOSE);

	size_t i = 0;
	std::string curr_input;

	for(; i < input.size(); ++i) {

		while(i < input.size()
				&& IS_WHITESPACE(input.at(i))) {
			++i;
		}

		while(i < input.size()
				&& !IS_WHITESPACE(input.at(i))) {
			curr_input += input.at(i);
			++i;
		}
		arguments.push_back(arg(curr_input, ARG_TYPE_STRING));
		curr_input.clear();
	}

	TRACE_EVENT("+_ppt::_parse_command_input", TRACE_TYPE_VERBOSE);
}

void 
_ppt::clear(void)
{
	TRACE_EVENT("+_ppt::clear", TRACE_TYPE_VERBOSE);

	_active = false;
	_argument_action = NULL;
	_exception_action = NULL;
	_generic_action = NULL;
	_schema.clear();

	TRACE_EVENT("-_ppt::clear", TRACE_TYPE_VERBOSE);
}

size_t 
_ppt::get_schema_count(void)
{
	TRACE_EVENT("+_ppt::get_schema_count", TRACE_TYPE_VERBOSE);

	size_t result = _schema.size();

	TRACE_EVENT("-_ppt::get_schema_count", TRACE_TYPE_VERBOSE);

	return result;
}

bool 
_ppt::has_argument_action(void)
{
	TRACE_EVENT("+_ppt::has_argument_action", TRACE_TYPE_VERBOSE);

	bool result = _argument_action != NULL;

	TRACE_EVENT("-_ppt::has_argument_action", TRACE_TYPE_VERBOSE);

	return result;
}

bool 
_ppt::has_exeception_action(void)
{
	TRACE_EVENT("+_ppt::has_exeception_action", TRACE_TYPE_VERBOSE);

	bool result = _exception_action != NULL;

	TRACE_EVENT("-_ppt::has_exeception_action", TRACE_TYPE_VERBOSE);

	return result;
}

bool 
_ppt::has_generic_action(void)
{
	TRACE_EVENT("+_ppt::has_generic_action", TRACE_TYPE_VERBOSE);

	bool result = _generic_action != NULL;

	TRACE_EVENT("-_ppt::has_generic_action", TRACE_TYPE_VERBOSE);

	return result;
}

bool 
_ppt::has_schemas(void)
{
	TRACE_EVENT("+_ppt::has_schemas", TRACE_TYPE_VERBOSE);

	bool result = !_schema.empty();

	TRACE_EVENT("-_ppt::has_schemas", TRACE_TYPE_VERBOSE);

	return result;
}

bool 
_ppt::is_active(void)
{
	TRACE_EVENT("+_ppt::is_active", TRACE_TYPE_VERBOSE);

	bool result = _active;

	TRACE_EVENT("-_ppt::is_active", TRACE_TYPE_VERBOSE);

	return result;
}

void 
_ppt::register_schema(
	sch &schema
	)
{
	TRACE_EVENT("+_ppt::register_schema", TRACE_TYPE_INFORMATION);

	if(schema.get_command().empty()) {
		THROW_PPT_EXC(PPT_EXC_INVAL_SCHEMA_CMD);
	}

	if(_has_registered_schema(schema)) {
		THROW_PPT_EXC_W_MESS(schema.get_command(), PPT_EXC_DUP_SCHEMA_REG);
	}
	_schema.push_back(schema);

	TRACE_EVENT("-_ppt::register_schema", TRACE_TYPE_INFORMATION);
}

void 
_ppt::set_argument_action(
	ppt_arg_action argument_action
	)
{
	TRACE_EVENT("+_ppt::set_argument_action", TRACE_TYPE_VERBOSE);

	_argument_action = argument_action;

	TRACE_EVENT("-_ppt::set_argument_action", TRACE_TYPE_VERBOSE);
}

void 
_ppt::set_exception_action(
	ppt_exc_action exception_action
	)
{
	TRACE_EVENT("+_ppt::set_exception_action", TRACE_TYPE_VERBOSE);

	_exception_action = exception_action;

	TRACE_EVENT("-_ppt::set_exception_action", TRACE_TYPE_VERBOSE);
}

void 
_ppt::set_generic_action(
	ppt_action generic_action
	)
{
	TRACE_EVENT("+_ppt::set_generic_action", TRACE_TYPE_VERBOSE);

	_generic_action = generic_action;

	TRACE_EVENT("-_ppt::set_generic_action", TRACE_TYPE_VERBOSE);
}

void 
_ppt::start(
	std::vector<std::string> &arguments,
	ppt_init_action initialization
	)
{
	TRACE_EVENT("+_ppt::start", TRACE_TYPE_INFORMATION);

	if(_active) {
		THROW_PPT_EXC(PPT_EXC_INVAL_STATE_CHANGE);
	}
	_active = true;

	if(initialization) {
		initialization(this);
	}
	_invoke_prompt(arguments);

	TRACE_EVENT("-_ppt::start", TRACE_TYPE_INFORMATION);
}

void 
_ppt::stop(void)
{
	TRACE_EVENT("+_ppt::stop", TRACE_TYPE_INFORMATION);

	if(!_active) {
		THROW_PPT_EXC(PPT_EXC_INVAL_STATE_CHANGE);
	}
	_active = false;

	TRACE_EVENT("-_ppt::stop", TRACE_TYPE_INFORMATION);
}

std::string 
_ppt::to_string(
	bool verbose
	)
{
	TRACE_EVENT("_ppt::to_string", TRACE_TYPE_VERBOSE);

	std::stringstream ss;
	std::vector<sch>::iterator schema_iter = _schema.begin();

	ss << "Status:" << (_active ? " Active" : " Not Active") << ", Schema Count: " << _schema.size();

	if(verbose) {
		ss << " {" << std::endl;

		for(; schema_iter != _schema.end(); ++schema_iter) {
			ss << "\t" << schema_iter->to_string(verbose) << std::endl;
		}
		ss << std::endl << "Argument Action:";

		if(_argument_action) {
			ss << " [0x" << std::setw(sizeof(unsigned long) * 2) << std::setfill('0') << std::hex << _argument_action << "]";
		} else {
			ss << "[NULL]";
		}
		ss << std::endl << "Exception Action:";

		if(_exception_action) {
			ss << " [0x" << std::setw(sizeof(unsigned long) * 2) << std::setfill('0') << std::hex << _exception_action << "]";
		} else {
			ss << "[NULL]";
		}

		ss << "}" << std::endl << "Generic Action:"; 

		if(_generic_action) {
			ss << " [0x" << std::setw(sizeof(unsigned long) * 2) << std::setfill('0') << std::hex << _generic_action << "]";
		} else {
			ss << " [NULL]";
		}
	}

	return ss.str();
}

void 
_ppt::unregister_all_schema(void)
{
	TRACE_EVENT("+_ppt::unregister_all_schema", TRACE_TYPE_VERBOSE);

	_schema.clear();

	TRACE_EVENT("-_ppt::unregister_all_schema", TRACE_TYPE_VERBOSE);
}

void 
_ppt::unregister_schema(
	sch &schema
	)
{
	TRACE_EVENT("+_ppt::unregister_schema", TRACE_TYPE_INFORMATION);

	std::vector<sch>::iterator schema_iter = _get_registered_schema(schema);

	if(schema_iter == _schema.end()) {
		THROW_PPT_EXC_W_MESS(schema.get_command(), PPT_EXC_INVAL_SCHEMA_UNREG);
	}
	_schema.erase(schema_iter);

	TRACE_EVENT("-_ppt::unregister_schema", TRACE_TYPE_INFORMATION);
}