/*
 * cont.cpp
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
#include "cont.h"
#include "cont_type.h"

_cont::_cont(void)
{
	clear();
}

_cont::_cont(
	const _cont &other
	) :
		_values(other._values)
{
	return;
}

_cont::~_cont(void)
{
	return;
}

_cont &
_cont::operator=(
	const _cont &other
	)
{
	if(this != &other) {
		_values = other._values;
	}

	return *this;
}

void 
_cont::clear(void)
{
	_values.clear();
}

tok_stmt_vector &
_cont::get_value(
	const std::string &key
	)
{
	if(_values.find(key) == _values.end()) {
		THROW_CONT_EXC_W_MESS(key, CONT_EXC_INVALID_KEY);
	}

	return _values.find(key)->second;
}

size_t 
_cont::get_value_count(void)
{
	return _values.size();
}

bool 
_cont::has_value(
	const std::string &key
	)
{
	return _values.find(key) != _values.end();
}

void 
_cont::remove_value(
	const std::string &key
	)
{
	std::map<std::string, tok_stmt_vector>::iterator entry = _values.find(key);

	if(entry == _values.end()) {
		THROW_CONT_EXC_W_MESS(key, CONT_EXC_INVALID_KEY);
	}
	_values.erase(entry);
}

void 
_cont::set_value(
	const std::string &key,
	const tok_stmt &value
	)
{
	set_value(key, value, 0);
}

void 
_cont::set_value(
	const std::string &key,
	const tok_stmt &value,
	size_t index
	)
{
	tok_stmt_vector value_list;

	if(_values.find(key) == _values.end()) {

		if(index) {
			THROW_CONT_EXC_W_MESS(key << "[" << index << "]", CONT_EXC_INVALID_INDEX);
		}
		value_list.push_back(value);
		_values.insert(std::pair<std::string, tok_stmt_vector>(key, value_list));
	} else {

		if(index < _values.at(key).size()) {
			_values.at(key).at(index) = value;
		} else if(index == _values.at(key).size()) {
			_values.at(key).push_back(value);
		} else {
			THROW_CONT_EXC_W_MESS(key << "[" << index << "]", CONT_EXC_INVALID_INDEX);
		}
	}
}

void 
_cont::set_value(
	const std::string &key,
	const tok_stmt_vector &value
	)
{
	if(_values.find(key) == _values.end()) {
		_values.insert(std::pair<std::string, tok_stmt_vector>(key, value));
	} else {
		_values.at(key) = value;
	}
}

std::string 
_cont::to_string(
	bool verbose
	)
{
	std::stringstream ss;
	tok_stmt_vector::iterator child;
	std::map<std::string, tok_stmt_vector>::iterator entry = _values.begin();

	if(!_values.empty()) {
		ss << entry->first << " (" << entry->second.size() << ")\t-->\n";

		for(; entry != _values.end(); ++entry) {

			for(child = entry->second.begin(); child != entry->second.end(); ++ child) {
				ss << child->to_string(verbose) << std::endl;
			}
		}
	} else {
		ss << "Empty context";
	}

	return ss.str();
}