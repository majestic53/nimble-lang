/*
 * ppt_type.cpp
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

#include <iomanip>
#include <sstream>
#include "ppt_type.h"

/*
 * Prompt column sizes
 */
#define SHORT_COL_WID 5
#define LONG_COL_WID 20

size_t 
ppt_find_argument(
	const std::string &text
	)
{
	size_t i = 0, result = INVALID_TYPE;

	for(; i <= MAX_PPT_ARG_TYPE; ++i) {

		if(text == PPT_ARG_STRING_0(i)
				|| text == PPT_ARG_STRING_1(i)) {
			result = i;
			break;
		}
	}

	return result;
}

std::string 
ppt_prompt_arg_help(void)
{
	size_t i = 0;
	std::string output;
	std::stringstream ss;

	PROMPT_VERSION(output);
	ss << output << std::endl;
	PROMPT_USAGE(output);
	ss << output << std::endl << "------------------------------------------------" << std::endl;
	
	for(; i <= MAX_PPT_ARG_TYPE; ++i) {

		if(i) {
			ss << std::endl;
		}
		ss << std::left << ARG_FLAG_0 << std::setw(SHORT_COL_WID) << PPT_ARG_STRING_0(i) << ARG_FLAG_1 << std::setw(LONG_COL_WID) 
				<< PPT_ARG_STRING_1(i) << PPT_ARG_DESCRIPTION(i);
	}

	return ss.str();
}

std::string 
ppt_prompt_cmd_help(void)
{
	size_t i = 0;
	std::string output;
	std::stringstream ss;
	
	PROMPT_VERSION(output);
	ss << output << std::endl << "------------------------------------------------" << std::endl;

	for(; i <= MAX_PPT_CMD_TYPE; ++i) {

		if(i) {
			ss << std::endl;
		}
		ss << std::left << CMD_FLAG << std::setw(LONG_COL_WID) << PPT_CMD_STRING(i) << PPT_CMD_DESCRIPTION(i);
	}

	return ss.str();
}