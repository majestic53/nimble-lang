/*
 * ver.h
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

#ifndef VER_H_
#define VER_H_

/*
 * Language version
 */
#define LANG_VER_MAJ 0
#define LANG_VER_MIN 6
#define LANG_VER_YR 13
#define LANG_VER_WW 14
#define LANG_VER_REV 03

/*
 * Language version strings
 */
#define LANG_TITLE "Nimble"
#define LANG_COPYRIGHT "Copyright (C) 2013 David Jolly"

/*
 * Language version string macro
 *		Version format: MAJ.MIN.YRWW.REV
 */
#define LANG_VER_STRING(_S_) {\
	std::stringstream ss;\
	ss << LANG_VER_MAJ << "." << LANG_VER_MIN << "." << LANG_VER_YR << LANG_VER_WW << " (rev. " << LANG_VER_REV << ", " << __DATE__ << ", " __TIME__ << ")";\
	_S_ = ss.str();\
	}

#endif