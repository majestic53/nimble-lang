/*
 * sch.cpp
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

#include <iomanip>
#include <sstream>
#include <stdexcept>
#include "arg_type.h"
#include "sch.h"
#include "sch_type.h"

_sch::_sch(void)
{
	clear();
}

_sch::_sch(
	const std::string &command
	)
{
	clear(command);
}

_sch::_sch(
	const _sch &other
	) :
		_action(other._action),
		_arguments(other._arguments),
		_command(other._command),
		_wildcard(other._wildcard)
{
	return;
}

_sch::~_sch(void)
{
	return;
}

_sch &
_sch::operator=(
	const _sch &other
	)
{
	if(this != &other) {
		_action = other._action;
		_arguments = other._arguments;
		_command = other._command;
		_wildcard = other._wildcard;
	}

	return *this;
}

bool 
_sch::operator==(
	const _sch &other
	)
{
	bool result = true;
	std::vector<arg>::iterator arg_iter = _arguments.begin();
	std::vector<arg>::const_iterator other_arg_iter = other._arguments.begin();

	if(this != &other) {
		result = _action == other._action
				&& _arguments.size() == other._arguments.size()
				&& _command == other._command
				&& _wildcard == other._wildcard;

		if(result) {

			for(; arg_iter != _arguments.end(); ++arg_iter, ++other_arg_iter) {

				if((arg) *arg_iter != (arg) *other_arg_iter) {
					result = false;
					break;
				}
			}
		}
	}

	return result;
}

bool 
_sch::operator!=(
	const _sch &other
	)
{
	return !(*this == other);
}

void 
_sch::add_argument_back(
	const arg &argument
	)
{
	_arguments.push_back(argument);
}

void 
_sch::add_argument_front(
	const arg &argument
	)
{
	_arguments.insert(_arguments.begin(), argument);
}

void 
_sch::clear(void)
{
	_action = NULL;
	_arguments.clear();
	_command.clear();
	_wildcard = false;
}

void 
_sch::clear(
	const std::string &command
	)
{
	_action = NULL;
	_arguments.clear();
	_command = command;
	_wildcard = false;
}

arg 
_sch::get_argument(
	size_t index
	)
{
	if(index >= _arguments.size()) {
		THROW_SCH_EXC_W_MESS(index, SCH_EXC_INVAL_ARG_INDEX);
	}

	return _arguments.at(index);
}

size_t 
_sch::get_argument_count(void)
{
	return _arguments.size();
}

std::string 
_sch::get_command(void)
{
	return _command;
}

bool 
_sch::has_action(void)
{
	return _action != NULL;
}

bool 
_sch::has_arguments(void)
{
	return !_arguments.empty();
}

void 
_sch::insert_argument(
	const arg &argument,
	size_t index
	)
{
	if(index < _arguments.size()) {
		_arguments.insert(_arguments.begin() + index, argument);
	} else if(index == _arguments.size()) {
		_arguments.push_back(argument);
	} else {
		THROW_SCH_EXC_W_MESS(index, SCH_EXC_INVAL_ARG_INDEX);
	}
}

void 
_sch::invoke_action(
	ppt_ptr prom,
	std::vector<arg> &arguments
	)
{
	if(!_action) {
		THROW_SCH_EXC(SCH_EXC_UNALLOC_ACTION);
	}
	_action(prom, arguments);
}

bool 
_sch::is_wildcard(void)
{
	return _wildcard;
}

bool 
_sch::match(
	const std::string &command,
	const std::vector<arg> &arguments
	)
{
	bool result = false;
	std::vector<arg>::iterator arg_iter = _arguments.begin();
	std::vector<arg>::const_iterator in_arg_iter = arguments.begin();

	if(command == _command) {
		result = true;
		
		if((!_wildcard && _arguments.size() == arguments.size())
				|| (_wildcard && _arguments.size() <= arguments.size())) {

			for(; arg_iter != _arguments.end(); ++arg_iter, ++in_arg_iter) {

				if((arg) *in_arg_iter != (arg) *arg_iter) {
					result = false;
					break;
				}
			}
		} else {
			result = false;
		}
	}

	return result;
}

void 
_sch::remove_all_arguments(void)
{
	_arguments.clear();
}

void 
_sch::remove_argument(
	size_t index
	)
{
	if(index >= _arguments.size()) {
		THROW_SCH_EXC_W_MESS(index, SCH_EXC_INVAL_ARG_INDEX);
	}

	_arguments.erase(_arguments.begin() + index);
}

void 
_sch::remove_argument_back(void)
{
	if(_arguments.empty()) {
		THROW_SCH_EXC(SCH_EXC_NO_ARG);
	}

	_arguments.pop_back();
}

void 
_sch::remove_argument_front(void)
{
	if(_arguments.empty()) {
		THROW_SCH_EXC(SCH_EXC_NO_ARG);
	}

	_arguments.erase(_arguments.begin());
}

void 
_sch::set_action(
	ppt_action action
	)
{
	_action = action;
}

void 
_sch::set_argument(
	const arg &argument,
	size_t index
	)
{
	if(index >= _arguments.size()) {
		THROW_SCH_EXC_W_MESS(index, SCH_EXC_INVAL_ARG_INDEX);
	}

	_arguments.at(index) = argument;
}

void 
_sch::set_as_wildcard(
	bool wildcard
	)
{
	_wildcard = wildcard;
}

void 
_sch::set_command(
	const std::string &command
	)
{
	_command = command;
}

std::string 
_sch::to_string(
	bool verbose
	)
{
	std::stringstream ss;
	std::vector<arg>::iterator argument = _arguments.begin();

	ss << _command; 
	
	if(_wildcard) {
		ss << " (WILDCARD) (";
	} else {
		ss << " (" << _arguments.size() << ") (";
	}

	for(; argument != _arguments.end(); ++argument) {

		if(argument != _arguments.begin()) {
			ss << ", ";
		}
		ss << argument->to_string(verbose);
	}
	ss << ")";

	if(verbose) {
		ss << " Action:"; 
		
		if(_action) {
			ss << " [0x" << std::setw(sizeof(unsigned long) * 2) << std::setfill('0') << std::hex << _action << "]";
		} else {
			ss << " [NULL]";
		}
	}

	return ss.str();
}