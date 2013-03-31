/*
 * ppt_ext.h
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

#ifndef PPT_EXT_H_
#define PPT_EXT_H_

#include "ppt_type.h"

/*
 * Prompt extension arguments handler
 */
extern in_arg ppt_ext_hdl_arguments(
	ppt_ptr prompt,
	std::vector<std::string> &arguments
	);

/*
 * Prompt extension command handler
 */
extern void ppt_ext_hdl_copyr_command(
	ppt_ptr prompt,
	std::vector<arg> &arguments
	);

extern void ppt_ext_hdl_exceptions(
	ppt_ptr prompt,
	std::runtime_error &exc
	);

extern void ppt_ext_hdl_exit_command(
	ppt_ptr prompt,
	std::vector<arg> &arguments
	);

extern void ppt_ext_hdl_gen_command(
	ppt_ptr prompt,
	std::vector<arg> &arguments
	);

extern void ppt_ext_hdl_help_command(
	ppt_ptr prompt,
	std::vector<arg> &arguments
	);

extern void ppt_ext_hdl_lic_command(
	ppt_ptr prompt,
	std::vector<arg> &arguments
	);

extern void ppt_ext_hdl_load_command(
	ppt_ptr prompt,
	std::vector <arg> &arguments
	);

extern void ppt_ext_hdl_res_command(
	ppt_ptr prompt,
	std::vector<arg> &arguments
	);

extern void ppt_ext_hdl_ver_command(
	ppt_ptr prompt,
	std::vector<arg> &arguments
	);

/*
 * Prompt extension initialize executor instance
 */
extern void ppt_ext_init_exe(
	ppt_ptr prompt
	);

#endif