/*
 * list.h
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

#ifndef LIST_H_
#define LIST_H_

#include "link.h"
#include "list_type.h"

/*
 * List template class
 */
template <class T>
class _list {

	public:

		/*
		 * List constructor
		 */
		_list(void);

		/*
		 * List constructor
		 * @param value list value reference
		 */
		_list(
			const T &value
			);

		/*
		 * List constructor
		 * @param other list object reference
		 */
		_list(
			const _list<T> &other
			);

		/*
		 * List destructor
		 */
		virtual ~_list(void);

		/*
		 * List assignment operator
		 * @param other list object reference
		 * @return list object reference
		 */
		_list<T> &operator=(
			const _list<T> &other
			);

		/*
		 * Add list value to the end of list
		 * @param value list value reference
		 */
		void add_back(
			const T &value
			);

		/*
		 * Add list value to the begining of list
		 * @param value list value reference
		 */
		void add_front(
			const T &value
			);

		/*
		 * Clear list
		 */
		void clear(void);

		/*
		 * Clear list with value
		 * @param value list value reference
		 */
		void clear(
			const T &value
			);

		/*
		 * Retrieve list value at a given index
		 * @param index list value index
		 * @return list value reference
		 */
		T &get(
			size_t index
			);

		/*
		 * Retrieve link at a given index
		 * @param index link object index
		 * @return link object pointer
		 */
		_link<T> *get_link(
			size_t index
			);

		/*
		 * Retrieve list value at end of list
		 * @param index list value index
		 * @return list value reference
		 */
		T &get_back(
			size_t index
			);

		/*
		 * Retrieve link at end of list
		 * @return link object pointer
		 */
		_link<T> *get_back_link(void);

		/*
		 * Retrieve list value at begining of list
		 * @param index list value index
		 * @return list value reference
		 */
		T &get_front(
			size_t index
			);

		/*
		 * Retrieve link at begining of list
		 * @return link object pointer
		 */
		_link<T> *get_front_link(void);

		/*
		 * Retrieve link count
		 * @return link count
		 */
		size_t get_link_count(void);

		/*
		 * Retrieve link count status
		 * @return true if not empty, false otherwise
		 */
		bool has_links(void);

		/* 
		 * Insert link value at a given index
		 * @param value link value reference
		 * @param index link index
		 */
		void insert(
			const T &value,
			size_t index
			);

		/*
		 * Remove link at a given index
		 * @param index link index
		 */
		void remove(
			size_t index
			);

		/*
		 * Remove all links
		 */
		void remove_all(void);

		/*
		 * Remove link at end of list
		 */
		void remove_back(void);

		/*
		 * Remove link at begining of list
		 */
		void remove_front(void);

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
		 * Copy list from link
		 * @param front link to copy from
		 */
		void _copy_helper(
			_link<T> *front
			);

		/*
		 * List back link
		 */
		_link<T> *_back;

		/*
		 * List front link
		 */
		_link<T> *_front;

		/*
		 * List size
		 */
		size_t _size;

};

template <class T>
_list<T>::_list(void)
{
	_back = NULL;
	_front = NULL;
	_size = 0;
	clear();
}

template <class T>
_list<T>::_list(
	const T &value
	)
{
	_back = NULL;
	_front = NULL;
	_size = 0;
	clear(value);
}

template <class T>
_list<T>::_list(
	const _list<T> &other
	)
{
	_back = NULL;
	_front = NULL;
	_size = 0;
	clear();
	_copy_helper(other._front);
}

template <class T>
_list<T>::~_list(void)
{
	remove_all();
}

template <class T> _list<T> & 
_list<T>::operator=(
	const _list<T> &other
	)
{
	if(this != &other) {
		clear();
		_copy_helper(other._front);
	}

	return *this;
}

template <class T> void 
_list<T>::_copy_helper(
	_link<T> *front
	)
{
	_link<T> *link = front, *new_link = NULL, *prev_link = NULL;

	_back = NULL;
	_front = NULL;
	_size = 0;

	while(link) {
		new_link = new _link<T>(*front);

		if(!new_link) {
			THROW_LIST_EXC(LIST_EXC_OUT_OF_MEMORY);
		}

		if(!_size) {
			_front = new_link;
		}

		if(prev_link) {
			prev_link->set_next(new_link);
			new_link->set_previous(prev_link);
		} else {
			new_link->set_previous(NULL);
		}
		new_link->set_next(NULL);
		link = link->get_next();
		prev_link = new_link;
		++_size;
	}
	_back = new_link;
}

template <class T> void 
_list<T>::add_back(
	const T &value
	)
{
	insert(value, _size);
}

template <class T> void 
_list<T>::add_front(
	const T &value
	)
{
	insert(value, 0);
}

template <class T> void 
_list<T>::clear(void)
{
	remove_all();
	_back = NULL;
	_front = NULL;
	_size = 0;
}

template <class T> void 
_list<T>::clear(
	const T &value
	)
{
	clear();
	add_back(value);
}

template <class T> T &
_list<T>::get(
	size_t index
	)
{
	return get_link(index)->get();
}

template <class T> _link<T> *
_list<T>::get_link(
	size_t index
	)
{
	size_t position = 0;
	_link<T> *link = _front;

	if(index >= _size) {
		THROW_LIST_EXC_W_MESS(index, LIST_EXC_INVALID_LINK_INDEX);
	}

	while(position++ < index) {
		link = link->get_next();
	}

	return link;
}

template <class T> T &
_list<T>::get_back(
	size_t index
	)
{
	return get_back_link(index)->get();
}

template <class T> _link<T> *
_list<T>::get_back_link(void)
{
	if(!_back) {
		THROW_LIST_EXC(LIST_EXC_INVALID_BACK_LINK);
	}

	return _back;
}

template <class T> T &
_list<T>::get_front(
	size_t index
	)
{
	return get_front_link(index)->get();
}

template <class T> _link<T> *
_list<T>::get_front_link(void)
{
	if(!_front) {
		THROW_LIST_EXC(LIST_EXC_INVALID_FRONT_LINK);
	}

	return _front;
}

template <class T> size_t 
_list<T>::get_link_count(void)
{
	return _size;
}

template <class T> bool 
_list<T>::has_links(void)
{
	return _size > 0;
}

template <class T> void 
_list<T>::insert(
	const T &value,
	size_t index
	)
{
	size_t position = 0;
	_link<T> *link = _front, *new_link = NULL;

	if(index > _size) {
		THROW_LIST_EXC_W_MESS(index, LIST_EXC_INVALID_LINK_INDEX);
	}
	new_link = new _link<T>(value);

	if(!new_link) {
		THROW_LIST_EXC(LIST_EXC_OUT_OF_MEMORY);
	}

	if(!index) {

		if(!link) {
			new_link->set_next(NULL);
			new_link->set_previous(NULL);
			_back = new_link;
			_front = new_link;
		} else {
			new_link->set_next(_front);
			new_link->set_previous(NULL);
			_front->set_previous(new_link);
			_front = new_link;
		}
	} else if(index < _size) {

		while(++position < index) {

			if(!link->has_next()) {
				THROW_LIST_EXC_W_MESS(index, LIST_EXC_INVALID_LINK_INDEX);
			}
			link = link->get_next();
		}
		new_link->set_next(link->get_next());
		new_link->set_previous(link);
		link->get_next()->set_previous(new_link);
		link->set_next(new_link);
	} else {
		new_link->set_next(NULL);
		new_link->set_previous(_back);
		_back->set_next(new_link);
		_back = new_link;
	}
	++_size;
}

template <class T> void 
_list<T>::remove(
	size_t index
	)
{
	size_t position = 0;
	_link<T> *link = _front, *old_link = NULL;

	if(index > _size) {
		THROW_LIST_EXC_W_MESS(index, LIST_EXC_INVALID_LINK_INDEX);
	}

	if(link) {

		if(!index) {
			old_link = _front;

			if(_front->has_next()) {
				_front = _front->get_next();
				_front->set_previous(NULL);
			} else {
				_front = NULL;
			}

			if(_size == 1) {
				_back = NULL;
			}
		} else if(index < _size) {

			while(position++ < index) {

				if(!link->has_next()) {
					THROW_LIST_EXC_W_MESS(index, LIST_EXC_INVALID_LINK_INDEX);
				}
				link = link->get_next();
			}
		
			if(link->has_next()) {
				old_link = link->get_next();
			}

			if(old_link->has_next()) {
				link->set_next(old_link->get_next());
				link->get_next()->set_previous(link);
			} else {
				link->set_next(NULL);
			}
		} else {
			old_link = _back;

			if(_back->has_previous()) {
				_back = _back->get_previous();
				_back->set_next(NULL);
			} else {
				_back = NULL;
			}

			if(_size == 1) {
				_front = NULL;
			}
		}
	}

	if(old_link) {
		delete old_link;
		--_size;
	}
}

template <class T> void 
_list<T>::remove_all(void)
{
	while(has_links()) {
		remove_front();
	}
}

template <class T> void 
_list<T>::remove_back(void)
{
	remove(_size);
}

template <class T> void 
_list<T>::remove_front(void)
{
	remove(0);
}

template <class T> std::string 
_list<T>::to_string(
	bool verbose
	)
{
	size_t i = 0;
	std::stringstream ss;
	_link<T> *link = _front;

	if(_size) {

		for(; i < _size; ++i) {
			ss << link->to_string(verbose) << (link == _front ? " (FRONT)" : "") << (link == _back ? " (BACK)" : "") << std::endl << std::endl;

			if(!link->has_next()) {
				break;
			}
			link = link->get_next();
		}
	} else {
		ss << "Empty list" << std::endl;
	}

	return ss.str();
}

#endif