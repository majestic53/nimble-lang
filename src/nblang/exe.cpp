/*
 * exe.cpp
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

#include <queue>
#include <sstream>
#include <stdexcept>
#include "exe.h"
#include "lang.h"

//#define EXECUTOR_TRACING
#ifndef EXECUTOR_TRACING
#define TRACE_EVENT(_M_, _T_)
#else
#define TRACE_HEADER "executor"
#include "trace.h"
#endif

_exe::_exe(void)
{
	TRACE_EVENT("+_exe::_exe", TRACE_TYPE_INFORMATION);

	memset(_actions, NULL, sizeof(exe_action) * (MAX_EXE_ACTION_TYPE + 1));
	memset(_eval_actions, NULL, sizeof(exe_eval_action) * (MAX_EXE_EVAL_ACTION_TYPE + 1));

	TRACE_EVENT("-_exe::_exe", TRACE_TYPE_INFORMATION);
}

_exe::_exe(
	const _exe &other
	) :
		_scope(other._scope),
		_stack(other._stack),
		_arguments(other._arguments)
{
	TRACE_EVENT("+_exe::_exe", TRACE_TYPE_INFORMATION);

	memcpy(_actions, other._actions, sizeof(exe_action) * (MAX_EXE_ACTION_TYPE + 1));
	memcpy(_eval_actions, other._eval_actions, sizeof(exe_eval_action) * (MAX_EXE_EVAL_ACTION_TYPE + 1));
	par::operator=(other);

	TRACE_EVENT("-_exe::_exe", TRACE_TYPE_INFORMATION);
}

_exe::~_exe(void)
{
	TRACE_EVENT("+_exe::~_exe", TRACE_TYPE_INFORMATION);
	TRACE_EVENT("-_exe::~_exe", TRACE_TYPE_INFORMATION);
}

_exe &
_exe::operator=(
	const _exe &other
	)
{
	TRACE_EVENT("+_exe::operator=", TRACE_TYPE_INFORMATION);

	if(this != &other) {
		_scope = other._scope;
		_stack = other._stack;
		_arguments = other._arguments;
		memcpy(_actions, other._actions, sizeof(exe_action) * (MAX_EXE_ACTION_TYPE + 1));
		memcpy(_eval_actions, other._eval_actions, sizeof(exe_eval_action) * (MAX_EXE_EVAL_ACTION_TYPE + 1));
		par::operator=(other);
	}

	TRACE_EVENT("-_exe::operator=", TRACE_TYPE_INFORMATION);

	return *this;
}

void 
_exe::_invoke_action(
	size_t type
	)
{
	TRACE_EVENT("+_exe::_invoke_action", TRACE_TYPE_INFORMATION);

	if(type <= MAX_EXE_ACTION_TYPE) {

		if(_actions[type]) {
			_actions[type](this);
		} else {
			TRACE_EVENT("Executor action is unallocated: " << EXE_ACTION_STRING(type), TRACE_TYPE_ERROR);
			THROW_EXE_EXC_W_MESS(EXE_ACTION_STRING(type), EXE_EXC_UNALLOC_ACTION);
		}
	} else {
		TRACE_EVENT("Invocation of invalid executor action: " << type, TRACE_TYPE_ERROR);
		THROW_EXE_EXC_W_MESS(EXE_ACTION_STRING(type), EXE_EXC_INVAL_ACTION_TYPE);
	}

	TRACE_EVENT("-_exe::_invoke_action", TRACE_TYPE_INFORMATION);
}

void 
_exe::_invoke_evaluation_action(
	size_t type
	)
{
	TRACE_EVENT("+_exe::_invoke_evaluation_action", TRACE_TYPE_INFORMATION);

	if(type <= MAX_EXE_EVAL_ACTION_TYPE) {

		if(_eval_actions[type]) {
			_eval_actions[type](_arguments, this, _scope, _stack);
		} else {
			TRACE_EVENT("Executor enumeration action is unallocated: " << EXE_EVAL_ACTION_STRING(type), TRACE_TYPE_ERROR);
			THROW_EXE_EXC_W_MESS(EXE_EVAL_ACTION_STRING(type), EXE_EXC_UNALLOC_EVAL_ACTION);
		}
	} else {
		TRACE_EVENT("Invocation of invalid executor enumeration action: " << type, TRACE_TYPE_ERROR);
		THROW_EXE_EXC_W_MESS(EXE_EVAL_ACTION_STRING(type), EXE_EXC_INVAL_EVAL_ACTION_TYPE);
	}

	TRACE_EVENT("-_exe::_invoke_evaluation_action", TRACE_TYPE_INFORMATION);
}

void 
_exe::clear_stack(void)
{
	TRACE_EVENT("+_exe::clear_stack", TRACE_TYPE_VERBOSE);

	while(!_stack.empty()) {
		_stack.pop();
	}
}

void 
_exe::evaluate(void)
{
	TRACE_EVENT("+_exe::evaluate", TRACE_TYPE_INFORMATION);

	if(IS_CLASS_TYPE_T(par::get().get_root(), CLASS_TOKEN_TYPE_BEGIN)) {
		par::move_next();
	}

	while(has_next()) {
		step();
	}

	TRACE_EVENT("-_exe::evaluate", TRACE_TYPE_INFORMATION);
}

cont &
_exe::get(void)
{
	TRACE_EVENT("+_exe::get", TRACE_TYPE_VERBOSE);

	return _scope.get();
}

tok &
_exe::get_argument(
	size_t index
	)
{
	TRACE_EVENT("+_exe::get_argument", TRACE_TYPE_INFORMATION);

	if(index >= _arguments.size()) {
		TRACE_EVENT("Executor argument index is out of bounds! (" << index << "/" << (_arguments.empty() ? 0 : (_arguments.size() - 1)) << ")", TRACE_TYPE_ERROR);
		THROW_EXE_EXC_W_MESS(index << " (" << (_arguments.empty() ? 0 : (_arguments.size() - 1)) << ")", EXE_EXC_ARG_OUT_OF_BOUNDS);
	}

	TRACE_EVENT("-_exe::get_argument", TRACE_TYPE_INFORMATION);

	return _arguments.at(index);
}

size_t 
_exe::get_argument_count(void)
{
	TRACE_EVENT("_exe::get_argument_count", TRACE_TYPE_VERBOSE);

	return _arguments.size();
}

tok_vector &
_exe::get_arguments(void)
{
	TRACE_EVENT("_exe::get_arguments", TRACE_TYPE_VERBOSE);

	return _arguments;
}

tok_stack &
_exe::get_stack(void)
{
	TRACE_EVENT("_exe::get_stack", TRACE_TYPE_VERBOSE);

	return _stack;
}

bool 
_exe::has_next(void)
{
	TRACE_EVENT("+_exe::has_next", TRACE_TYPE_INFORMATION);

	return par::has_next();

	TRACE_EVENT("-_exe::has_next", TRACE_TYPE_INFORMATION);
}

void 
_exe::initialize(
	const std::string &input,
	const tok_vector &arguments,
	bool is_file,
	bool clear_scope
	)
{
	TRACE_EVENT("+_exe::initialize", TRACE_TYPE_INFORMATION);

	reset(clear_scope);
	par::initialize(input, is_file);
	par::enumerate();
	_arguments = arguments;

	TRACE_EVENT("-_exe::initialize", TRACE_TYPE_INFORMATION);
}

void 
_exe::reset(
	bool clear_scope
	)
{
	TRACE_EVENT("+_exe::reset", TRACE_TYPE_INFORMATION);

	if(clear_scope) {
		_scope.clear();
	}

	while(!_stack.empty()) {
		_stack.pop();
	}
	_invoke_action(EXE_ACTION_CONFIG_PARSER);
	par::reset();

	TRACE_EVENT("-_exe::reset", TRACE_TYPE_INFORMATION);
}

void 
_exe::set_action(
	exe_action action,
	size_t type
	)
{
	TRACE_EVENT("+_exe::set_action", TRACE_TYPE_INFORMATION);

	if(type <= MAX_EXE_ACTION_TYPE) {
		_actions[type] = action;
	} else {
		TRACE_EVENT("Invalid executor action type: " << type, TRACE_TYPE_ERROR);
		THROW_EXE_EXC_W_MESS(EXE_ACTION_STRING(type), EXE_EXC_INVAL_ACTION_TYPE);
	}

	TRACE_EVENT("-_exe::set_action", TRACE_TYPE_INFORMATION);
}

void 
_exe::set_evaluation_action(
	exe_eval_action action,
	size_t type
	)
{
	TRACE_EVENT("+_exe::set_evaluation_action", TRACE_TYPE_INFORMATION);

	if(type <= MAX_EXE_EVAL_ACTION_TYPE) {
		_eval_actions[type] = action;
	} else {
		TRACE_EVENT("Invalid executor evaluation action type: " << type, TRACE_TYPE_ERROR);
		THROW_EXE_EXC_W_MESS(EXE_EVAL_ACTION_STRING(type), EXE_EXC_INVAL_EVAL_ACTION_TYPE);
	}

	TRACE_EVENT("-_exe::set_evaluation_action", TRACE_TYPE_INFORMATION);
}

void 
_exe::step(void)
{
	TRACE_EVENT("+_exe::step", TRACE_TYPE_INFORMATION);

	if(IS_CLASS_TYPE_T(par::get().get_root(), CLASS_TOKEN_TYPE_BEGIN)) {
		par::move_next();
	}

	if(has_next()) {
		_invoke_evaluation_action(EXE_EVAL_ACTION_STATEMENT);
	} else {
		TRACE_EVENT("Executor has no next statement to step to!", TRACE_TYPE_ERROR);
		THROW_EXE_EXC(EXE_EXC_NO_NEXT_STATEMENT);
	}

	TRACE_EVENT("-_exe::step", TRACE_TYPE_INFORMATION);
}

std::string 
_exe::to_string(
	bool verbose
	)
{
	TRACE_EVENT("_exe::to_string", TRACE_TYPE_VERBOSE);

	return _scope.get().to_string(verbose);
}