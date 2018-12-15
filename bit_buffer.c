/** 
 * File: bit_buffer.c
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

#include "bit_buffer.h"

/** ---------------------------------------------------------------------------
 *  bit_buffer_init
 *  ------------------------------------------------------------------------ */
extern void bit_buffer_init(FILE *file_handle)
{
    /*
     * Allokiere Speicher fuer Struktur
     */
    bit_buffer = calloc(1,sizeof(BIT_BUFFER));
    ENSURE_ENOUGH_MEMORY(bit_buffer, "init_bit_buffer");
    
    /*
     * Allokiere Speicher fuer Buffer
     */
    bit_buffer->start = calloc(BUFFER_SIZE, sizeof(char));
    ENSURE_ENOUGH_MEMORY(bit_buffer->start, "init_bit_buffer");
    
    /*
     * Oeffne Datei zum schreiben
     */
    bit_buffer->file_handle = file_handle;
    
    if (bit_buffer->file_handle == NULL)
    {
        printf("Ausgabedatei konnte nicht geoeffnet werden!");
        exit(0);
    }
    
    bit_buffer->array_index = 0;
    bit_buffer->index = 0;
}

/** ---------------------------------------------------------------------------
 *  bit_buffer_add_bit
 *  ------------------------------------------------------------------------ */
extern void bit_buffer_add_bit(BOOL bit)
{
    unsigned char bit_mask = 0;
    unsigned int i = 0;
    
    /*
     * Wenn eine 1 gesetzt werden muss wird eine Bitmaske vorbereitet
     * und verwendet um das entsprechende Bit im Char zu setzen.
     * Sollte eine 0 gesetzt werden muss nichts getan werden, da der
     * Buffer mit 0 initialisiert wird.
     */
    if (bit)
    {
        /* Bit setzen und an die richtige Stelle shiften. */
        bit_mask = 1;
        for (i = 0; i < (7 - bit_buffer->index); i++) bit_mask = bit_mask * 2;
        
        /*
         * Nun den entsprechenden Char wert verodern um das 1 Bit zu uebernehmen.
         */
        bit_buffer->start[bit_buffer->array_index] = 
                bit_buffer->start[bit_buffer->array_index] |
                bit_mask;
    }
    
    /* Anschliessend die Indexe inkrementieren. */
    bit_buffer->index++;
    if (bit_buffer->index > 7)
    {
        bit_buffer->index = 0;
        bit_buffer->array_index++;
    }
    
    /*
     * Wenn der Buffer voll sein sollte wird dieser in die Datei geschrieben,
     * und der Array_index wieder auf 0 gesetzt werden.
     */
    if (bit_buffer->array_index >= BUFFER_SIZE)
    {
        bit_buffer_write_to_file(FALSE);
        bit_buffer->array_index = 0;
    }
}

/** ---------------------------------------------------------------------------
 *  bit_buffer_add_binary_string
 *  ------------------------------------------------------------------------ */
extern void bit_buffer_add_binary_string(char *bin_string)
{
    char current_char;
    if (bin_string != NULL)
    {
        current_char = *bin_string;
        while(current_char != '\0')
        {
            /* Schreiben des jeweiligen Bits. */
            if (current_char == '1') bit_buffer_add_bit(TRUE);
            if (current_char == '0') bit_buffer_add_bit(FALSE);
            
            /* Pointer verschieben und Zeichen aktualisieren. */
            bin_string++;
            current_char = *bin_string;
        }
    }
}

/** ---------------------------------------------------------------------------
 *  bit_buffer_write_to_file
 *  ------------------------------------------------------------------------ */
extern void bit_buffer_write_to_file(BOOL only_used)
{
    unsigned int i;
    size_t char_written = 0;
    
    if (bit_buffer->file_handle != NULL)
    {
        /*
         * Schreibe die Char Werte in die Datei (komplett wenn only_used
         * = false, sonst nur die verwendeten Char).
         */
        if (only_used)
        {
            if (bit_buffer->index > 0 || bit_buffer->array_index > 0)
            {
                char_written = fwrite(bit_buffer->start,
                       sizeof(char),
                       bit_buffer->array_index + 1,
                       bit_buffer->file_handle);
                if (char_written == 0) {
                    printf("Konnte den Bit Buffer nicht rausschreiben.");
                    exit(EXIT_FAILURE);
                }
            }
        }
        else
        {
            char_written = fwrite(bit_buffer->start,
                   sizeof(char),
                   BUFFER_SIZE,
                   bit_buffer->file_handle);
            if (char_written == 0) {
                printf("Konnte den Bit Buffer nicht rausschreiben.");
                exit(EXIT_FAILURE);            
            }
        }
    }
    
    /*
     * Bitbuffer Index und Arrayindex zuruecksetzen und den Buffer mit 0
     * initialisieren.
     */
    bit_buffer->index = 0;
    bit_buffer->array_index = 0;
    
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        bit_buffer->start[i] = 0;
    }
}

/** ---------------------------------------------------------------------------
 *  bit_buffer_destroy
 *  ------------------------------------------------------------------------ */
extern void bit_buffer_destroy(void)
{
    if (bit_buffer != NULL)
    {
        /* Buffer freigeben. */
        if (bit_buffer->start != NULL)
        {
            free(bit_buffer->start);
        }

        /* Speicher fuer Struktur freigeben. */
        free(bit_buffer);
    }
}
