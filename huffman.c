/** 
 * File: huffman.c 
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
#include "huffman.h"
#include "bit_buffer.h"

/**
 * Diese Funktion traversiert den Baum und erzeugt dabei die Kodierungen der 
 * einzelnen Zeichen.
 * 
 * @param root Wurzel des Binaerbaums
 * @param p_current_bit Momentaner Code
 * @param p_start_bit Start des Codes
 * @param depth Momentane Tiefe im Binaerbaum
 */
static void create_code_table(BTREE_NODE *root,
                                char *p_current_bit,
                                char *p_start_bit,
                                unsigned int depth);

/**
 * Diese Funktion gibt ein einzelnes Symbol aus.
 * 
 * @param symbol Auszugebendes Symbol 
 */
static void print_symbol(SYMBOL *symbol);

/**
 * Diese Funktion liest eine Datei mit dem uebergebenen Dateinamen ein und
 * erstellt aus den enthaltenen Zeichen eine symbolmap.
 * Diese wird anschliessend mit Quicksort sortiert.
 * 
 * @param in_filename Dateiname der Eingabedatei
 */
static void build_symbol_map(char *in_filename);

/**
 * Diese Funktion erzeugt den Wald aus den gesammelten Haeufigkeiten.
 * 
 * @return Binaerheap mit allen Teilbaeumen
 */
static BINARY_HEAP *create_tree_heap(void);

/**
 * Diese Funktion erzeugt den optimierten Huffman Codebaum aus dem uebergebenen 
 * Binaer Heap. Der uebergebene Binaerheap muss die 1-elementigen Teilbaeume 
 * aus der Haeufigkeitstabelle enthalten.
 * 
 * @param p_tree_heap Binaerheap aus dem ein Baum erzeugt werden soll
 * @return Huffman Codebaum
 */
static BTREE* create_huffman_tree(BINARY_HEAP *p_tree_heap);

/**
 * Diese Funktion gibt fuer Testzwecke die erstellte symbol_map aus.
 */
static void print_symbol_map(void);

/**
 * Diese Funktion gibt Informationen uber den allokierten Speicher aus.
 */
static void print_memory_info(void);

/**
 * Diese Funktion gibt die Code Tabelle fuer die jeweiligen Zeichen aus.
 */
static void print_code_table(void);

/**
 * Diese Funktion erstellt aus einem uebergebenen Binaerbaum den 
 * dekompressierten Text.
 * 
 * @param p_input_stream Eingabestrom der zu dekompressierenden Datei
 * @param root Wurzel des Binaerbaums
 */
static void create_decompressed_text(FILE *p_input_stream, BTREE_NODE *root);

/**
 * Diese Funktion liest nach und nach Bits aus der Eingabedatei und
 * durchlaeuft hierbei rekursiv den Binaerbaum. Wurde ein passendes Symbol
 * gefunden, wird dieses in den Ausgabetext geschrieben.
 * 
 * @param p_input_stream Eingabestrom der zu dekompressierenden Datei
 * @param root Wurzel des Binaerbaums oder Teilbaums
 */
static void get_symbol_from_tree(FILE *p_input_stream, BTREE_NODE *root);

/**
 * Diese Funktion liest die fuer die Dekomprimierung notwendigen Daten aus 
 * dem Header.
 * 
 * @param p_input_stream Eingabestrom der zu dekompressierenden Datei
 */
static void read_header(FILE *p_input_stream);

/**
 * Diese Funktion schreibt die komprimierte Datei.
 * 
 * @param out_filename Name der Ausgabedatei
 * @param in_filename Name der Eingabedatei
 */
static void write_compressed_file(char *out_filename, char *in_filename);

/**
 * Diese Funktion schreibt die dekomprimierte Datei.
 * 
 * @param out_filename Name der Ausgabedatei
 */
static void write_decompressed_file(char *out_filename);

/**
 * Diese Funktion schreibt den komprimierten Text in die Ausgabedatei.
 * 
 * @param p_output_stream Ausgabestrom fuer den komprimierten Text
 * @param in_filename Name der Eingabedatei
 */
static void write_huffman_code(FILE *p_output_stream, char *in_filename);

/**
 * Diese Funktion schreibt die fuer die Dekomprimierung notwendigen 
 * Daten in den Header.
 * 
 * @param p_output_stream Ausgabestrom fuer die komprimierte Datei
 */
static void write_header(FILE *p_output_stream);

/** ---------------------------------------------------------------------------
 *  Funktion: compress
 *  ------------------------------------------------------------------------ */
extern void compress(char *in_filename, char *out_filename)
{
    BINARY_HEAP *p_tree_heap;
    BTREE *p_huffman_tree;
    char *p_code = NULL;
    
    build_symbol_map(in_filename);

    if (debug_mode)
    {
        printf("\n---------------- Symbolmap erstellt ----------------\n\n");
        print_symbol_map();
    }
    
    p_tree_heap = create_tree_heap();
    if (debug_mode)
    {
        printf("\n---------------- TreeHeap erstellt ----------------\n\n");
        heap_print(p_tree_heap);
    }

    p_huffman_tree = create_huffman_tree(p_tree_heap);
    if (debug_mode)
    {
        printf("\n---------------- Huffman-Tree erstellt ----------------\n\n");
        btree_print(p_huffman_tree);
    }

    p_code = calloc((unsigned int)
                  btree_get_max_depth(btree_get_root(p_huffman_tree)),
                  sizeof(unsigned char));
    ENSURE_ENOUGH_MEMORY(p_code, "compress");
    
    create_code_table(btree_get_root(p_huffman_tree), p_code, p_code, 1);
    if (debug_mode)
    {
        print_code_table();
    }
    
    write_compressed_file(out_filename,in_filename);
    if (debug_mode) 
    {
        print_memory_info();
        printf("\n---------------- .hc-Datei geschrieben ----------------\n\n");
    }
    
    /**
     * Speicherfreigabe
     */
    free(p_code);
    p_code = NULL;
    
    btree_destroy(&p_huffman_tree, FALSE);
    
    heap_destroy(p_tree_heap);
    p_tree_heap = NULL;
}

/** ---------------------------------------------------------------------------
 *  Funktion: decompress
 *  ------------------------------------------------------------------------ */
extern void decompress(char *in_filename, char *out_filename)
{
    FILE *p_input_stream = fopen(in_filename,"rb");
    BINARY_HEAP* p_tree_heap;
    BTREE* p_huffman_tree;
    
    if (p_input_stream == NULL)
    {
        printf("Datei Einlesen fehlgeschlagen!\n");
        fflush(stdout);
        exit(EXIT_FAILURE);
    }
    
    read_header(p_input_stream);
    
    p_tree_heap = create_tree_heap();
    if (debug_mode)
    {
        printf("\n---------------- TreeHeap erstellt ----------------\n\n");
        heap_print(p_tree_heap);
    }
    
    p_huffman_tree = create_huffman_tree(p_tree_heap);
    if (debug_mode)
    {
        printf("\n---------------- Huffman-Tree erstellt ----------------\n\n");
        btree_print(p_huffman_tree);
    }
    
    create_decompressed_text(p_input_stream, btree_get_root(p_huffman_tree));
    if (debug_mode)
    {
        printf("\n----------- Dekomprimierter Text erstellt ------------\n\n");
        printf("%s\n", (char*) p_decompressed_text_start);
        printf("\n------------- .hc.hd-Datei geschrieben -------------\n\n");
    }

    fclose(p_input_stream);
    
    write_decompressed_file(out_filename);
    
    /**
     * Speicherfreigabe
     */
    btree_destroy(&p_huffman_tree, FALSE);
    p_huffman_tree = NULL;
    
    heap_destroy(p_tree_heap);
    p_tree_heap = NULL;    
}

/** ---------------------------------------------------------------------------
 *  Funktion: get_count_from_tree
 *  ------------------------------------------------------------------------ */
static unsigned int get_count_from_tree(BTREE* tree)
{
    BTREE_NODE* root = btree_get_root(tree);
    SYMBOL* symbol = btreenode_get_data(root);
    
    return symbol->count;
}

/** ---------------------------------------------------------------------------
 *  Funktion: create_code_table
 *  ------------------------------------------------------------------------ */
static void create_code_table(BTREE_NODE *root,
                                char *p_current_bit,
                                char *p_start_bit,
                                unsigned int depth)
{
    if (btreenode_is_leaf(root))
    {
        /*
         * Es liegt ein Blatt vor terminiere die Zeichenkette des Codes.
         */
        *p_current_bit = '\0';
        
        p_symbol = p_symbol_start;
        while (p_symbol->symbol != ((SYMBOL*)btreenode_get_data(root))->symbol)
        {
            p_symbol++;
        }
        
        p_symbol->code = calloc(1,strlen(p_start_bit) + 1);
        strcpy(p_symbol->code, p_start_bit);
    }
    else
    {
        /*
         * Kein Blatt? Dann Baum weiter traversieren.
         */
        if (btreenode_get_left(root) != NULL)
        {
            *p_current_bit = '0';
            create_code_table(btreenode_get_left(root),
                              p_current_bit  + 1,
                              p_start_bit,
                              depth + 1);
        }
        if (btreenode_get_right(root) != NULL) 
        {
            *p_current_bit = '1';
            create_code_table(btreenode_get_right(root),
                              p_current_bit  + 1,
                              p_start_bit,
                              depth + 1);
        }
    }
}

/** ---------------------------------------------------------------------------
 *  Funktion: create_decompressed_text
 *  ------------------------------------------------------------------------ */
static void create_decompressed_text(FILE *p_input_stream, BTREE_NODE *root)
{
    unsigned int i;
    
    p_decompressed_text_start = calloc(read_char_count + 1, 
                                       sizeof(unsigned char));
    p_decompressed_text = p_decompressed_text_start;
    
    bit_buffer_index = 0;
    
    for (i = 0; i < read_char_count; i++)
    {
        get_symbol_from_tree(p_input_stream, root);
    }
    *p_decompressed_text = '\0';
}

/** ---------------------------------------------------------------------------
 *  Funktion: get_next_bit
 *  ------------------------------------------------------------------------ */
static int get_next_bit(FILE *p_input_stream)
{
    unsigned int i;
    size_t read_byte;
    unsigned char current_byte;
    unsigned char checkmask = 1;
    
    /*
     * Lesen eines Bytes wenn notwendig.
     */
    if (bit_buffer_index == 0)
    {
        read_byte = (unsigned int) fread(&read_bit_buffer, 
                                         sizeof(unsigned char), 
                                         1, 
                                         p_input_stream);
        /*
         * Wenn nichts mehr gelesen wurde -1 zurueckgeben.
         */
        if (read_byte != 1) 
        {
            return -1;
        }
    }
    
    /*
     * Nun das jeweilige Bit an das Ende verschieben (z.B. 10000000 -> 00000001)
     * und mit 1 verunden. Sollte an der jeweiligen Stelle eine 0 stehen, wird
     * aus der 1 mit der verundet wurde eine 0 -> false.
     */
    current_byte = read_bit_buffer;
    
    /*
     * Wenn der Bitshift mal nicht funktioniert DIY ;-)
     */
    for (i = 0; i < (7 - bit_buffer_index); i++) 
    {
        current_byte /= 2;
    }
    
    checkmask = current_byte & checkmask;
    
    /*
     * Aktualisieren des bit_buffer_index.
     */
    bit_buffer_index = (bit_buffer_index + 1) % 8;
    
    return (int) checkmask;
}

/** ---------------------------------------------------------------------------
 *  Funktion: get_symbol_from_tree
 *  ------------------------------------------------------------------------ */
static void get_symbol_from_tree(FILE* p_input_stream, BTREE_NODE *root)
{
    int read_bit;

    if (btreenode_is_leaf(root))
    {
        *p_decompressed_text = ((SYMBOL*)btreenode_get_data(root))->symbol;
        p_decompressed_text++;
    }
    else
    {
        read_bit = get_next_bit(p_input_stream);
        if (read_bit && btreenode_get_right(root) != NULL)
        {
            get_symbol_from_tree(p_input_stream, btreenode_get_right(root));
        }
        else if (!read_bit && btreenode_get_left(root) != NULL)
        {
            get_symbol_from_tree(p_input_stream, btreenode_get_left(root));
        }
    }
}

/** ---------------------------------------------------------------------------
 *  Funktion: create_huffman_tree
 *  ------------------------------------------------------------------------ */
static BTREE* create_huffman_tree(BINARY_HEAP* p_tree_heap)
{
    BTREE* p_new_tree = NULL;
    BTREE* p_tree1 = NULL;
    BTREE* p_tree2 = NULL;
    SYMBOL* p_symbol = NULL;
    
    while (p_tree_heap->count > 1)
    {
        /*
         * Extrahiere die 2 Teilbaeume mit der kleinsten Haeufigkeit.
         */
        heap_extract_min(p_tree_heap, (void**)&p_tree1);
        heap_extract_min(p_tree_heap, (void**)&p_tree2);
        
        p_symbol = calloc(1,sizeof(SYMBOL));
        
        ENSURE_ENOUGH_MEMORY(p_symbol, "create_huffman_tree");
        
        p_symbol->symbol = 0;
        p_symbol->count = (unsigned int) (p_tree_heap->get_value(p_tree1) 
                                + p_tree_heap->get_value(p_tree2));
        
        p_new_tree = btree_merge(p_tree1, p_tree2, p_symbol);
        
        heap_insert(p_tree_heap, p_new_tree);
    }
    
    /*
     * Wenn alles Erfolgreich ist, dann gebe den finalen Codebaum zurueck.
     */
    return (heap_extract_min(p_tree_heap,(void**) & p_new_tree)) 
            ? p_new_tree : NULL;
}

/** ---------------------------------------------------------------------------
 *  Funktion: create_tree_heap
 *  ------------------------------------------------------------------------ */
static BINARY_HEAP* create_tree_heap(void)
{
    unsigned int i;
    BTREE* p_tree;
    BINARY_HEAP* p_tree_heap = heap_init((GET_VALUE)get_count_from_tree,
                                         (PRINT_VALUE)btree_print, NULL);
    
    for (i = 0; i < symbol_count; i++)
    {
        /*
         * Erzeugen der einzelen Baeume aus den Haeufigkeiten.
         */
        p_tree = btree_new(p_symbol_start+i, NULL, (PRINT_FCT) print_symbol);

        /*
         * Einfuegen in den HEAP.
         */
        heap_insert(p_tree_heap, p_tree);
    }
    return p_tree_heap;
}

/** ---------------------------------------------------------------------------
 *  Funktion: build_symbol_map
 *  ------------------------------------------------------------------------ */
static void build_symbol_map(char *in_filename)
{
    int symbol;
    unsigned int i;
    BOOL symbol_found;    
    BOOL debug_mode_extra = FALSE;
    FILE *p_input_stream = fopen(in_filename, "rb");
    
    symbol_count = 0;
    read_char_count = 0;
    
    if (p_input_stream != NULL)
    {
        p_symbol_start = calloc(ALLOC_ELEMENTS, sizeof(SYMBOL));
        p_symbol = p_symbol_start;
        
        if (p_symbol_start == NULL)
        {
            printf("Nicht genuegend Speicher vorhanden.\n");
            fflush(stdout);
            exit(EXIT_FAILURE);
        }
        
        /* 
         * Erstes Zeichen holen und so lange weitere Einlesen
         * bis Dateiende erreicht wurde.
         */
        symbol = getc(p_input_stream);
        while (symbol != EOF) 
        {
            
            if (debug_mode_extra)
            {
                printf("Read  int: %d\tchar: %c\n", 
                       symbol, (unsigned char) symbol);
                fflush(stdout);
            }
            
            symbol_found = FALSE;
            read_char_count++;
            
            /* 
             * Durchlaufen aller bishaerigen Eintraege der symbol_map 
             * und Vergleichen mit dem momentan eingelesenen Zeichen.
             */
            for (i = 0; i < symbol_count; i++)
            {
                if (p_symbol->symbol == (unsigned char) symbol)
                {
                    p_symbol->count++;
                    symbol_found = TRUE;
                    break;
                }
                else
                {
                    p_symbol++;
                }
            }

            /*
             * Wenn das Zeichen in der symbol_map nicht gefunden wurde, wird
             * entweder das Zeichen direkt in der symbol_map gespeichert oder
             * vorher noch Speicher allokiert.
             */
            if (!symbol_found)
            {
                symbol_count++;
                if ((symbol_count % ALLOC_ELEMENTS) > 0)
                {
                    p_symbol->symbol = (unsigned char) symbol;
                    p_symbol->count = 1;
                    
                    if (debug_mode_extra) 
                    {
                        printf("Added int: %d\t char: %c\n", 
                               symbol, p_symbol->symbol);
                        printf("Check int: %d\t char: %c\n", 
                               (int) p_symbol->symbol, p_symbol->symbol);
                        fflush(stdout);
                    }
                }
                else
                {
                    /* Nicht genuegend Speicher allokiert, realloc folgt. */
                    p_symbol_start = realloc(p_symbol_start,
                                            (symbol_count + ALLOC_ELEMENTS) *
                                                sizeof(SYMBOL));
                    memset(p_symbol_start + symbol_count, 0, ALLOC_ELEMENTS);
                    
                    if (p_symbol_start == NULL) {
                        
                        printf("Reallokierung fehlgeschlagen.\n");
                        fflush(stdout);
                        exit(EXIT_FAILURE);
                        
                    } else {
                        
                        p_symbol = p_symbol_start + symbol_count -1;
                        p_symbol->symbol = (unsigned char) symbol;
                        p_symbol->count = 1;
                        
                        if (debug_mode_extra)
                        {
                            printf("Added int: %d\t char: %c\n", 
                                   symbol, p_symbol->symbol);
                            printf("Check int: %d\t char: %c\n", 
                                   (int) p_symbol->symbol, p_symbol->symbol);     
                            fflush(stdout);
                        }
                    }
                }
            }
            /* Zeiger zurueck auf den Startwert setzen. */
            p_symbol = p_symbol_start;
            symbol = getc(p_input_stream);
       }
    }
    else
    {
        printf("Datei Einlesen fehlgeschlagen!\n");
        fflush(stdout);
        exit(EXIT_FAILURE);
    }
    /* Schliessen des Eingabestroms und sortieren der symbol_map. */
    fclose(p_input_stream);
}

/** ---------------------------------------------------------------------------
 *  Funktion: write_compressed_file
 *  ------------------------------------------------------------------------ */
static void write_compressed_file(char *out_filename, char *in_filename)
{
    FILE *p_output_stream = fopen(out_filename, "wb");
    
    if (p_output_stream != NULL)
    {
        write_header(p_output_stream);
        write_huffman_code(p_output_stream, in_filename);
    }
    else
    {
        printf("Datei zum Schreiben konnte nicht geoeffnet werden.\n");
        fflush(stdout);
        exit(EXIT_FAILURE);
    }
    fclose(p_output_stream);
}

/** ---------------------------------------------------------------------------
 *  Funktion: write_decompressed_file
 *  ------------------------------------------------------------------------ */
static void write_decompressed_file(char *out_filename)
{
    FILE *p_output_stream = fopen(out_filename, "wb");
    
    
    if (p_output_stream != NULL)
    {
        /**
         * TODO: Hier eine ueberpruefung der Architektur und nach
         * Big Endian / Little Endian schreiben
         */
        fwrite(p_decompressed_text_start,
               sizeof(unsigned char*),
               read_char_count,
               p_output_stream);
    }
    else
    {
        printf("Datei zum Schreiben konnte nicht geoeffnet werden.\n");
        fflush(stdout);
        exit(EXIT_FAILURE);
    }
    fclose(p_output_stream);   
}

/** ---------------------------------------------------------------------------
 *  Funktion: write_huffman_code
 *  ------------------------------------------------------------------------ */
static void write_huffman_code(FILE *p_output_stream, char *in_filename)
{
    unsigned int i;
    int char_read;
    FILE *p_input_stream = fopen(in_filename, "rb");
    p_symbol = p_symbol_start;    
    
    /**
     * Ab 2.Zeile: Huffman-Code schreiben.
     */
    bit_buffer_init(p_output_stream);
    if (p_input_stream != NULL)
    {
        char_read = fgetc(p_input_stream);
        while (char_read != EOF)
        {
            p_symbol = p_symbol_start;
            for (i = 0; i < symbol_count; i++)
            {
                /**
                 * Zeichen im Struct-Array gefunden: Code schreiben.
                 */
                if (p_symbol->symbol == (unsigned char) char_read)
                {
                    bit_buffer_add_binary_string(p_symbol->code);
                }
                p_symbol++;
            }
            char_read = fgetc(p_input_stream);
        }
    }
    fclose(p_input_stream);
    bit_buffer_write_to_file(TRUE);
    bit_buffer_destroy();
}

/** ---------------------------------------------------------------------------
 *  Funktion: print_symbol_map
 *  ------------------------------------------------------------------------ */
static void print_symbol_map(void) 
{
    unsigned int i;
    
    /* Ausgabe der eingelesenen Zeichen mit ihrer Anzahl. */
    p_symbol = p_symbol_start;
    for (i = 0; i < symbol_count; i++)
    {
        if (p_symbol->symbol == '\n')
        {
            printf("\tSymbol: \\n \tAnzahl:  %u\n", p_symbol->count);
            fflush(stdout);
        }
        else if (p_symbol->symbol == '\t')
        {
            printf("\tSymbol: \\t \tAnzahl:  %u\n", p_symbol->count);
            fflush(stdout);
        }
        else
        {
            printf("\tSymbol:  %c \tAnzahl:  %u\n", 
                   (char) p_symbol->symbol, p_symbol->count);
            fflush(stdout);
        }
        p_symbol++;
    }
}

/** ---------------------------------------------------------------------------
 *  Funktion: print_memory_info
 *  ------------------------------------------------------------------------ */
static void print_memory_info(void)
{
    int bytes = (int) (symbol_count * sizeof(SYMBOL));
    printf("\n------------------ Memory Info -------------------\n\n");
    printf("\tAnzahl Zeichen: \t%u\n", symbol_count);
    printf("\tAnzahl gel. Zeichen: \t%u\n", read_char_count);
    printf("\tStruct Array: \t\t%d Byte (%d Bit)\n", bytes, bytes * 8);
    fflush(stdout);
}

/** ---------------------------------------------------------------------------
 *  Funktion: print_code_table
 *  ------------------------------------------------------------------------ */
static void print_code_table(void)
{
    unsigned int i;
    p_symbol = p_symbol_start;
    
    printf("\n------------------ Code Table -------------------\n\n");
    for (i = 0; i < symbol_count; i++)
    {
        if (p_symbol->symbol == '\n')
        {
            printf("\tSymbol: \\n\t\tCode: %s\n", p_symbol->code);
            fflush(stdout);
        }
        else if (p_symbol->symbol == '\t')
        {
            printf("\tSymbol: \\t\t\tCode: %s\n", p_symbol->code);
            fflush(stdout);
        }
        else
        {
            printf("\tSymbol: %c\t\tCode: %s\n", 
                   p_symbol->symbol, p_symbol->code);
            fflush(stdout);
        }
        p_symbol++;
    }    
}

/** ---------------------------------------------------------------------------
 *  Funktion: print_symbol
 *  ------------------------------------------------------------------------ */
static void print_symbol(SYMBOL* symbol)
{
    if (symbol != NULL)
    {
        
        if (symbol->symbol == '\n')
        {
            printf("Char: \\n, Count: %u\n", symbol->count);
            fflush(stdout);
        } 
        else
        {
            printf("Char: %c, Count: %u, \n", symbol->symbol, symbol->count);
            fflush(stdout);
        }
    }
}

/** ---------------------------------------------------------------------------
 *  Funktion: read_header
 *  ------------------------------------------------------------------------ */
static void read_header(FILE *p_input_stream)
{
    unsigned int i = 0;
    size_t bytes_read = 0;
    
    bytes_read = fread(&symbol_count, sizeof(unsigned int), 1, p_input_stream);
    
    if (bytes_read == 0)
    {
        printf("Fehler beim einlesen des Headers.\n");
        exit(EXIT_FAILURE);
    }
    
    if (debug_mode)
    {
        printf("------------------ Symbolmap erstellt -------------------\n\n");
        printf("\tSymbol_count: %u\n", symbol_count);
        fflush(stdout);
    }
    
    bytes_read = fread(&read_char_count, 
                       sizeof(unsigned int), 
                       1, 
                       p_input_stream);

    if (bytes_read == 0)
    {
        printf("Fehler beim einlesen des Headers.\n");
        exit(EXIT_FAILURE);
    }
    
    if (debug_mode)
    {
        printf("\tRead_char_count: %u\n\n", read_char_count);
        fflush(stdout);
    }
    
    
    p_symbol_start = calloc(symbol_count, sizeof(SYMBOL));
    p_symbol = p_symbol_start;
    
    for (i = 0; i < symbol_count; i++)
    {
        bytes_read =  fread(&p_symbol->symbol,
                            sizeof(unsigned char), 1, p_input_stream);
        bytes_read += fread(&p_symbol->count,
                            sizeof(unsigned int), 1, p_input_stream);
        if (bytes_read == 0)
        {
            printf("Fehler beim einlesen des Headers.\n");
            exit(EXIT_FAILURE);
        }        
        p_symbol++;
    }
    
    if (debug_mode)
    {
        print_symbol_map();
    }
}

/** ---------------------------------------------------------------------------
 *  Funktion: write_header
 *  ------------------------------------------------------------------------ */
static void write_header(FILE *p_output_stream)
{
    size_t bytes_written = 0;
    unsigned int i = 0;
    p_symbol = p_symbol_start;
    
    bytes_written =  fwrite(&symbol_count,
                            sizeof(unsigned int), 1, p_output_stream);
    bytes_written += fwrite(&read_char_count,
                            sizeof(unsigned int), 1, p_output_stream);

    if (bytes_written == 0)
    {
        printf("Fehler beim schreiben des Headers.\n");
        exit(EXIT_FAILURE);
    }    
    
    for (i = 0; i < symbol_count; i++)
    {
        bytes_written =  fwrite(&(p_symbol->symbol),
                                sizeof(unsigned char), 1, p_output_stream);
        bytes_written += fwrite(&(p_symbol->count),
                                sizeof(unsigned int), 1, p_output_stream);

        if (bytes_written == 0)
        {
            printf("Fehler beim schreiben des Headers.\n");
            exit(EXIT_FAILURE);
        }           
        p_symbol++;
    }
}
