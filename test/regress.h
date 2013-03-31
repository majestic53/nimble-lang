/*
 * regress.h
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

#ifndef REGRESS_H_
#define REGRESS_H_

#include "regress_type.h"

/*
 * Invoke executor regression test
 * @param test executor regression test type
 * @return true on success, false otherwise
 */
extern bool exe_regress_test(
	size_t test
	);

/*
 * Invoke all executor regression tests
 * @return true on success, false otherwise
 */
extern bool exe_regress_test_all(void);

/*
 * Invoke lexer regression test
 * @param test lexer regression test type
 * @return true on success, false otherwise
 */
extern bool lex_regress_test(
	size_t test
	);

/*
 * Invoke all lexer regression tests
 * @return true on success, false otherwise
 */
extern bool lex_regress_test_all(void);

/*
 * Invoke parser regression test
 * @param test parser regression test type
 * @return true on success, false otherwise
 */
extern bool par_regress_test(
	size_t test
	);

/*
 * Invoke all parser regression tests
 * @return true on success, false otherwise
 */
extern bool par_regress_test_all(void);

#endif