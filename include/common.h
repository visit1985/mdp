#if !defined( COMMON_H )
#define COMMON_H

/*
 * Common macros.
 * Copyright (C) 2014 Michael Goehler
 *
 * This file is part of mdp.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * macro: MAX returns the higher one of two input variables
 * macro: MIN returns the lower one of two input variables
 *
 */

#define MAX(a, b) ({ typeof(a) _a = a; typeof(b) _b = b; _a > _b? _a : _b; })
#define MIN(a, b) ({ typeof(a) _a = a; typeof(b) _b = b; _a < _b? _a : _b; })

#endif // !defined( COMMON_H )
