/*
 * lex_base.cpp
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

#include <fstream>
#include <sstream>
#include <stdexcept>
#include "lex_base.h"

//#define LEXER_BASE_TRACING
#ifndef LEXER_BASE_TRACING
#define TRACE_EVENT(_M_, _T_)
#else
#define TRACE_HEADER "lexer_base"
#include "trace.h"
#endif

_lex_base::_lex_base(void)
{
	TRACE_EVENT("+_lex_base::_lex_base", TRACE_TYPE_INFORMATION);

	clear();

	TRACE_EVENT("-_lex_base::_lex_base", TRACE_TYPE_INFORMATION);
}

_lex_base::_lex_base(
	const std::string &input,
	bool is_file
	)
{
	TRACE_EVENT("+_lex_base::_lex_base", TRACE_TYPE_INFORMATION);

	initialize(input, is_file);

	TRACE_EVENT("-_lex_base::_lex_base", TRACE_TYPE_INFORMATION);
}
	
_lex_base::_lex_base(
	const _lex_base &other
	) :
		_line(other._line),
		_absolute_position(other._absolute_position),
		_relative_position(other._relative_position),
		_base_type(other._base_type),
		_input(other._input)
{
	TRACE_EVENT("+_lex_base::_lex_base", TRACE_TYPE_INFORMATION);
	TRACE_EVENT("-_lex_base::_lex_base", TRACE_TYPE_INFORMATION);
}
	
_lex_base::~_lex_base(void)
{
	TRACE_EVENT("+_lex_base::~_lex_base", TRACE_TYPE_INFORMATION);
	TRACE_EVENT("-_lex_base::~_lex_base", TRACE_TYPE_INFORMATION);
}

_lex_base &
_lex_base::operator=(
	const _lex_base &other
	)
{
	TRACE_EVENT("+_lex_base::operator=", TRACE_TYPE_INFORMATION);

	if(this != &other) {
		_line = other._line;
		_absolute_position = other._absolute_position;
		_relative_position = other._relative_position;
		_base_type = other._base_type;
		_input = other._input;
	}
	
	TRACE_EVENT("-_lex_base::operator=", TRACE_TYPE_INFORMATION);

	return *this;
}
	
void 
_lex_base::clear(void)
{
	TRACE_EVENT("+_lex_base::clear", TRACE_TYPE_INFORMATION);

	_line = 0;
	_absolute_position = 0;
	_relative_position = 0;
	_base_type = INVALID_TYPE;
	_input.clear();

	TRACE_EVENT("-_lex_base::clear", TRACE_TYPE_INFORMATION);
}

char &
_lex_base::get(void)
{
	TRACE_EVENT("+_lex_base::get", TRACE_TYPE_INFORMATION);

	if((size_t) _absolute_position >= _input.size()) {
		TRACE_EVENT("Lexer base symbol position is out of bounds! (" << _absolute_position << "/" << (_input.size() - 1) << ")", TRACE_TYPE_ERROR);
		THROW_LEX_BASE_EXC_W_MESS(_absolute_position << " (" << (_input.size() - 1) << ")", LEX_BASE_EXC_OUT_OF_BOUNDS);
	}

	TRACE_EVENT("-_lex_base::get", TRACE_TYPE_INFORMATION);

	return _input.at((size_t) _absolute_position);
}

size_t 
_lex_base::get_absolute_position(void)
{
	TRACE_EVENT("_lex_base::get_absolute_position", TRACE_TYPE_VERBOSE);

	return (size_t) _absolute_position;
}

size_t 
_lex_base::get_base_type(void)
{
	TRACE_EVENT("_lex_base::get_base_type", TRACE_TYPE_VERBOSE);

	return _base_type;
}

size_t 
_lex_base::get_count(void)
{
	TRACE_EVENT("_lex_base::get_count", TRACE_TYPE_VERBOSE);

	return _input.size();
}

size_t 
_lex_base::get_line(void)
{
	TRACE_EVENT("_lex_base::get_line", TRACE_TYPE_VERBOSE);

	return _line;
}

size_t 
_lex_base::get_relative_position(void)
{
	TRACE_EVENT("_lex_base::get_relative_position", TRACE_TYPE_VERBOSE);

	return (size_t) _relative_position;
}

bool 
_lex_base::has_next(void)
{
	TRACE_EVENT("_lex_base::has_next", TRACE_TYPE_VERBOSE);

	return get() != WS_ENDSTREAM;
}

bool 
_lex_base::has_previous(void)
{
	TRACE_EVENT("_lex_base::has_previous", TRACE_TYPE_VERBOSE);

	return _absolute_position > 0;
}

void 
_lex_base::initialize(
	const std::string &input,
	bool is_file
	)
{
	TRACE_EVENT("+_lex_base::initialize", TRACE_TYPE_INFORMATION);

	std::stringstream ss;
	
	clear();

	if(is_file) {
		std::ifstream file(input.c_str(), std::ios::in);
		
		if(file) {
			ss << file.rdbuf();
			_input = ss.str();
			file.close();
		} else {
			TRACE_EVENT("Lexer base could not find specified file: \'" << input << "\'", TRACE_TYPE_ERROR);
			THROW_LEX_BASE_EXC_W_MESS("\'" << input << "\'", LEX_BASE_EXC_FILE_NOT_FOUND);
		}
	} else {
		_input = input;
	}
	_input += WS_ENDSTREAM;
	reset();

	TRACE_EVENT("-_lex_base::initialize", TRACE_TYPE_INFORMATION);
}

void 
_lex_base::move_next(void)
{
	TRACE_EVENT("+_lex_base::move_next", TRACE_TYPE_INFORMATION);

	if(has_next()) {
		
		if(get() == WS_NEWLINE) {
			++_line;
			_relative_position = 0;
		} else {
			++_relative_position;
		}
		++_absolute_position;
		_base_type = DETERMINE_SYMBOL_BASE_TYPE(get());
	} else {
		TRACE_EVENT("Lexer base has no next symbol!", TRACE_TYPE_ERROR);
		THROW_LEX_BASE_EXC_W_MESS(_absolute_position, LEX_BASE_EXC_NO_NEXT_SYMBOL);
	}

	TRACE_EVENT("-_lex_base::move_next", TRACE_TYPE_INFORMATION);
}

void 
_lex_base::move_previous(void)
{
	TRACE_EVENT("+_lex_base::move_previous", TRACE_TYPE_INFORMATION);

	long long tmp_position;

	if(has_previous()) {
		--_absolute_position;
		--_relative_position;

		if(get() == WS_NEWLINE) {
			--_line;
			tmp_position = _absolute_position - 1;

			while(tmp_position >= 0
					&& _input.at((size_t) tmp_position) != WS_NEWLINE) {
				--tmp_position;
			}
			_relative_position = _absolute_position - tmp_position - 1;
		}
		_base_type = DETERMINE_SYMBOL_BASE_TYPE(get());
	} else {
		TRACE_EVENT("Lexer base has no previous symbol!", TRACE_TYPE_ERROR);
		THROW_LEX_BASE_EXC_W_MESS(_absolute_position, LEX_BASE_EXC_NO_PREV_SYMBOL);
	}

	TRACE_EVENT("-_lex_base::move_previous", TRACE_TYPE_INFORMATION);
}

void 
_lex_base::reset(void)
{
	TRACE_EVENT("+_lex_base::reset", TRACE_TYPE_INFORMATION);

	_line = 1;
	_absolute_position = 0;

	if(_input.empty()) {
		_base_type = INVALID_TYPE;
	} else {
		_base_type = DETERMINE_SYMBOL_BASE_TYPE(get());
	}

	TRACE_EVENT("-_lex_base::reset", TRACE_TYPE_INFORMATION);
}

std::string 
_lex_base::to_string(
	bool verbose
	)
{
	TRACE_EVENT("_lex_base::to_string", TRACE_TYPE_VERBOSE);

	std::stringstream ss;
	
	if(verbose) {
		ss << "[" << SYMBOL_BASE_TYPE_STRING(_base_type) << "] (ln. " << _line << ", pos. " << _relative_position << ") ";
	}

	switch(_base_type) {
		case SYMBOL_BASE_TYPE_ALPHA:
		case SYMBOL_BASE_TYPE_DIGIT:
		case SYMBOL_BASE_TYPE_SYMBOL:
			ss << (char) get();
			break;
		case SYMBOL_BASE_TYPE_WHITESPACE:
			
			switch(get()) {
				case WS_CARAGE_RETURN:
					ss << "CARAGE RETURN";
					break;
				case WS_ENDSTREAM:
					ss << "END STREAM";
					break;
				case WS_FEED:
					ss << "FEED";
					break;
				case WS_HORIZONTAL_TAB:
					ss << "HORIZONTAL TAB";
					break;
				case WS_NEWLINE:
					ss << "NEWLINE";
					break;
				case WS_SPACE:
					ss << "SPACE";
					break;
				case WS_VERTICAL_TAB:
					ss << "VERTICAL TAB";
					break;
				default:
					ss << "NON-WHITESPACE (" << get() << ")";
					break;
			}
			break;
		default:
			ss << "UNKNOWN (" << get() << ")";
			break;
	}

	return ss.str();
}
