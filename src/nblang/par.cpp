/*
 * par.cpp
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
#include "par.h"

//#define PARSER_TRACING
#ifndef PARSER_TRACING
#define TRACE_EVENT(_M_, _T_)
#else
#define TRACE_HEADER "parser"
#include "trace.h"
#endif

_par::_par(void)
{
	TRACE_EVENT("+_par::_par", TRACE_TYPE_INFORMATION);

	memset(_actions, NULL, sizeof(par_action) * (MAX_PAR_ACTION_TYPE + 1));
	memset(_enum_actions, NULL, sizeof(par_enum_action) * (MAX_PAR_ENUM_ACTION_TYPE + 1));
	clear();

	TRACE_EVENT("-_par::_par", TRACE_TYPE_INFORMATION);
}

_par::_par(
	const _par &other
	) :
		_position(other._position),
		_statements(other._statements)
{
	TRACE_EVENT("+_par::_par", TRACE_TYPE_INFORMATION);

	memcpy(_actions, other._actions, sizeof(par_action) * (MAX_PAR_ACTION_TYPE + 1));
	memcpy(_enum_actions, other._enum_actions, sizeof(par_enum_action) * (MAX_PAR_ENUM_ACTION_TYPE + 1));
	lex::operator=(other);

	TRACE_EVENT("-_par::_par", TRACE_TYPE_INFORMATION);
}

_par::~_par(void)
{
	TRACE_EVENT("+_par::~_par", TRACE_TYPE_INFORMATION);
	TRACE_EVENT("-_par::~_par", TRACE_TYPE_INFORMATION);
}

_par &
_par::operator=(
	const _par &other
	)
{
	TRACE_EVENT("+_par::operator=", TRACE_TYPE_INFORMATION);

	if(this != &other) {
		_position = other._position;
		_statements = other._statements;
		memcpy(_actions, other._actions, sizeof(par_action) * (MAX_PAR_ACTION_TYPE + 1));
		memcpy(_enum_actions, other._enum_actions, sizeof(par_enum_action) * (MAX_PAR_ENUM_ACTION_TYPE + 1));
		lex::operator=(other);
	}

	TRACE_EVENT("-_par::operator=", TRACE_TYPE_INFORMATION);

	return *this;
}

void 
_par::_invoke_action(
	size_t type
	)
{
	TRACE_EVENT("+_par::_invoke_action", TRACE_TYPE_INFORMATION);

	if(type <= MAX_PAR_ACTION_TYPE) {

		if(_actions[type]) {
			_actions[type](this);
		} else {
			TRACE_EVENT("Parser action is unallocated: " << PAR_ACTION_STRING(type), TRACE_TYPE_ERROR);
			THROW_PAR_EXC_W_MESS(PAR_ACTION_STRING(type), PAR_EXC_UNALLOC_ACTION);
		}
	} else {
		TRACE_EVENT("Invocation of invalid parser action: " << type, TRACE_TYPE_ERROR);
		THROW_PAR_EXC_W_MESS(PAR_ACTION_STRING(type), PAR_EXC_INVAL_ACTION_TYPE);
	}

	TRACE_EVENT("-_par::_invoke_action", TRACE_TYPE_INFORMATION);
}

void 
_par::_invoke_enumeration_action(
	tok_stmt &statement,
	size_t type
	)
{
	TRACE_EVENT("+_par::_invoke_enumeration_action", TRACE_TYPE_INFORMATION);

	if(type <= MAX_PAR_ENUM_ACTION_TYPE) {

		if(_enum_actions[type]) {
			_enum_actions[type](this, statement);
		} else {
			TRACE_EVENT("Parser enumeration action is unallocated: " << PAR_ENUM_ACTION_STRING(type), TRACE_TYPE_ERROR);
			THROW_PAR_EXC_W_MESS(PAR_ENUM_ACTION_STRING(type), PAR_EXC_UNALLOC_ENUM_ACTION);
		}
	} else {
		TRACE_EVENT("Invocation of invalid parser enumeration action: " << type, TRACE_TYPE_ERROR);
		THROW_PAR_EXC_W_MESS(PAR_ENUM_ACTION_STRING(type), PAR_EXC_INVAL_ENUM_ACTION_TYPE);
	}

	TRACE_EVENT("-_par::_invoke_enumeration_action", TRACE_TYPE_INFORMATION);
}

void 
_par::clear(void)
{
	TRACE_EVENT("+_par::clear", TRACE_TYPE_INFORMATION);

	_position = 0;
	_statements.clear();
	_statements.add_back(tok_stmt(tok(CLASS_TOKEN_TYPE_BEGIN)));
	_statements.add_back(tok_stmt(tok(CLASS_TOKEN_TYPE_END)));
	lex::reset();

	TRACE_EVENT("-_par::clear", TRACE_TYPE_INFORMATION);
}

void 
_par::enumerate(void)
{
	TRACE_EVENT("+_par::enumerate", TRACE_TYPE_INFORMATION);

	clear();
	lex::move_next();

	while(has_next()) {
		move_next();
	}
	reset();

	TRACE_EVENT("-_par::enumerate", TRACE_TYPE_INFORMATION);
}

tok_stmt &
_par::export_statement(
	size_t index
	)
{
	TRACE_EVENT("_par::export_statement", TRACE_TYPE_INFORMATION);

	return _statements.get(index);
}

tok_stmt_list &
_par::export_statements(void)
{
	TRACE_EVENT("_par::export_statements", TRACE_TYPE_INFORMATION);

	return _statements;
}

tok_stmt &
_par::get(void)
{
	TRACE_EVENT("+_par::get", TRACE_TYPE_INFORMATION);

	if(_position >= _statements.get_link_count()) {
		TRACE_EVENT("Parser statement position is out of bounds! (" << _position << "/" << (_statements.has_links() ? 0 : (_statements.get_link_count() - 1)) << ")", TRACE_TYPE_ERROR);
		THROW_PAR_EXC_W_MESS(_position << " (" << (_statements.has_links() ? 0 : (_statements.get_link_count() - 1)) << ")", PAR_EXC_OUT_OF_BOUNDS);
	}

	TRACE_EVENT("-_par::get", TRACE_TYPE_INFORMATION);

	return _statements.get(_position);
}

size_t 
_par::get_count(void)
{
	TRACE_EVENT("_par::get_count", TRACE_TYPE_VERBOSE);

	return _statements.get_link_count() - MIN_LEX_TOKEN_COUNT;
}

size_t 
_par::get_position(void)
{
	TRACE_EVENT("_par::get_position", TRACE_TYPE_VERBOSE);

	return _position;
}

bool 
_par::has_next(void)
{
	TRACE_EVENT("_par::has_next", TRACE_TYPE_VERBOSE);

	return get().get_root().get_class_type() != CLASS_TOKEN_TYPE_END;
}
		
bool 
_par::has_previous(void)
{
	TRACE_EVENT("_par::has_previous", TRACE_TYPE_VERBOSE);

	return _position > 0;
}
		
bool 
_par::has_statements(void)
{
	TRACE_EVENT("_par::has_statements", TRACE_TYPE_VERBOSE);

	return !_statements.has_links();
}

void 
_par::import_statements(
	tok_stmt_list &statements
	)
{
	TRACE_EVENT("+_par::import_statements", TRACE_TYPE_INFORMATION);

	size_t i = 1;

	if(statements.get_link_count() >= MIN_IMPORT_STATEMENT_COUNT) {

		for(; i < statements.get_link_count() - 1; ++i) {
			_statements.insert(statements.get(i), _position + i + (_statements.get_link_count() >= MIN_IMPORT_STATEMENT_COUNT ? (-1) : 0));
		}
	}

	TRACE_EVENT("-_par::import_statements", TRACE_TYPE_INFORMATION);
}

void 
_par::initialize(
	const std::string &input,
	bool is_file
	)
{
	TRACE_EVENT("+_par::initialize", TRACE_TYPE_INFORMATION);

	lex::initialize(input, is_file);
	reset();
	lex::enumerate();

	TRACE_EVENT("-_par::initialize", TRACE_TYPE_INFORMATION);
}

void 
_par::move_next(void)
{
	TRACE_EVENT("+_par::move_next", TRACE_TYPE_INFORMATION);

	tok_stmt stmt;

	if(lex::has_next()
			&& _position == (_statements.get_link_count() - 2)) {\
		_invoke_enumeration_action(stmt, PAR_ENUM_ACTION_STATEMENT);
		_statements.insert(stmt, ++_position);
	} else if(_position < (_statements.get_link_count() - 1)) {
		++_position;
	} else {
		TRACE_EVENT("Parser has no next statement!", TRACE_TYPE_ERROR);
		THROW_PAR_EXC_W_MESS(_position, PAR_EXC_NO_NEXT_TOKEN);
	}

	TRACE_EVENT("-_par::move_next", TRACE_TYPE_INFORMATION);
}
		
void 
_par::move_previous(void)
{
	TRACE_EVENT("+_par::move_previous", TRACE_TYPE_INFORMATION);

	if(has_previous()) {
		--_position;
	} else {
		TRACE_EVENT("Parser has no previous statement!", TRACE_TYPE_ERROR);
		THROW_PAR_EXC_W_MESS(_position, PAR_EXC_NO_PREV_TOKEN);
	}

	TRACE_EVENT("-_par::move_previous", TRACE_TYPE_INFORMATION);
}

void 
_par::reset(void)
{
	TRACE_EVENT("+_par::reset", TRACE_TYPE_INFORMATION);

	_position = 0;
	_invoke_action(PAR_ACTION_CONFIG_LEXER);

	TRACE_EVENT("-_par::reset", TRACE_TYPE_INFORMATION);
}

void 
_par::set_action(
	par_action action,
	size_t type
	)
{
	TRACE_EVENT("+_par::set_action", TRACE_TYPE_INFORMATION);

	if(type <= MAX_PAR_ACTION_TYPE) {
		_actions[type] = action;
	} else {
		TRACE_EVENT("Invalid parser action type: " << type, TRACE_TYPE_ERROR);
		THROW_PAR_EXC_W_MESS(PAR_ACTION_STRING(type), PAR_EXC_INVAL_ACTION_TYPE);
	}

	TRACE_EVENT("-_par::set_action", TRACE_TYPE_INFORMATION);
}

void 
_par::set_enumeration_action(
	par_enum_action action,
	size_t type
	)
{
	TRACE_EVENT("+_par::set_enumeration_action", TRACE_TYPE_INFORMATION);

	if(type <= MAX_PAR_ENUM_ACTION_TYPE) {
		_enum_actions[type] = action;
	} else {
		TRACE_EVENT("Invalid parser enumeration action type: " << type, TRACE_TYPE_ERROR);
		THROW_PAR_EXC_W_MESS(PAR_ENUM_ACTION_STRING(type), PAR_EXC_INVAL_ENUM_ACTION_TYPE);
	}

	TRACE_EVENT("-_par::set_enumeration_action", TRACE_TYPE_INFORMATION);
}

std::string 
_par::to_string(
	bool verbose
	)
{
	TRACE_EVENT("_par::to_string", TRACE_TYPE_VERBOSE);

	return get().to_string(verbose);
}