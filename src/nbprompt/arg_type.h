/*
 * arg_type.cpp
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

#ifndef ARG_TYPE_H_
#define ARG_TYPE_H_

#include <string>

/*
 * Argument class types
 */
class _arg;
typedef _arg arg, *arg_ptr;

/*
 * Argument types
 */
enum {
	ARG_TYPE_FLOAT = 0,
	ARG_TYPE_INTEGER,
	ARG_TYPE_STATIC,
	ARG_TYPE_STRING,
};

/*
 * Max argument types
 */
#define MAX_ARG_TYPE ARG_TYPE_STRING

/*
 * Argument strings
 */
static const std::string ARG_TYPE_STR[] = {
	"float", "integer", "static", "string",
};

/*
 * Argument string lookup macros
 */
#define ARG_TYPE_STRING(_T_) (_T_ > MAX_ARG_TYPE ? "Unknown argument type" : ARG_TYPE_STR[_T_])

#endif