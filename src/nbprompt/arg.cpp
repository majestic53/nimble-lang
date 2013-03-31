/*
 * arg.cpp
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
#include "arg.h"
#include "arg_type.h"
#include "..\nblang\lang.h"
#include "..\nblang\tok.h"

_arg::_arg(void)
{
	clear();
}

_arg::_arg(
	size_t type
	)
{
	clear(type);
}

_arg::_arg(
	const std::string text,
	size_t type
	) :
		_text(text),
		_type(type)
{
	return;
}

_arg::_arg(
	const _arg &other
	) :
		_text(other._text),
		_type(other._type)
{
	return;
}

_arg::~_arg(void)
{
	return;
}

_arg &
_arg::operator=(
	const _arg &other
	)
{
	if(this != &other) {
		_text = other._text;
		_type = other._type;
	}

	return *this;
}

bool 
_arg::operator==(
	const _arg &other
	)
{
	return _type == other._type;
}

bool 
_arg::operator!=(
	const _arg &other
	)
{
	return !(*this == other);
}

void 
_arg::clear(void)
{
	clear(ARG_TYPE_STATIC);
}

void 
_arg::clear(
	size_t type
	)
{
	_text.clear();
	_type = type;
}

std::string 
_arg::get_text(void)
{
	return _text;
}

size_t 
_arg::get_type(void)
{
	return _type;
}

bool 
_arg::has_text(void)
{
	return !_text.empty();
}

void 
_arg::set_text(
	const std::string &text
	)
{
	_text = text;
}

void 
_arg::set_type(
	size_t type
	)
{
	_type = type;
}

std::string 
_arg::to_string(
	bool verbose
	)
{
	std::stringstream ss;

	ss << "[" << ARG_TYPE_STRING(_type) << "]";

	if(verbose
			&& !_text.empty()) {
		ss << " " << _text;
	}

	return ss.str();
}

tok 
_arg::to_token(void)
{
	tok result;
	
	result.set_as_token(_type == ARG_TYPE_FLOAT ? TOKEN_TYPE_FLOAT : (_type == ARG_TYPE_INTEGER ? TOKEN_TYPE_INTEGER : TOKEN_TYPE_VAR_STRING), INVALID_TYPE);
	result.get_text() = _text;

	return result;
}