/** 
 * File: binary_heap.c
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "binary_heap.h"
#include "btree.h"
#include "huffman.h"

/**
 * Hilfsfunktionn fuer rekursive Ausgabe.
 * 
 * @param i     Startindex
 * @param ident Einrueckung
 */
static void print_help(BINARY_HEAP* heap,
                        unsigned int i,
                        unsigned int ident);

/**
 * Hilfsfunktion zum vertauschen von Elementen.
 * 
 * @param x Element 1
 * @param y Element 2 
 */
static void swap(void** p_x, void** p_y);


/** ---------------------------------------------------------------------------
 *  Funktion: heap_init
 * ------------------------------------------------------------------------- */
extern BINARY_HEAP* heap_init(GET_VALUE get_value,
                               PRINT_VALUE print_value,
                               DESTROY destroy)
{
    BINARY_HEAP* heap = calloc(1,sizeof(BINARY_HEAP));
    
    ENSURE_ENOUGH_MEMORY(heap, "heap_init");
    
    heap->count = 0;
    heap->size = MIN_HEAP_SIZE;
    heap->start = calloc(MIN_HEAP_SIZE, sizeof(void*));
    
    heap->get_value = get_value;
    heap->print_value = print_value;
    heap->destroy = destroy;
    
    return heap;
}

/** ---------------------------------------------------------------------------
 * Funktion: heap_destroy
 * ------------------------------------------------------------------------- */
extern void heap_destroy(BINARY_HEAP* heap)
{
    unsigned int i;
    if (heap != NULL && heap->start != NULL)
    {
        if (heap->destroy != NULL)
        {
                for (i = 0; i < heap->size; i++)
                {
                        heap->destroy(heap->start[i]);
                }
        }
    }
    free(heap->start);
    free(heap);
}

/** ---------------------------------------------------------------------------
 * Funktion: heap_insert
 * ------------------------------------------------------------------------- */
extern void heap_insert(BINARY_HEAP* heap, void* p_element)
{
    unsigned int current_element, parent;
    
    /* Wenn der Heap nicht initialisiert ist abbrechen. */
    if (heap == NULL) return;
    
    /* Pruefen der Heapgroesse. */
    if (heap->count + 1 > heap->size)
    {
        heap->size = heap->size * 2;
        heap->start = realloc(heap->start, sizeof(void*) * heap->size);
        memset(heap->start + (heap->size / 2), 0, sizeof(void*));
        
        ENSURE_ENOUGH_MEMORY(heap->start, "heap_insert");
    }
    
    /* Heapcounter inkrementieren. */
    heap->count++;
    
    /* Element in Heap einfuegen an die naechste freie Stelle. */
    current_element = heap->count;
    heap->start[current_element-1] = p_element;
    
    /*
     * Vertausche solange Vater und Sohn bis Heapeigenschaft wiederhergestellt.
     */
    parent = PARENT(current_element);
    
    while (parent > 0 && (heap->get_value(heap->start[current_element-1]) < 
                          heap->get_value(heap->start[parent-1])))
    {
        swap(&heap->start[current_element-1],&heap->start[parent-1]);
        current_element = parent;
        parent = PARENT(parent);
    }
}

/** ---------------------------------------------------------------------------
 * Funktion: heap_extract_min
 * ------------------------------------------------------------------------- */
extern BOOL heap_extract_min(BINARY_HEAP* heap,void** p_min_element)
{
    unsigned int current_element, left, right;
    BOOL success = FALSE;
    
    /* Wurzel des Heaps. */
    current_element = 1;
    
    /* Wenn der Baum nicht leer ist ist eine Extraktion moeglich. */
    if (heap->count > 0)
    {
        /* Extrahiere Wert. */        
        *p_min_element = heap->start[current_element-1];
    
        /*
         * Letztes Element an erste Stelle setzen und die letzte 
         * Stelle "loeschen".
         */
        swap(&heap->start[current_element-1], &heap->start[heap->count-1]);
        heap->count = heap->count - 1;
        success = TRUE;

        /* Heapstruktur wiederherstellen. */
        left = LEFT(current_element);
        right = RIGHT(current_element);

        while ( (left  <= heap->count || right <= heap->count) &&
              (heap->get_value(heap->start[current_element-1]) > 
                heap->get_value(heap->start[left-1]) ||
               heap->get_value(heap->start[current_element-1]) > 
                heap->get_value(heap->start[right-1])))
        {
            left = LEFT(current_element);
            right = RIGHT(current_element);

            if (heap->get_value(heap->start[left-1]) 
                    < heap->get_value(heap->start[right-1])) 
            {
                swap(&heap->start[current_element-1], &heap->start[left-1]);

                current_element = left;
                left = LEFT(current_element);
                right = RIGHT(current_element);
            }
            else if (right <= heap->count)
            {
                swap(&heap->start[current_element-1], &heap->start[right-1]);

                current_element = right;
                left = LEFT(current_element);
                right = RIGHT(current_element);            
            }
            else
            {
                current_element = left;
                left = LEFT(current_element);
                right = RIGHT(current_element);            
            }
        }
    }
    
    /*
     * Heap wieder verkleinern insofern nicht mehr soviel Platz benoetigt wird.
     */    
    if (heap->size > 1 && heap->count <= heap->size / 2)
    {
        heap->size = heap->size / 2;
        heap->start = realloc(heap->start, sizeof(void*) * heap->size);
        
        ENSURE_ENOUGH_MEMORY(heap->start, "heap_extract_min");     
    }
    
    return success;
}

/** ---------------------------------------------------------------------------
 * Funktion: print_help
 * ------------------------------------------------------------------------- */
static void print_help(BINARY_HEAP* heap,
                       unsigned int i,
                       unsigned int ident)
{
    unsigned int ident_counter = ident;
    
    if (i <= heap->count)
    {
        printf("|");
        
        /* Einruecken */
        while (ident_counter > 0)
        {
            printf("-");
            ident_counter = ident_counter - 1;
        }
        printf("%p: ",heap->start[i-1]);
        
        /* Wenn Ausgabefunktion vorhanden Ausgabe des Elements. */
        if (heap->print_value != NULL)
        {
            heap->print_value(heap->start[i-1]);
        }
        
        /* Rekursiver Aufruf der Printfunktion. */
        print_help(heap,LEFT(i),ident+1);
        print_help(heap,RIGHT(i),ident+1);
    }
}

/** ---------------------------------------------------------------------------
 * Funktion: swap
 * ------------------------------------------------------------------------- */
static void swap(void** p_x, void** p_y)
{
    void* p_temp;
    
    p_temp = *p_x;
    *p_x = *p_y;
    *p_y = p_temp;
}

/** ---------------------------------------------------------------------------
 * Funktion: heap_print
 * ------------------------------------------------------------------------- */
extern void heap_print(BINARY_HEAP* heap)
{
    print_help(heap,1,0);
}
