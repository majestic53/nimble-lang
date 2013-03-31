/*
 * trace.h
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

#ifndef TRACE_H_
#define TRACE_H_

#include <sstream>
#include <string>

/*
 * Trace types
 */
enum {
	TRACE_TYPE_ERROR = 0,
	TRACE_TYPE_WARNING,
	TRACE_TYPE_INFORMATION,
	TRACE_TYPE_VERBOSE,
};

/*
 * Max trace types
 */
#define MAX_TRACE_TYPE TRACE_TYPE_VERBOSE

/*
 * Trace strings
 */
static const std::string TRACE_TYPE_STR[] = {
	"ERROR", "WARNING", "INFORMATION", "VERBOSE",
};

/*
 * Trace string lookup macros
 */
#define TRACE_TYPE_STRING(_T_) (_T_ > MAX_TRACE_TYPE ? "UNKNOWN" : TRACE_TYPE_STR[_T_])

/* 
 * Trace event macros
 * Under Windows, visible from debugview; else visible from stderr stream
 */
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOCOMM
#include <Windows.h>
#define TRACE_EVENT(_M_, _T_) {\
	std::stringstream ss;\
	ss << " [" << TRACE_TYPE_STRING(_T_) << "] " << TRACE_HEADER << ": " << _M_ << std::endl;\
	OutputDebugString(ss.str().c_str());\
	}
#else
#include <iostream>
#define TRACE_EVENT(_M_, _T_) {\
	std::stringstream ss;\
	ss << " [" << TRACE_TYPE_STRING(_T_) << "] " << TRACE_HEADER << ": " << _M_;\
	std::cerr << ss.str() << std::endl;\
	}
#endif

#endif