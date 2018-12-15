/** 
 * File: bit_buffer.h
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

#ifndef BIT_BUFFER_H

#define	BIT_BUFFER_H

#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "binary_heap.h"

/** Struktur fuer die Verwaltung des BIT_BUFFER. */
typedef struct _BIT_BUFFER
{
    /** Pointer auf den Anfang des Bufffers. */
    unsigned char *start;
    
    /** Bitindex des n�chsten zu setzenden Bits. */
    unsigned int index;
    
    /** Index des aktuellen char Wertes aus dem Buffer. */
    unsigned int array_index;
    
    /** Dateihandle fuer den Schreibzugriff. */
    FILE *file_handle;
} BIT_BUFFER;

/** Groessee des Buffers (Feldgroesse). */
#define BUFFER_SIZE 10

/** Variable fuer die Speicherung der BIT_BUFFER Struktur. */
BIT_BUFFER *bit_buffer;

/**
 * Initialisiert den Buffer und oeffnet die Datei zum schreiben.
 * 
 * @param file_handle Dateinname der Ausgabedatei
 */
extern void bit_buffer_init(FILE *file_handle);

/**
 * Schreibt ein weiteres Bit in den Buffer. Wenn der Buffer voll ist, wird
 * dieser in die Datei geschrieben.
 * 
 * @param bit FALSE fuer 0 TRUE f�r 1
 */
extern void bit_buffer_add_bit(BOOL bit);

/**
 * Fuegt eine gesamte Zeichenkette bestehend aus 0 & 1 in den Buffer ein.
 * Alle anderen Zeichen werden ignoriert.
 * 
 * @param bin_string binaercode in String Repr�sentation
 */
extern void bit_buffer_add_binary_string(char *bin_string);

/**
 * Schreibt den Buffer in die Datei. Wenn only_used = true dann wird nur
 * der bisher verwendete Buffer in die Datei geschrieben. Die verbleibenden
 * nicht benutzten Bits im letzten char werden mit 0 aufgef�llt.
 * Wenn only_used = false dann wird der gesamte Buffer geschrieben.
 * 
 * @param only_used true fuer ganzen Buffer, false nur fuer benutzten Buff.
 */
extern void bit_buffer_write_to_file(BOOL only_used);

/**
 * Gibt den Speicher fuer den Buffer wieder frei und schlie�t die Datei.
 */
extern void bit_buffer_destroy(void);

#endif	/* BIT_BUFFER_H */
