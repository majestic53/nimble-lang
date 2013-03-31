/*
 * ppt_type.h
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

#ifndef PPT_TYPE_H_
#define PPT_TYPE_H_

#include <set>
#include <string>
#include <vector>
#include "arg_type.h"
#include "in_arg_type.hpp"
#include "..\nblang\lang.h"
#include "..\nblang\ver.h"

/*
 * Prompt class type
 */
class _ppt;
typedef _ppt ppt, *ppt_ptr;

/*
 * Prompt types
 */
enum {
	PPT_EXC_DUP_SCHEMA_REG = 0,
	PPT_EXC_INPUT_PIPE_CLOSED,
	PPT_EXC_INVAL_HDL_ACTION_TYPE,
	PPT_EXC_INVAL_SCHEMA_CMD,
	PPT_EXC_INVAL_SCHEMA_UNREG,
	PPT_EXC_INVAL_SIGNAL_TYPE,
	PPT_EXC_INVAL_STATE_CHANGE,
	PPT_EXC_UNK_SCHEMA_CMD_TYPE,
};

enum {
	PPT_ARG_FILE_IN = 0,
	PPT_ARG_HELP,
	PPT_ARG_IN,
	PPT_ARG_INTERACT,
	PPT_ARG_VERSION,
};

enum {
	PPT_ARG_SIG_EXIT = 0,
	PPT_ARG_SIG_INPUT,
	PPT_ARG_SIG_INTERACT,
};

enum {
	PPT_CMD_COPYRIGHT = 0,
	PPT_CMD_EXIT,
	PPT_CMD_HELP,
	PPT_CMD_LICENSE,
	PPT_CMD_LOAD,
	PPT_CMD_RESET,
	PPT_CMD_VERSION,
};

enum {
	PPT_HDL_ACTION_ARGUMENTS = 0,
	PPT_HDL_ACTION_GENERIC,
};

/*
 * Prompt max types
 */
#define MAX_PPT_EXC_TYPE PPT_EXC_UNK_SCHEMA_CMD_TYPE
#define MAX_PPT_ARG_TYPE PPT_ARG_VERSION
#define MAX_PPT_ARG_SIG_TYPE PPT_ARG_SIG_INTERACT
#define MAX_PPT_CMD_TYPE PPT_CMD_VERSION
#define MAX_PPT_HDL_ACTION_TYPE PPT_HDL_ACTION_GENERIC

/*
 * Prompt strings
 */
static const std::string PPT_EXC_STR[] = {
	"Duplicate schema registration",
	"Input pipe was closed",
	"Invalid prompt action type",
	"Attempting to register schema with in valid command",
	"Attempting to unregister unknown schema",
	"Invalid argument handler signal type",
	"Attempting to transition to invalid state",
	"Unknown command",
};

static const std::string PPT_ARG_STR_0[] = {
	"fi", "h", "in", "i", "v",
};

static const std::string PPT_ARG_STR_1[] = {
	"file-in", "help", "input", "interactive", "version",
};

static const std::string PPT_ARG_DESC_STR[] = {
	"Specify an input file to execute, followed by a series of arguments",
	"Display help information",
	"Specify input code, followed by a series of arguments",
	"Enter interactive mode",
	"Display version information",
};

static const std::string PPT_CMD_STR[] = {
	"copyright", "exit", "help", "license", "load", "reset", "version",
};

static const std::string PPT_CMD_DESC_STR[] = {
	"Display copyright information",
	"Exit interactive mode",
	"Display help information",
	"Display license information",
	"Load a file in interactive mode, followed by a series of arguments",
	"Reset the state of the interactive mode",
	"Display version information",
};

static const std::string PPT_HDL_ACTION_STR[] = {
	"PPT_HDL_ACTION_ARGUMENTS",
	"PPT_HDL_ACTION_GENERIC",
};

/*
 * Prompt sets
 */
static const std::set<std::string> PPT_ARG_SET_0(
	PPT_ARG_STR_0,
	PPT_ARG_STR_0 + MAX_PPT_ARG_TYPE + 1
	);

static const std::set<std::string> PPT_ARG_SET_1(
	PPT_ARG_STR_1,
	PPT_ARG_STR_1 + MAX_PPT_ARG_TYPE + 1
	);

static const std::set<std::string> PPT_CMD_SET(
	PPT_CMD_STR,
	PPT_CMD_STR + MAX_PPT_CMD_TYPE + 1
	);

/*
 * Prompt symbols
 */
#define ARG_FLAG_0 '-'
#define ARG_FLAG_0_LEN 1
#define ARG_FLAG_1 "--"
#define ARG_FLAG_1_LEN 2
#define CMD_FLAG '/'
#define CMD_FLAG_LEN 1
#define CURSOR "?> "
#define MIN_PROMPT_INPUT 1

/*
 * Prompt string lookup macros
 */
#define PPT_EXC_STRING(_T_) (_T_ > MAX_PPT_EXC_TYPE ? "Unknown prompt exception type" : PPT_EXC_STR[_T_])
#define PPT_ARG_STRING_0(_T_) (_T_ > MAX_PPT_ARG_TYPE ? "Unknown prompt argument type" : PPT_ARG_STR_0[_T_])
#define PPT_ARG_STRING_1(_T_) (_T_ > MAX_PPT_ARG_TYPE ? "Unknown prompt argument type" : PPT_ARG_STR_1[_T_])
#define PPT_ARG_DESCRIPTION(_T_) (_T_ > MAX_PPT_ARG_TYPE ? "Unknown prompt argument description type": PPT_ARG_DESC_STR[_T_])
#define PPT_CMD_STRING(_T_) (_T_ > MAX_PPT_CMD_TYPE ? "Unknown prompt command type" : PPT_CMD_STR[_T_])
#define PPT_CMD_DESCRIPTION(_T_) (_T_ > MAX_PPT_CMD_TYPE ? "Unknown prompt command description type" : PPT_CMD_DESC_STR[_T_])
#define PPT_HDL_ACTION_STRING(_T_) (_T_ > MAX_PPT_HDL_ACTION_TYPE ? "Unknown prompt handler action type" : PPT_HDL_ACTION_STR[_T_])

/*
 * Prompt string match macros
 */
#define IS_ARGUMENT(_S_) (_S_.empty() ? false : (_S_.at(0) == ARG_FLAG_0 || ((_S_.size() >= ARG_FLAG_1_LEN) && (_S_.substr(0, ARG_FLAG_1_LEN) == ARG_FLAG_1))))
#define IS_ARGUMENT_0(_S_) (_S_.empty() ? false : (_S_.at(0) == ARG_FLAG_0) && (PPT_ARG_SET_0.find(_S_.substr(ARG_FLAG_0_LEN)) != PPT_ARG_SET_0.end()))
#define IS_ARGUMENT_1(_S_) ((_S_.empty() || (_S_.size() < ARG_FLAG_1_LEN)) ? false : ((_S_.substr(0, ARG_FLAG_1_LEN) == ARG_FLAG_1) && (PPT_ARG_SET_1.find(_S_.substr(ARG_FLAG_1_LEN)) != PPT_ARG_SET_1.end())))
#define IS_COMMAND(_S_) (_S_.empty() ? false : (_S_.at(0) == CMD_FLAG)) // && (PPT_CMD_SET.find(_S_.substr(CMD_FLAG_LEN)) != PPT_CMD_SET.end()))

/*
 * Prompt helper macros
 */
#define PROMPT_COPYRIGHT(_S_) {\
	std::string ver;\
	std::stringstream ss;\
	LANG_VER_STRING(ver);\
	ss << LANG_TITLE << " " << ver << std::endl << LANG_COPYRIGHT;\
	_S_ = ss.str();\
	}

#define PROMPT_HEADER(_S_) {\
	std::string ver;\
	std::stringstream ss;\
	LANG_VER_STRING(ver);\
	ss << LANG_TITLE << " " << ver << std::endl << "Type \'" << CMD_FLAG << PPT_CMD_STRING(PPT_CMD_HELP) << "\' for more information.";\
	_S_ = ss.str();\
	}

#define PROMPT_HELP(_S_) {\
	std::stringstream ss;\
	ss << ppt_prompt_cmd_help();\
	_S_ = ss.str();\
	}

#define PROMPT_LICENSE(_S_) {\
	std::string ver;\
	std::stringstream ss;\
	LANG_VER_STRING(ver);\
	ss << LANG_TITLE << " " << ver << std::endl << LANG_TITLE << " is free software, released under version 3 of the GNU General Public License.";\
	_S_ = ss.str();\
	}

#define PROMPT_VERSION(_S_) {\
	std::string ver;\
	std::stringstream ss;\
	LANG_VER_STRING(ver);\
	ss << LANG_TITLE << " " << ver;\
	_S_ = ss.str();\
	}

#define PROMPT_USAGE(_S_) {\
	std::stringstream ss;\
	ss << "Usage: " << LANG_TITLE << " [" << ARG_FLAG_0 << PPT_ARG_STRING_0(PPT_ARG_HELP) << " | " << ARG_FLAG_0 << PPT_ARG_STRING_0(PPT_ARG_INTERACT) << " | " << ARG_FLAG_0 << PPT_ARG_STRING_0(PPT_ARG_VERSION)\
		<< "] (" << ARG_FLAG_0 << PPT_ARG_STRING_0(PPT_ARG_FILE_IN) << " [file] args... | " << ARG_FLAG_0 << PPT_ARG_STRING_0(PPT_ARG_IN) << " [input] args...)";\
	_S_ = ss.str();\
	}

/*
 * Prompt exception macros
 */
#define THROW_PPT_EXC(_T_) {\
	std::stringstream ss;\
	ss << PPT_EXC_STRING(_T_);\
	throw std::runtime_error(ss.str().c_str());\
	}

#define THROW_PPT_EXC_W_MESS(_M_, _T_) {\
	std::stringstream ss;\
	ss << PPT_EXC_STRING(_T_) << ": " << _M_;\
	throw std::runtime_error(ss.str().c_str());\
	}

/*
 * Prompt callback types
 */
typedef void(*ppt_action)(ppt_ptr, std::vector<arg> &);
typedef in_arg(*ppt_arg_action)(ppt_ptr, std::vector<std::string> &);
typedef void(*ppt_init_action)(ppt_ptr);
typedef void(*ppt_exc_action)(ppt_ptr, std::runtime_error &exc);

/*
 * Find prompt argument
 * @param text input text
 * @return prompt argument type
 */
extern size_t ppt_find_argument(
	const std::string &text
	);

/*
 * Form prompt help text
 * @return prompt help text
 */
extern std::string ppt_prompt_arg_help(void);

/*
 * Form prompt command help text
 * @return prompt command help text
 */
extern std::string ppt_prompt_cmd_help(void);

#endif