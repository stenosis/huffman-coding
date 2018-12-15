/** 
 * File: huffman.h 
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

#ifndef HUFFMAN_H

#define	HUFFMAN_H

#include "binary_heap.h"

/** Anzahl Elemente fuer die Speicher allokiert werden soll. */
#define ALLOC_ELEMENTS 10

/** Struktur eines Symbols. */
typedef struct _SYMBOL
{
    /**
     * Das jeweilige Zeichen
     */
    unsigned char symbol;
    /**
     * Die Haeufigkeit des Zeichens
     */
    unsigned int count;
    /**
     * Binaerkodierung als String
     */
    char *code;
} SYMBOL;

/** Zeiger auf den Startpunkt des Speicherbereichs der symbol_map. */
SYMBOL *p_symbol_start;

/** Zeiger fuer das Durchlaufen des Speicherbereichs der symbol_map. */
SYMBOL *p_symbol;

/** Anzahl der Symbole in der symbol_map. */
unsigned int symbol_count;

/** Anzahl eingelesener Zeichen. */
unsigned int read_char_count;

/** Zeiger fuer den dekomprimierten Text. */
unsigned char *p_decompressed_text;

/** Zeiger fuer den Startpunkt des dekomprimierten Text. */
unsigned char *p_decompressed_text_start;

/** Buffer fuer das eingelesene Bit. */
unsigned char read_bit_buffer;

/** Index des Bit Buffers. */
unsigned int  bit_buffer_index;

/**
 * Diese Funktion komprimiert den Inhalt einer Eingabedatei und schreibt den
 * komprimierten Inhalt in einer Ausgabedatei.
 * 
 * @param in_filename Eingabedatei
 * @param out_filename Ausgabedatei
 */
extern void compress(char *in_filename, char *out_filename);

/**
 * Diese Funktion dekomprimiert dne Inhalt einer Eingabedatei und schreibt den
 * dekomprimierten Inhalt in eine Ausgabedatei.
 * 
 * @param in_filename Eingabedatei
 * @param out_filename Ausgabedatei
 */
extern void decompress(char *in_filename, char *out_filename);

#endif	/* HUFFMAN_H */
