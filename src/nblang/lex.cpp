/*
 * lex.cpp
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
#include "lex.h"
#include "tok.h"

//#define LEXER_TRACING
#ifndef LEXER_TRACING
#define TRACE_EVENT(_M_, _T_)
#else
#define TRACE_HEADER "lexer"
#include "trace.h"
#endif

_lex::_lex(void)
{
	TRACE_EVENT("+_lex::_lex", TRACE_TYPE_INFORMATION);

	memset(_actions, NULL, sizeof(lex_action) * (MAX_LEX_ACTION_TYPE + 1));
	memset(_enum_actions, NULL, sizeof(lex_enum_action) * (MAX_LEX_ENUM_ACTION_TYPE + 1));
	clear();

	TRACE_EVENT("-_lex::_lex", TRACE_TYPE_INFORMATION);
}

_lex::_lex(
	const _lex &other
	) :
		_position(other._position),
		_tokens(other._tokens)
{
	TRACE_EVENT("+_lex::_lex", TRACE_TYPE_INFORMATION);

	memcpy(_actions, other._actions, sizeof(lex_action) * (MAX_LEX_ACTION_TYPE + 1));
	memcpy(_enum_actions, other._enum_actions, sizeof(lex_enum_action) * (MAX_LEX_ENUM_ACTION_TYPE + 1));
	lex_base::operator=(other);

	TRACE_EVENT("-_lex::_lex", TRACE_TYPE_INFORMATION);
}

_lex::~_lex(void)
{
	TRACE_EVENT("+_lex::~_lex", TRACE_TYPE_INFORMATION);
	TRACE_EVENT("-_lex::~_lex", TRACE_TYPE_INFORMATION);
}

_lex &
_lex::operator=(
	const _lex &other
	)
{
	TRACE_EVENT("+_lex::operator=", TRACE_TYPE_INFORMATION);

	if(this != &other) {
		_position = other._position;
		_tokens = other._tokens;
		memcpy(_actions, other._actions, sizeof(lex_action) * (MAX_LEX_ACTION_TYPE + 1));
		memcpy(_enum_actions, other._enum_actions, sizeof(lex_enum_action) * (MAX_LEX_ENUM_ACTION_TYPE + 1));
		lex_base::operator=(other);
	}

	TRACE_EVENT("-_lex::operator=", TRACE_TYPE_INFORMATION);

	return *this;
}

void 
_lex::_invoke_action(
	size_t type
	)
{
	TRACE_EVENT("+_lex::_invoke_action", TRACE_TYPE_INFORMATION);

	if(type <= MAX_LEX_ACTION_TYPE) {

		if(_actions[type]) {
			_actions[type](this);
		} else {
			TRACE_EVENT("Lexer action is unallocated: " << LEX_ACTION_STRING(type), TRACE_TYPE_ERROR);
			THROW_LEX_EXC_W_MESS(LEX_ACTION_STRING(type), LEX_EXC_UNALLOC_ACTION);
		}
	} else {
		TRACE_EVENT("Invocation of invalid lexer action: " << type, TRACE_TYPE_ERROR);
		THROW_LEX_EXC_W_MESS(LEX_ACTION_STRING(type), LEX_EXC_INVAL_ACTION_TYPE);
	}

	TRACE_EVENT("-_lex::_invoke_action", TRACE_TYPE_INFORMATION);
}

void 
_lex::_invoke_enumeration_action(
	_tok &token,
	size_t type
	)
{
	TRACE_EVENT("+_lex::_invoke_enumeration_action", TRACE_TYPE_INFORMATION);

	if(type <= MAX_LEX_ENUM_ACTION_TYPE) {

		if(_enum_actions[type]) {
			_enum_actions[type](this, token);
		} else {
			TRACE_EVENT("Lexer enumeration action is unallocated: " << LEX_ENUM_ACTION_STRING(type), TRACE_TYPE_ERROR);
			THROW_LEX_EXC_W_MESS(LEX_ENUM_ACTION_STRING(type), LEX_EXC_UNALLOC_ENUM_ACTION);
		}
	} else {
		TRACE_EVENT("Invocation of invalid lexer enumeration action: " << type, TRACE_TYPE_ERROR);
		THROW_LEX_EXC_W_MESS(LEX_ENUM_ACTION_STRING(type), LEX_EXC_INVAL_ENUM_ACTION_TYPE);
	}

	TRACE_EVENT("-_lex::_invoke_enumeration_action", TRACE_TYPE_INFORMATION);
}

void 
_lex::clear(void)
{
	TRACE_EVENT("+_lex::clear", TRACE_TYPE_INFORMATION);

	_position = 0;
	_tokens.clear();
	_tokens.push_back(tok(CLASS_TOKEN_TYPE_BEGIN));
	_tokens.push_back(tok(CLASS_TOKEN_TYPE_END));
	lex_base::reset();

	TRACE_EVENT("-_lex::clear", TRACE_TYPE_INFORMATION);
}

void 
_lex::enumerate(void)
{
	TRACE_EVENT("+_lex::enumerate", TRACE_TYPE_INFORMATION);

	clear();

	while(has_next()) {
		move_next();
	}
	reset();

	TRACE_EVENT("-_lex::enumerate", TRACE_TYPE_INFORMATION);
}

_tok &
_lex::get(void)
{
	TRACE_EVENT("+_lex::get", TRACE_TYPE_INFORMATION);

	if(_position >= _tokens.size()) {
		TRACE_EVENT("Lexer token position is out of bounds! (" << _position << "/" << (!_tokens.empty() ? (_tokens.size() - 1) : 0) << ")", TRACE_TYPE_ERROR);
		THROW_LEX_EXC_W_MESS(_position << " (" << (!_tokens.empty() ? (_tokens.size() - 1) : 0) << ")", LEX_EXC_OUT_OF_BOUNDS);
	}

	TRACE_EVENT("-_lex::get", TRACE_TYPE_INFORMATION);

	return _tokens.at(_position);
}

size_t 
_lex::get_count(void)
{
	TRACE_EVENT("_lex::get_count", TRACE_TYPE_VERBOSE);

	return _tokens.size() - MIN_LEX_TOKEN_COUNT;
}
		
size_t 
_lex::get_position(void)
{
	TRACE_EVENT("_lex::get_position", TRACE_TYPE_VERBOSE);

	return _position;
}
		
bool 
_lex::has_next(void)
{
	TRACE_EVENT("_lex::has_next", TRACE_TYPE_VERBOSE);

	return get().get_class_type() != CLASS_TOKEN_TYPE_END;
}
		
bool 
_lex::has_previous(void)
{
	TRACE_EVENT("_lex::has_previous", TRACE_TYPE_VERBOSE);

	return _position > 0;
}
		
bool 
_lex::has_text(void)
{
	TRACE_EVENT("_lex::has_text", TRACE_TYPE_VERBOSE);

	return !get().get_text().empty();
}

bool 
_lex::has_tokens(void)
{
	TRACE_EVENT("_lex::has_tokens", TRACE_TYPE_VERBOSE);

	return !_tokens.empty();
}

void 
_lex::insert_class_token(
	size_t class_type
	)
{
	TRACE_EVENT("+_lex::insert_class_token", TRACE_TYPE_INFORMATION);

	if(class_type <= MAX_STMT_TOK_TYPE) {

		if(_position) {
			_tokens.insert(_tokens.begin() + _position, tok(class_type));
		} else {
			TRACE_EVENT("Attempting to insert token into an empty list!", TRACE_TYPE_ERROR);
			THROW_LEX_EXC(LEX_EXC_INSERT_INTO_EMPTY_LIST);
		}
	} else {
		TRACE_EVENT("Attempting to insert a token with an invalid class type!", TRACE_TYPE_ERROR);
		THROW_LEX_EXC_W_MESS((int) class_type, LEX_EXC_INVAL_STMT_TOK_TYPE);
	}

	TRACE_EVENT("-_lex::insert_class_token", TRACE_TYPE_INFORMATION);
}

void 
_lex::move_next(void)
{
	TRACE_EVENT("+_lex::move_next", TRACE_TYPE_INFORMATION);

	_tok token;

	_invoke_action(LEX_ACTION_SKIP_WHITESPACE);

	if(lex_base::has_next()
			&& _position == (_tokens.size() - MIN_LEX_TOKEN_COUNT)) {
		token.set_line(lex_base::get_line());
		token.set_position(lex_base::get_relative_position());

		switch(lex_base::get_base_type()) {
			case SYMBOL_BASE_TYPE_ALPHA:
				_invoke_enumeration_action(token, LEX_ENUM_ACTION_ALPHA);
				break;
			case SYMBOL_BASE_TYPE_DIGIT:
				_invoke_enumeration_action(token, LEX_ENUM_ACTION_NUMBER);
				break;
			case SYMBOL_BASE_TYPE_SYMBOL:
				_invoke_enumeration_action(token, LEX_ENUM_ACTION_SYMBOL);
				break;
			default:
				TRACE_EVENT("Encountered an invalid symbol type: " << lex_base::to_string(true), TRACE_TYPE_ERROR);
				THROW_LEX_EXC_W_MESS(lex_base::to_string(true), LEX_EXC_INVAL_CLASS_SYMBOL);
		}
		_tokens.insert(_tokens.begin() + ++_position, token);
	} else if(_position < (_tokens.size() - 1)) {
		++_position;
	} else {
		TRACE_EVENT("Lexer has no next token!", TRACE_TYPE_ERROR);
		THROW_LEX_EXC_W_MESS(_position, LEX_EXC_NO_NEXT_TOKEN);
	}

	TRACE_EVENT("-_lex::move_next", TRACE_TYPE_INFORMATION);
}
		
void 
_lex::move_previous(void)
{
	TRACE_EVENT("+_lex::move_next", TRACE_TYPE_INFORMATION);

	if(has_previous()) {
		--_position;
	} else {
		TRACE_EVENT("Lexer has no previous token!", TRACE_TYPE_ERROR);
		THROW_LEX_EXC_W_MESS(_position, LEX_EXC_NO_PREV_TOKEN);
	}

	TRACE_EVENT("-_lex::move_next", TRACE_TYPE_INFORMATION);
}

void 
_lex::reset(void)
{
	TRACE_EVENT("+_lex::reset", TRACE_TYPE_INFORMATION);

	_position = 0;

	TRACE_EVENT("-_lex::reset", TRACE_TYPE_INFORMATION);
}

void 
_lex::set_action(
	lex_action action,
	size_t type
	)
{
	TRACE_EVENT("+_lex::set_action", TRACE_TYPE_INFORMATION);

	if(type <= MAX_LEX_ACTION_TYPE) {
		_actions[type] = action;
	} else {
		TRACE_EVENT("Invalid lexer action type: " << type, TRACE_TYPE_ERROR);
		THROW_LEX_EXC_W_MESS(LEX_ACTION_STRING(type), LEX_EXC_INVAL_ACTION_TYPE);
	}

	TRACE_EVENT("-_lex::set_action", TRACE_TYPE_INFORMATION);
}

void 
_lex::set_enumeration_action(
	lex_enum_action action,
	size_t type
	)
{
	TRACE_EVENT("+_lex::set_enumeration_action", TRACE_TYPE_INFORMATION);

	if(type <= MAX_LEX_ENUM_ACTION_TYPE) {
		_enum_actions[type] = action;
	} else {
		TRACE_EVENT("Invalid lexer enumeration action type: " << type, TRACE_TYPE_ERROR);
		THROW_LEX_EXC_W_MESS(LEX_ENUM_ACTION_STRING(type), LEX_EXC_INVAL_ENUM_ACTION_TYPE);
	}

	TRACE_EVENT("-_lex::set_enumeration_action", TRACE_TYPE_INFORMATION);
}

std::string 
_lex::to_string(
	bool verbose
	)
{
	TRACE_EVENT("_lex::to_string", TRACE_TYPE_VERBOSE);

	return get().to_string(verbose);
}