/** 
 * File: common.c 
 * Copyright (C) 2014 - Tim F. Rieck
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
 *	You should have received a copy of the GNU General Public License
 *	along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef COMMON_H

#define	COMMON_H
#define COMPRESS_EXT ".hc"
#define DECOMPRESS_EXT ".hd"

/**
 * Makro zur Pruefung, ob die Variable den Wert NULL hat: Ist die Bedingung
 * wahr, wird das Programm abgebrochen
 */
#define ENSURE_ENOUGH_MEMORY(VAR, FUNCTION)             \
do {                                                    \
    if (VAR == NULL) {                                  \
        printf(FUNCTION ": not enough memory\n");       \
        exit(EXIT_FAILURE); }} while(0)

/**
 * Makro zur Beendigung des Programs bei Fehlern welche den weiteren Verlauf
 * verhindern.
 */
#define ERROR_STOP_EXECUTION(FUNCTION, MESSAGE) \
{ printf(FUNCTION ": " MESSAGE); \
  exit(EXIT_FAILURE); }

/** Makro zur Ueberpruefung ob zwei Zeiger beide ungleich NULL sind. */
#define BOTH_NOT_NULL(NODE1, NODE2) (NODE1 != NULL) && (NODE2 != NULL)

/** Makro zur UEberpruefung ob zwei Zeiger beide gleich NULL sind. */
#define BOTH_NULL(NODE1, NODE2) (NODE1 == NULL) && (NODE2 == NULL)

/** Wahrheitswerte */
typedef enum
{
    FALSE,
    TRUE
} BOOL;

/** Kompressionsmodus. */
BOOL compress_mode;

/** Debug Ausagben. **/
BOOL debug_mode;

#endif	/* COMMON_H */
