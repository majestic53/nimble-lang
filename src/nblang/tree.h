/*
 * tree.h
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

#ifndef TREE_H_
#define TREE_H_

#include <queue>
#include <sstream>
#include <stdexcept>
#include "node.h"
#include "tree_type.h"

/*
 * Tree template class
 */
template <class T> 
class _tree {

	public:

		/*
		 * Tree constructor
		 */
		_tree(void);

		/*
		 * Tree constructor
		 * @param value root node value reference
		 */
		_tree(
			const T &value
			);

		/*
		 * Tree constructor
		 * @param tree object reference
		 */
		_tree(
			const _tree<T> &other
			);

		/*
		 * Tree constructor
		 * @param tree object reference
		 * @param as_subtree true if append to current position node, false as root
		 */
		_tree(
			const _tree<T> &other,
			bool as_subtree
			);

		/*
		 * Tree destructor
		 */
		virtual ~_tree(void);

		/*
		 * Tree assignment operator
		 * @param tree object reference
		 * @return tree object reference
		 */
		_tree<T> &operator=(
			const _tree<T> &other
			);

		/*
		 * Add child to the end of current position node children
		 * @param child child object reference
		 */
		void add_child_back(
			const T &child
			);

		/*
		 * Add child to the begining of current position node children
		 * @param child child object reference
		 */
		void add_child_front(
			const T &child
			);

		/*
		 * Clear tree
		 */
		void clear(void);

		/*
		 * Clear tree with a given value
		 */
		void clear(
			const T &value
			);

		/* 
		 * Retrieve current position node object reference
		 * @return current position node object reference
		 */
		T &get(void);

		/* 
		 * Retrieve current position node container object reference
		 * @return current position node container object reference
		 */
		_node<T> *get_position_node(void);

		/*
		 * Retrieve current position node object child reference at a given index
		 * @return current position node object child reference at a given index
		 */
		T &get_child(
			size_t index
			);

		/* 
		 * Retrieve current position node object child node count
		 * @return current position node object child node count
		 */
		size_t get_child_count(void);

		/*
		 * Retrieve current position node container object pointer child at a given index
		 * @return current position node container object pointer child at a given index
		 */
		_node<T> *get_child_node(
			size_t index
			);

		/*
		 * Retrieve root node object reference
		 * @return root node object reference
		 */
		T &get_root(void);

		/*
		 * Retrieve root node container object pointer child at a given index
		 * @return root node container object pointer child at a given index
		 */
		_node<T> *get_root_node(void);

		/*
		 * Retrieve current position node child status
		 * @return true if not empty, false otherwise
		 */
		bool has_children(void);

		/*
		 * Retrieve current position node parent status
		 * @return true if has parent, false otherwise
		 */
		bool has_parent(void);

		/*
		 * Insert child into current position node at a given index
		 * @param child child object reference
		 * @param index child index
		 */
		void insert_child(
			const T &child, 
			size_t index
			);

		/*
		 * Insert child into current position node at a given index
		 * @param child child node object pointer
		 * @param index child index
		 */
		void insert_child_node(
			_node<T> *child, 
			size_t index
			);

		/*
		 * Move current position node to a child at a given index
		 * @param index child index
		 */
		void move_child(
			size_t index
			);

		/*
		 * Move current position node to the last child
		 */
		void move_child_back(void);

		/*
		 * Move current position node to the first child
		 */
		void move_child_front(void);

		/*
		 * Move current position node to the parent
		 */
		void move_parent(void);

		/*
		 * Move current position node to the root
		 */
		void move_root(void);

		/*
		 * Remove child from current position node at a given index
		 * @param index child index
		 */
		void remove_child(
			size_t index
			);

		/*
		 * Remove all children from current position node
		 */
		void remove_children(void);

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
		 * Root node
		 */
		_node<T> *_root;

		/*
		 * Current position node
		 */
		_node<T> *_position;

		/*
		 * Clear all nodes from a given parent node
		 * @param parent parent node object pointer pointer
		 */
		void _clear_helper(
			_node<T> **parent
			);

		/*
		 * Copy all nodes from a given parent node into another node
		 * @param parent parent node object pointer
		 * @param other other node object pointer
		 * @return NULL on failure
		 */
		_node<T> * _copy_helper(
			_node<T> *parent,
			_node<T> *other
			);

		/* 
		 * Retrieve current position node
		 * @return current position node pointer
		 */
		_node<T> *_get_position(void);

		/*
		 * Retrieve root node
		 * @return root node pointer
		 */
		_node<T> *_get_root(void);

};

template <class T>
_tree<T>::_tree(void)
{
	_root = NULL;
	_position = NULL;
	clear();
};

template <class T>
_tree<T>::_tree(
	const T &value
	)
{
	_root = NULL;
	_position = NULL;
	clear(value);
}

template <class T>
_tree<T>::_tree(
	const _tree<T> &other
	)
{
	_root = NULL;
	_position = NULL;
	_clear_helper(&_root);
	_root = _copy_helper(NULL, other._root);
	_position = _root;
}

template <class T>
_tree<T>::_tree(
	const _tree<T> &other,
	bool as_subtree
	)
{
	_root = NULL;
	_position = NULL;
	_clear_helper(&_root);
	_root = _copy_helper(NULL, as_subtree ? other._position : other._root);
	_position = _root;
};

template <class T>
_tree<T>::~_tree(void)
{
	_clear_helper(&_root);
	_root = NULL;
	_position = NULL;
};

template <class T> _tree<T> &
_tree<T>::operator=(
	const _tree<T> &other
	)
{
	if(this != &other) {
		_clear_helper(&_root);
		_root = _copy_helper(NULL, other._root);
		_position = _root;
	}

	return *this;
};

template <class T> void 
_tree<T>::_clear_helper(
	_node<T> **parent
	)
{
	std::vector<_node<T> *>::iterator child;

	if(*parent) {

		for(child = (*parent)->get_child_begin(); child != (*parent)->get_child_end(); ++child) {
			_clear_helper(&*child);
		}
		delete *parent;
		*parent = NULL;
	}
}

template <class T> _node<T> * 
_tree<T>::_copy_helper(
	_node<T> *parent,
	_node<T> *other
	)
{
	_node<T> *entry = NULL;
	std::vector<_node<T> *>::iterator child;

	if(other) {
		entry = new _node<T>(*other);
		
		if(!entry) {
			THROW_TREE_EXC(TREE_EXC_OUT_OF_MEMORY);
		}
		entry->set_parent(parent);
		entry->remove_children();

		for(child = other->get_child_begin(); child != other->get_child_end(); ++child) {
			entry->insert_child(_copy_helper(entry, *child), entry->get_child_count());
		}
	}

	return entry;
}

template <class T> _node<T> *
_tree<T>::_get_position(void)
{
	if(!_position) {
		THROW_TREE_EXC(TREE_EXC_INVAL_POS_PTR);
	}

	return _position;
}

template <class T> _node<T> *
_tree<T>::_get_root(void)
{
	if(!_root) {
		THROW_TREE_EXC(TREE_EXC_INVAL_ROOT_PTR);
	}

	return _root;
}

template <class T> void
_tree<T>::add_child_back(
	const T &child
	)
{
	insert_child(child, get_child_count());
}

template <class T> void 
_tree<T>::add_child_front(
	const T &child
	)
{
	insert_child(child, 0);
}

template <class T> void 
_tree<T>::clear(void)
{
	_clear_helper(&_root);
	_root = new _node<T>;

	if(!_root) {
		THROW_TREE_EXC(TREE_EXC_OUT_OF_MEMORY);
	}
	_position = _root;
};

template <class T> void 
_tree<T>::clear(
	const T &value
	)
{
	_clear_helper(&_root);
	_root = new _node<T>(value);

	if(!_root) {
		THROW_TREE_EXC(TREE_EXC_OUT_OF_MEMORY);
	}
	_position = _root;
};

template <class T> T &
_tree<T>::get(void)
{
	return _position->get();
}

template <class T> _node<T> *
_tree<T>::get_position_node(void)
{
	return _position;
}

template <class T> T &
_tree<T>::get_child(
	size_t index
	)
{
	_node<T> *child = _get_position()->get_child(index);

	return child->get();
};

template <class T> size_t 
_tree<T>::get_child_count(void)
{
	return _get_position()->get_child_count();
};

template <class T> _node<T> *
_tree<T>::get_child_node(
	size_t index
	)
{
	_node<T> *result = NULL;

	move_child(index);
	result = get_position_node();
	move_parent();

	if(!result) {
		THROW_TREE_EXC(TREE_EXC_INVAL_CHILD_NODE);
	}

	return result;
}

template <class T> T &
_tree<T>::get_root(void)
{
	return _root->get();
}

template <class T> _node<T> *
_tree<T>::get_root_node(void)
{
	return _root;
}

template <class T> bool 
_tree<T>::has_children(void)
{
	return _get_position()->has_children();
};

template <class T> bool 
_tree<T>::has_parent(void)
{
	return _get_position()->has_parent();
};

template <class T> void 
_tree<T>::insert_child(
	const T &child, 
	size_t index
	)
{
	_node<T> *position = _get_position(), *new_child = NULL;

	if(index > position->get_child_count()) {
		THROW_TREE_EXC_W_MESS(index, TREE_EXC_INVAL_CHILD_NODE_INDEX);
	}
	new_child = new _node<T>;
	
	if(!new_child) {
		THROW_TREE_EXC(TREE_EXC_OUT_OF_MEMORY);
	}
	new_child->set_parent(position);
	new_child->get() = child;
	position->insert_child(new_child, index);
};

template <class T> void 
_tree<T>::insert_child_node(
	_node<T> *child, 
	size_t index
	)
{
	_node<T> *position = _get_position();

	if(index > position->get_child_count()) {
		THROW_TREE_EXC_W_MESS(index, TREE_EXC_INVAL_CHILD_NODE_INDEX);
	}
	position->insert_child(_copy_helper(position, child), index);
}

template <class T> void 
_tree<T>::move_child(
	size_t index
	)
{
	_node<T> *position = _get_position();

	if(index >= position->get_child_count()) {
		THROW_TREE_EXC_W_MESS(index, TREE_EXC_INVAL_CHILD_NODE_INDEX);
	}
	_position = _position->get_child(index);
};

template <class T> void 
_tree<T>::move_child_back(void)
{
	move_child(get_child_count() - 1);
};

template <class T> void 
_tree<T>::move_child_front(void)
{
	move_child(0);
};

template <class T> void 
_tree<T>::move_parent(void)
{
	_node<T> *position = _get_position();

	if(!position->has_parent()) {
		THROW_TREE_EXC(TREE_EXC_INVAL_PARENT_NODE);
	}
	_position = _position->get_parent();
};

template <class T> void 
_tree<T>::move_root(void)
{
	_position = _get_root();
};

template <class T> void 
_tree<T>::remove_child(
	size_t index
	)
{
	_node<T> *position = _get_position(), *old_child = NULL;

	if(index >= position->get_child_count()) {
		THROW_TREE_EXC_W_MESS(index, TREE_EXC_INVAL_CHILD_NODE_INDEX);
	}
	old_child = position->remove_child(index);

	if(old_child) {
		delete old_child;
		old_child = NULL;
	}
};

template <class T> void 
_tree<T>::remove_children(void)
{
	_node<T> *position = _get_position();

	while(position->has_children()) {
		remove_child(0);
	}
};

template <class T> std::string 
_tree<T>::to_string(
	bool verbose
	)
{
	std::stringstream ss;
	std::queue<_node<T> *> que;
	std::vector<_node<T> *>::iterator child;
	_node<T> *curr = NULL;
	
	if(_root) {
		que.push(_root);

		while(!que.empty()) {
			curr = que.front();

			if(_root == curr) {
				ss << "[R]";
			} else if(_position == curr) {
				ss << "[P]";
			}
			ss << curr->to_string(verbose) << std::endl;

			for(child = curr->get_child_begin(); child != curr->get_child_end(); ++child) {
				que.push(*child);
			}
			que.pop();
		}
	} else {
		ss << "Empty Tree";
	}

	return ss.str();
};

#endif