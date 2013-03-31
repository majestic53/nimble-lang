/*
 * in_arg.cpp
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
#include "arg_type.h"
#include "in_arg.h"
#include "in_arg_type.hpp"
#include "ppt_type.h"

_in_arg::_in_arg(void)
{
	clear();
}

_in_arg::_in_arg(
	const std::string &input,
	bool is_file
	)
{
	clear(input, is_file);
}

_in_arg::_in_arg(
	const _in_arg &other
	) :
		_arguments(other._arguments),
		_input(other._input),
		_is_file(other._is_file),
		_signal(other._signal)
{
	return;
}

_in_arg::~_in_arg(void)
{
	return;
}

_in_arg &
_in_arg::operator=(
	const _in_arg &other
	)
{
	if(this != &other) {
		_arguments = other._arguments;
		_input = other._input;
		_is_file = other._is_file;
		_signal = other._signal;
	}

	return *this;
}

void 
_in_arg::add_argument_back(
	const arg &argument
	)
{
	_arguments.push_back(argument);
}

void 
_in_arg::add_argument_front(
	const arg &argument
	)
{
	_arguments.insert(_arguments.begin(), argument);
}

void 
_in_arg::clear(void)
{
	_arguments.clear();
	_input.clear();
	_is_file = false;
	_signal = PPT_ARG_SIG_EXIT;
}

void 
_in_arg::clear(
	const std::string &input,
	bool is_file
	)
{
	_arguments.clear();
	_input = input;
	_is_file = is_file;
	_signal = PPT_ARG_SIG_EXIT;
}

arg &
_in_arg::get_argument(
	size_t index
	)
{
	if(index >= _arguments.size()) {
		THROW_IN_ARG_EXC_W_MESS(index, IN_ARG_EXC_INVAL_ARG_INDEX);
	}

	return _arguments.at(index);
}

size_t 
_in_arg::get_argument_count(void)
{
	return _arguments.size();
}

std::string &
_in_arg::get_input(void)
{
	return _input;
}

size_t 
_in_arg::get_signal(void)
{
	return _signal;
}

bool 
_in_arg::has_arguments(void)
{
	return !_arguments.empty();
}

void 
_in_arg::insert_argument(
	const arg &argument,
	size_t index
	)
{
	if(index < _arguments.size()) {
		_arguments.insert(_arguments.begin() + index, argument);
	} else if(index == _arguments.size()) {
		_arguments.push_back(argument);
	} else {
		THROW_IN_ARG_EXC_W_MESS(index, IN_ARG_EXC_INVAL_ARG_INDEX);
	}
}

bool 
_in_arg::is_file(void)
{
	return _is_file;
}

void 
_in_arg::remove_all_arguments(void)
{
	_arguments.clear();
}

void 
_in_arg::remove_argument(
	size_t index
	)
{
	if(index >= _arguments.size()) {
		THROW_IN_ARG_EXC_W_MESS(index, IN_ARG_EXC_INVAL_ARG_INDEX);
	}
	_arguments.erase(_arguments.begin() + index);
}

void 
_in_arg::set_argument(
	const arg &argument,
	size_t index
	)
{
	if(index >= _arguments.size()) {
		THROW_IN_ARG_EXC_W_MESS(index, IN_ARG_EXC_INVAL_ARG_INDEX);
	}
	_arguments.at(index) = argument;
}

void 
_in_arg::set_as_file(
	bool is_file
	)
{
	_is_file = is_file;
}

void 
_in_arg::set_input(
	const std::string &input
	)
{
	_input = input;
}

void 
_in_arg::set_signal(
	size_t signal
	)
{
	_signal = signal;
}

std::string 
_in_arg::to_string(
	bool verbose
	)
{
	std::stringstream ss;
	std::vector<arg>::iterator arg_iter = _arguments.begin();

	if(IS_ARGUMENT_0(_input)
			|| IS_ARGUMENT_1(_input)) {
		ss << "[" << PPT_ARG_STRING_1(ppt_find_argument(_input)) << "] ";
	}
	ss << _input << (_is_file ? " (FILE) " : " ");

	if(verbose) {
		ss << "(" << _arguments.size() << ") {" << std::endl;

		for(; arg_iter != _arguments.end(); ++arg_iter) {
			ss << "\t" << arg_iter->to_string(verbose) << std::endl;
		}
		ss << "}";
	}

	return ss.str();
}