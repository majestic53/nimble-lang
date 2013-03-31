/*
 * tok.cpp
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
#include "lang.h"
#include "tok.h"
#include "tok_type.h"

_tok::_tok(void)
{
	clear();
}

_tok::_tok(
	size_t class_type
	)
{
	clear();
	_class_type = class_type;
}

_tok::_tok(
	const _tok &other
	) :
		_class_type(other._class_type),
		_line(other._line),
		_position(other._position),
		_subtype(other._subtype),
		_type(other._type),
		_text(other._text)
{
	return;
}

_tok::~_tok(void)
{
	return;
}

_tok &
_tok::operator=(
	const _tok &other
	)
{
	if(this != &other) {
		_class_type = other._class_type;
		_line = other._line;
		_position = other._position;
		_subtype = other._subtype;
		_type = other._type;
		_text = other._text;
	}

	return *this;
}

void 
_tok::ceiling(void)
{
	std::stringstream ss;

	if(_class_type != CLASS_TOKEN_TYPE_TOKEN) {
		THROW_TOK_EXC_W_MESS(_class_type, TOK_EXC_INVALID_CLS_TYPE);
	}

	if(_type == TOKEN_TYPE_INTEGER) {
		return;
	}

	switch(_type) {
		case TOKEN_TYPE_FLOAT:
			ss << (double) std::ceil(to_float());
			break;
		case TOKEN_TYPE_INTEGER:
			break;
		default:
			THROW_TOK_EXC_W_MESS(_type, TOK_EXC_INVALID_TYPE);
			break;
	}
	_text = ss.str();
}

void 
_tok::clear(void)
{
	_class_type = CLASS_TOKEN_TYPE_STATEMENT;
	_line = 0;
	_position = 0;
	_subtype = INVALID_TYPE;
	_type = INVALID_TYPE;
	_text.clear();
}

void 
_tok::floor(void)
{
	std::stringstream ss;

	if(_class_type != CLASS_TOKEN_TYPE_TOKEN) {
		THROW_TOK_EXC_W_MESS(_class_type, TOK_EXC_INVALID_CLS_TYPE);
	}

	if(_type == TOKEN_TYPE_INTEGER) {
		return;
	}

	switch(_type) {
		case TOKEN_TYPE_FLOAT:
			ss << (double) std::floor(to_float());
			break;
		case TOKEN_TYPE_INTEGER:
			break;
		default:
			THROW_TOK_EXC_W_MESS(_type, TOK_EXC_INVALID_TYPE);
			break;
	}
	_text = ss.str();
}

size_t 
_tok::get_class_type(void)
{
	return _class_type;
}

size_t 
_tok::get_line(void)
{
	return _line;
}

size_t 
_tok::get_position(void)
{
	return _position;
}

size_t 
_tok::get_subtype(void)
{
	return _subtype;
}

size_t 
_tok::get_type(void)
{
	return _type;
}

std::string &
_tok::get_text(void)
{
	return _text;
}

void 
_tok::negate(void)
{
	std::stringstream ss;

	if(_class_type != CLASS_TOKEN_TYPE_TOKEN) {
		THROW_TOK_EXC_W_MESS(_class_type, TOK_EXC_INVALID_CLS_TYPE);
	}

	switch(_type) {
		case TOKEN_TYPE_FLOAT:
			ss << (double) (to_float() * -1.0);
			break;
		case TOKEN_TYPE_INTEGER:
			ss << (long) (to_integer() * -1);
			break;
		default:
			THROW_TOK_EXC_W_MESS(_type, TOK_EXC_INVALID_TYPE);
			break;
	}
	_text = ss.str();
}

void 
_tok::round(void)
{
	std::stringstream ss;

	if(_class_type != CLASS_TOKEN_TYPE_TOKEN) {
		THROW_TOK_EXC_W_MESS(_class_type, TOK_EXC_INVALID_CLS_TYPE);
	}

	if(_type == TOKEN_TYPE_INTEGER) {
		return;
	}

	switch(_type) {
		case TOKEN_TYPE_FLOAT:
			ss << (double) std::floor(to_float() + 0.5);
			break;
		case TOKEN_TYPE_INTEGER:
			break;
		default:
			THROW_TOK_EXC_W_MESS(_type, TOK_EXC_INVALID_TYPE);
			break;
	}
	_text = ss.str();
}

void 
_tok::set_as_token(
	size_t type,
	size_t subtype
	)
{
	_class_type = CLASS_TOKEN_TYPE_TOKEN;
	_type = type;
	_subtype = subtype;
}

void 
_tok::set_class_type(
	size_t class_type
	)
{
	_class_type = class_type;
}

void 
_tok::set_line(
	size_t line
	)
{
	_line = line;
}

void 
_tok::set_position(
	size_t position
	)
{
	_position = position;
}

void 
_tok::set_subtype(
	size_t subtype
	)
{
	_subtype = subtype;
}

void 
_tok::set_type(
	size_t type
	)
{
	_type = type;
}

void 
_tok::shift_left(
	size_t amount
	)
{
	std::stringstream ss;

	ss << (long) (to_integer() << amount);
	_text = ss.str();
}

void 
_tok::shift_right(
	size_t amount
	)
{
	std::stringstream ss;

	ss << (long) (to_integer() >> amount);
	_text = ss.str();
}

double 
_tok::to_float(void)
{
	if(_class_type != CLASS_TOKEN_TYPE_TOKEN
			|| (_type != TOKEN_TYPE_INTEGER
				&& _type != TOKEN_TYPE_FLOAT)) {
		THROW_TOK_EXC_W_MESS(to_string(true) << " (" << _class_type << ", " << _type << ")", TOK_EXC_NON_NUMERIC_TYPE);
	}

	return std::atof(_text.c_str());
}

long 
_tok::to_integer(void)
{
	if(_class_type != CLASS_TOKEN_TYPE_TOKEN
			|| (_type != TOKEN_TYPE_INTEGER
				&& _type != TOKEN_TYPE_FLOAT)) {
		THROW_TOK_EXC_W_MESS(to_string(true) << " (" << _class_type << ", " << _type << ")", TOK_EXC_NON_NUMERIC_TYPE);
	}

	return std::atoi(_text.c_str());
}

std::string 
_tok::to_string(
	bool verbose
	)
{
	std::string subtype;
	std::stringstream ss;

	switch(_class_type) {
		case CLASS_TOKEN_TYPE_TOKEN:
			subtype = TOKEN_SUBTYPE_STRING(_type, _subtype);
			ss << "[" << TOKEN_TYPE_STRING(_type);

			if(subtype != NONE_TYPE) {
				ss << ", " << subtype;
			}
			ss << "]";

			switch(_type) {
				case TOKEN_TYPE_FLOAT:
				case TOKEN_TYPE_IDENTIFIER:
				case TOKEN_TYPE_INTEGER:
					ss << " " << _text;
					break;
				case TOKEN_TYPE_VAR_STRING:
					ss << " \'" << _text << "\'";
					break;
			}

			if(verbose) {
				ss << " (ln. " << _line << ", pos. " << _position << ")";
			}
			break;
		default:
			ss << "[" << CLASS_TOK_TYPE_STRING(_class_type) << "]";
			break;
	}

	return ss.str();
}