/*
 * node.h
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

#ifndef NODE_H_
#define NODE_H_

#include <sstream>
#include <stdexcept>
#include <vector>
#include "node_type.h"

/*
 * Node template class
 */
template <class T> 
class _node {

	public:

		/*
		 * Node constructor
		 */
		_node(void);

		/*
		 * Node constructor
		 * @param value node value reference
		 */
		_node(
			const T &value
			);

		/*
		 * Node constructor
		 * @param other node object reference
		 */
		_node(
			const _node<T> &other
			);

		/*
		 * Node destructor
		 */
		virtual ~_node(void);

		/*
		 * Node assignment operator
		 * @param other node object reference
		 * @return node object reference
		 */
		_node<T> &operator=(
			const _node<T> &other
			);

		/*
		 * Add child to the end of node children
		 * @param child child node object reference
		 */
		void add_child_back(
			_node<T> *child
			);

		/*
		 * Add child to the begining of node children
		 * @param child child node object reference
		 */
		void add_child_front(
			_node<T> *child
			);

		/*
		 * Clear node
		 */
		void clear(void);

		/*
		 * Clear node with value
		 * @param value node value reference
		 */
		void clear(
			const T &value
			);

		/*
		 * Retrieve node value
		 * @return node value reference
		 */
		T &get(void);

		/*
		 * Retrieve child node at a given index
		 * @param index child node index
		 * @return child node pointer
		 */
		_node<T> *get_child(
			size_t index
			);

		/* 
		 * Retrieve node children begin iterator
		 * @return node children begin iterator
		 */
		typename std::vector<_node<T> *>::iterator get_child_begin(void);

		/* 
		 * Retrieve child node count
		 * @return child node count
		 */
		size_t get_child_count(void);

		/* 
		 * Retrieve node children end iterator
		 * @return node children end iterator
		 */
		typename std::vector<_node<T> *>::iterator get_child_end(void);

		/* 
		 * Retrieve parent node
		 * @return parent node pointer
		 */
		_node<T> *get_parent(void);

		/*
		 * Retrieve parent node status
		 * @return true if has parent, false otherwise
		 */
		bool has_parent(void);

		/*
		 * Retrieve children node status
		 * @return true if has children, false otherwise
		 */
		bool has_children(void);

		/*
		 * Insert child into node at a given index
		 * @param child child node pointer
		 * @param index child index
		 */
		void insert_child(
			_node<T> *child,
			size_t index
			);

		/*
		 * Remove child from node at a given index
		 * @param index child index
		 * @return child node pointer
		 */
		_node<T> *remove_child(
			size_t index
			);

		/*
		 * Remove all children from current position node
		 */
		void remove_children(void);

		/*
		 * Set child node at a given index
		 * @param child child node pointer
		 * @param index child node index
		 * @return child node pointer
		 */
		_node<T> *set_child(
			_node<T> *child,
			size_t index
			);

		/*
		 * Set node parent
		 * @param parent parent node pointer
		 * @return parent node pointer
		 */
		_node<T> *set_parent(
			_node<T> *parent
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
		 * Node children
		 */
		std::vector<_node<T> *> _children;

		/*
		 * Node parent
		 */
		_node<T> *_parent;

		/* 
		 * Node value
		 */
		T _value;

};

template <class T> 
_node<T>::_node(void)
{
	clear();
};

template <class T> 
_node<T>::_node(
	const T &value
	)
{
	clear(value);
}

template <class T> 
_node<T>::_node(
	const _node<T> &other
	) :
		_children(other._children),
		_parent(other._parent),
		_value(other._value)
{
	return;
};

template <class T> 
_node<T>::~_node(void)
{
	return;
};

template <class T> _node<T> &
_node<T>::operator=(
	const _node<T> &other
	)
{
	if(this != &other) {
		_children = other._children;
		_parent = other._parent;
		_value = other._value;
	}

	return *this;
}

template <class T> void 
_node<T>::add_child_back(
	_node<T> *child
	)
{
	insert_child(child, get_child_count());
}

template <class T> void 
_node<T>::add_child_front(
	_node<T> *child
	)
{
	insert_child(child, 0);
}

template <class T> void 
_node<T>::clear(void)
{
	_children.clear();
	_parent = NULL;
	_value.clear();
};

template <class T> void 
_node<T>::clear(
	const T &value
	)
{
	_children.clear();
	_parent = NULL;
	_value = value;
}

template <class T> T &
_node<T>::get(void)
{
	return _value;
};

template <class T> _node<T> *
_node<T>::get_child(
	size_t index
	)
{
	if(index >= _children.size()) {
		THROW_NODE_EXC_W_MESS(index, NODE_EXC_INVALID_CHILD_NODE_INDEX);
	}

	return _children.at(index);
};

template <class T> typename std::vector<_node<T> *>::iterator 
_node<T>::get_child_begin(void)
{
	return _children.begin();
}

template <class T> size_t 
_node<T>::get_child_count(void)
{
	return _children.size();
};

template <class T> typename std::vector<_node<T> *>::iterator 
_node<T>::get_child_end(void)
{
	return _children.end();
}

template <class T> _node<T> *
_node<T>::get_parent(void)
{
	if(!_parent) {
		THROW_NODE_EXC(NODE_EXC_INVALID_PARENT_NODE);
	}

	return _parent;
};

template <class T> bool 
_node<T>::has_parent(void)
{
	return _parent != NULL;
};

template <class T> bool 
_node<T>::has_children(void)
{
	return !_children.empty();
};

template <class T> void 
_node<T>::insert_child(
	_node<T> *child,
	size_t index
	)
{
	if(index < _children.size()) {
		_children.insert(_children.begin() + index, child);
		_children.at(index)->set_parent(this);
	} else if(index == _children.size()) {
		_children.push_back(child);
		_children.back()->set_parent(this);
	} else {
		THROW_NODE_EXC_W_MESS(index, NODE_EXC_INVALID_CHILD_NODE_INDEX);
	}
};

template <class T> _node<T> *
_node<T>::remove_child(
	size_t index
	)
{
	_node<T> *old_child = NULL;

	if(index >= _children.size()) {
		THROW_NODE_EXC_W_MESS(index, NODE_EXC_INVALID_CHILD_NODE_INDEX);
	}
	old_child = _children.at(index);
	_children.erase(_children.begin() + index);

	return old_child;
};

template <class T> void 
_node<T>::remove_children(void)
{
	_children.clear();
};

template <class T> _node<T> *
_node<T>::set_child(
	_node<T> *child,
	size_t index
	)
{
	_node<T> *old_child = NULL;

	if(index >= _children.size()) {
		THROW_NODE_EXC_W_MESS(index, NODE_EXC_INVALID_CHILD_NODE_INDEX);
	}
	old_child = _children.at(index);
	_children.at(index) = child;

	return old_child;
};

template <class T> _node<T> *
_node<T>::set_parent(
	_node<T> *parent
	)
{
	_node<T> *old_parent = _parent;

	_parent = parent;

	return old_parent;
};

template <class T> std::string 
_node<T>::to_string(
	bool verbose
	)
{
	std::stringstream ss;

	ss << _value.to_string(verbose);

	if(verbose) {

		if(!has_parent()) {
			ss << " (ROOT)";
		}

		if(has_children()) {
			ss << " (" << _children.size() << ")";
		} else {
			ss << " (LEAF)";
		}		
	}

	return ss.str();
};

#endif