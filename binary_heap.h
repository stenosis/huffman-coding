/** 
 * File: binary_heap.h
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

#ifndef BINARY_HEAP_H
#define	BINARY_HEAP_H

/**
 * Makro fuer die Berechnung des Index des Elternelements
 */
#define PARENT(i)  (i/2)
/**
 * Makro fuer die Berechnung des Index des linken Kindelements
 */
#define LEFT(i)    (i*2)
/**
 * Makro fuer die Berechnung des Index des rechten Kindelements
 */
#define RIGHT(i)   (i*2+1)
/**
 * Konstante fuer die Minimal Groesse des Heaps
 */
#define MIN_HEAP_SIZE 10

#include "common.h"
#include "btree.h"

/** Funktion fuer die Rueckgabe des enthaltenen Wertes. */
typedef int (*GET_VALUE) (void*);

/** Ausgabe des enthaltenen Wertes. */
typedef void (*PRINT_VALUE) (void*);

/** Funktion fuer die Freigabe des Speichers des jeweiligen Elements. */
typedef void (*DESTROY) (void*);

/** Datentyp & Struktur fuer die Verwaltung des Heap. */
typedef struct _binary_heap
{
    /**
     * Aktuelle Groesse des Heaps in Byte
     */
    unsigned int  size;
    /**
     * Im Heap enthaltene Elemente
     */
    unsigned int  count;
    /**
     * Pointer auf das erste Segment im Speicher des Heaps
     */
    void**        start;
    /**
     * Template Funktion fuer die Ermittlung des Wertes der enthaltenen Objekte
     */
    GET_VALUE     get_value;
    /**
     * Template Funktion fuer die Ausgabe des Wertes der enthaltenen Objekte
     */
    PRINT_VALUE   print_value;
    /**
     * Template Funktion fuer die freigabe des Speichers der enthaltenen Objekte
     */    
    DESTROY       destroy;
} BINARY_HEAP;

/**
 * Initialisiert den Heap und reserviert entsprechenden Speicher.
 * 
 * @param Funktion fuer die Rueckgabe des Wertes der enth. Obj.
 * @param Funktion fuer die Ausgabe der enth. Obj.
 * @param Funktion fuer die Freigabe und Loeschung der enth. Obj.
 * @return Initialisierter Heap
 */
extern BINARY_HEAP* heap_init(GET_VALUE, PRINT_VALUE, DESTROY);

/**
 * Loescht den Binaerheap und gibt den Speicher wieder frei.
 * 
 * @param Zeiger auf den zu zerstoerenden Binaer Heap
 */
extern void heap_destroy(BINARY_HEAP*);

/**
 * Fuegt ein neues Element in den Baum ein und erhaelt dabei die Heapeigenschaft
 * aufrecht.
 * 
 * @param Zeiger auf den Baum in den eingefuegt werden soll
 * @param Zeiger auf das einzufuegende Element
 */
extern void heap_insert(BINARY_HEAP*, void*);

/**
 * Extrahiert das kleinste Element aus dem heap und liefer es im Parameter
 * min_element zurueck. Die Heapeigenschaft wird beibehalten und Speicher
 * wenn notwendig freigegeben.
 * 
 * @param min_element Parameter in dem das Element zurueckgegeben wird
 * @return True wenn ein Element entfernt und zurueckgegeben werden kann, sonst
 *         false
 */
extern BOOL heap_extract_min(BINARY_HEAP*, void**);

/**
 * Ausgabe des Binaerbaumes auf dem Bildschirm.
 * 
 * @param Zeiger auf den Binaer_heap der ausgegeben werden soll
 */
extern void heap_print(BINARY_HEAP*);

#endif	/* BINARY_HEAP_H */
