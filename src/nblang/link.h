/*
 * link.h
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

#ifndef LINK_H_
#define LINK_H_

#include "link_type.h"

/*
 * Link template class
 */
template <class T>
class _link {

	public:

		/*
		 * Link constructor
		 */
		_link(void);
		
		/*
		 * Link constructor
		 * @param value link value reference
		 */
		_link(
			const T &value
			);

		/*
		 * Link constructor
		 * @param other link object reference
		 */
		_link(
			const _link<T> &other
			);

		/*
		 * Link destructor
		 */
		virtual ~_link(void);

		/*
		 * Link assignment operator
		 * @param other link object reference
		 * @return link object reference
		 */
		_link<T> &operator=(
			const _link<T> &other
			);

		/* 
		 * Clear link
		 */
		void clear(void);

		/*
		 * Clear link with value
		 * @param value link value reference
		 */
		void clear(
			const T &value
			);

		/* 
		 * Retrieve link value 
		 * @return link value reference
		 */
		T &get(void);

		/*
		 * Retrieve next link
		 * @return next link pointer
		 */
		_link<T> *get_next(void);

		/*
		 * Retrieve previous link
		 * @return previous link pointer
		 */
		_link<T> *get_previous(void);

		/*
		 * Retrieve next link status
		 * @return true if has next link, false otherwise
		 */
		bool has_next(void);

		/*
		 * Retrieve previous link status
		 * @return true if has previous link, false otherwise
		 */
		bool has_previous(void);

		/*
		 * Set next link
		 * @param next next link pointer
		 */
		void set_next(
			_link<T> *next
			);

		/*
		 * Set previous link
		 * @param next previous link pointer
		 */
		void set_previous(
			_link<T> *previous
			);

		/*
		 * Retrieve string represenation
		 * @param verbose verbose string status
		 * @return string represenation
		 */
		std::string to_string(
			bool verbose
			);

	protected:

		/*
		 * Next link
		 */
		_link<T> *_next;

		/* 
		 * Previous link
		 */
		_link<T> *_previous;

		/*
		 * Link value
		 */
		T _value;

};

template <class T>
_link<T>::_link(void)
{
	clear(void);
}
		
template <class T>
_link<T>::_link(
	const T &value
	)
{
	clear(value);
}

template <class T>
_link<T>::_link(
	const _link<T> &other
	)
{
	_next = other._next;
	_previous = other._previous;
	_value = other._value;
}

template <class T>
_link<T>::~_link(void)
{
	return;
}

template <class T> _link<T> &
_link<T>::operator=(
	const _link<T> &other
	)
{
	if(this != &other) {
		_next = other._next;
		_previous = other._previous;
		_value = other._value;
	}

	return *this;
}

template <class T> void 
_link<T>::clear(void)
{
	_next = NULL;
	_previous = NULL;
	_value.clear();
}

template <class T> void 
_link<T>::clear(
	const T &value
	)
{
	_next = NULL;
	_previous = NULL;
	_value = value;
}

template <class T> T &
_link<T>::get(void)
{
	return _value;
}

template <class T> _link<T> *
_link<T>::get_next(void)
{
	if(!_next) {
		THROW_LINK_EXC(LINK_EXC_INVALID_NEXT_NODE);
	}

	return _next;
}

template <class T> _link<T> *
_link<T>::get_previous(void)
{
	if(!_previous) {
		THROW_LINK_EXC(LINK_EXC_INVALID_PREVIOUS_NODE);
	}

	return _previous;
}

template <class T> bool 
_link<T>::has_next(void)
{
	return _next != NULL;
}

template <class T> bool 
_link<T>::has_previous(void)
{
	return _previous != NULL;
}

template <class T> void 
_link<T>::set_next(
	_link<T> *next
	)
{
	_next = next;
}

template <class T> void 
_link<T>::set_previous(
	_link<T> *previous
	)
{
	_previous = previous;
}

template <class T> std::string 
_link<T>::to_string(
	bool verbose
	)
{
	std::stringstream ss;

	ss << _value.to_string(verbose);

	if(verbose) {

		if(has_next()
				|| has_previous()) {
			ss << " (" << (has_next() ? (has_previous() ? "NEXT, " : "NEXT") : "") << (has_previous() ? "PREV" : "") << ")";
		}
	}

	return ss.str();
}

#endif