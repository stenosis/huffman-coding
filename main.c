/** 
 * File: main.c 
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
#include "argument_checker.h"

/**
 * Diese Funktion startet das Programm.
 * 
 * @param argc Anzahl Parameter + Programmname
 * @param argv Parameterliste
 * @return 0 wenn fehlerfrei
 */
int main(int argc, char** argv)
{
    char *in_filename = NULL;
    char *out_filename = NULL;
    
    check_arguments(argc, argv, &in_filename, &out_filename);
    
    if (compress_mode == TRUE)
    {
        compress(in_filename, out_filename);
    }
    else
    {
        decompress(in_filename, out_filename);
    }
    
    return (EXIT_SUCCESS);
}
