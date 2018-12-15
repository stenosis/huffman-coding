/** 
 * File: argument_checker.c
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
#include "argument_checker.h"

/**
 * Diese Funktion prueft ob nur gueltige Parameter angegeben wurden.
 * Bei ungueltigen Parametern wird das Programm beendet.
 * 
 * @param argv Parameterliste
 */
static void check_for_unknown_parameter(char **argv);

/** Diese Funktion gibt die Hilfe auf dem Bildschirm aus. */
static void print_help();


/** ---------------------------------------------------------------------------
 *  Funktion: check_arguments
 *  ------------------------------------------------------------------------ */
extern void check_arguments(int argc,
                              char** argv,
                              char **in_filename,
                              char **out_filename)
{
    char **p_argv_start = argv;
    
    /* Pruefen ob keine Parameter angegeben wurden. */
    if (argc < 2)
    {
        printf("Sie haben keinen Parameter angegeben!\n"
                "Bitte geben Sie mindestens einen Parameter an.\n");
        print_help();
        exit(EXIT_FAILURE);
    }
    
    /* Pruefen ob zu viele Parameter angegeben wurden. */
    if (argc > 5)
    {
        printf("Sie haben zu viele Parameter angegeben!\n"
                "Fuer eine Info ueber die verfuegbaren Parameter geben.\n");
        print_help();
        exit(EXIT_FAILURE);
    }
    
    argv++;
    check_for_unknown_parameter(argv);
    
    /* Anzeigen der Hilfe. */
    if (argc == 2 && strcmp(*argv, "-h") == 0)
    {
        print_help();
        exit(EXIT_SUCCESS);
    }
    
    /* Pruefen ob eine Inputdatei angegeben wurde. */
    if (argc == 2 && (strcmp(*argv, "-c") == 0 || strcmp(*argv, "-d") == 0))
    {
        printf("Geben Sie eine Input Datei an!\n");
        print_help();
        exit(EXIT_FAILURE);
    } 
    else if (argc == 3 && (strcmp(*argv, "-c") == 0 || strcmp(*argv, "-d") == 0) 
            && (strcmp(*(argv + 1), "-debug") == 0))
    {
        printf("Geben Sie eine Input Datei an!\n");
        print_help();
        exit(EXIT_FAILURE);
    }
    
    /* Pruefen ob Kompression erwuenscht. */
    if (argc > 2 && strcmp(*argv, "-c") == 0)
    {
        compress_mode = TRUE;
        *in_filename = *(argv + 1);

        /* 
         * Setzen des out_filename aus den Parametern.
         * Wenn kein out_filename angegeben wurde, wird Speicher allokiert fuer
         * den in_filename + Kompressionsdateinamen. Dies bildet dann den
         * out_filename.
         */
        if (argc >= 4 && strcmp(*(argv + 2), "-debug") != 0)
        {
            *out_filename = *(argv + 2);
        }
        else
        {
            *out_filename = malloc((strlen(*in_filename) 
                    + strlen(COMPRESS_EXT)) * sizeof(char));
            strcpy(*out_filename, *in_filename);
            strcat(*out_filename, COMPRESS_EXT);
        }
    }
    
    /* Pruefen ob Dekompression erwuenscht. */
    if (argc > 2 && strcmp(*argv, "-d") == 0)
    {
        compress_mode = FALSE;
        *in_filename = *(argv + 1);
        
        /* 
         * Setzen des out_filename aus den Parametern.
         * Wenn kein out_filename angegeben wurde, wird Speicher allokiert fuer
         * den in_filename + Kompressionsdateinamen. Dies bildet dann den
         * out_filename.
         */
        if (argc >= 4 && strcmp(*(argv + 2), "-debug") != 0)
        {
            *out_filename = *(argv + 2);
        }
        else
        {
            *out_filename = malloc((strlen(*in_filename) 
                    + strlen(DECOMPRESS_EXT)) * sizeof(char));
            strcpy(*out_filename, *in_filename);
            strcat(*out_filename, DECOMPRESS_EXT);
        }
    }
    
    /*
     * Aktivierung des globalen Debug Modus fuer globale Ausgaben.
     */
    if (strcmp(*(p_argv_start + argc - 1), "-debug") == 0)
    {
        printf("\n\n\t**********************************\n");
        printf("\t** Debug Modus wurde aktiviert. **\n");
        printf("\t**********************************\n\n");
        fflush(stdout);
        debug_mode = TRUE;
    }
}

/** ---------------------------------------------------------------------------
 *  Funktion: check_for_unknown_parameter
 *  ------------------------------------------------------------------------ */
static void check_for_unknown_parameter(char *argv[])
{
    if (strcmp(*argv, "-c") != 0 && strcmp(*argv, "-d") != 0 
            && strcmp(*argv, "-h") != 0)
    {
        if (strcmp(*argv, "-debug") == 0)
        {
            printf("Geben Sie den -debug Parameter bitte nur in Verbindung "
                    "mit -c oder -d an.!\n");
        }
        else
        {
            printf("Sie haben einen ungueltigen Parameter angegeben!\n");
        }
        print_help();
        exit(EXIT_FAILURE);
    }
}

/** ---------------------------------------------------------------------------
 *  Funktion: print_help
 *  ------------------------------------------------------------------------ */
static void print_help()
{
    printf("Hilfe:\n"
                "-h zum Aufrufen der Hilfe.\n"
                "-c zum Komprimieren einer Datei: -c Eingabedatei "
            "[Ausgabedatei] [-debug]\n"
                "-d zum Dekomprimieren einer Datei: -d Eingabedatei "
            "[Ausgabedatei] [-debug]\n");
}
