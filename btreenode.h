/**
 * @file
 * Dieses Modul definiert Knoten von Binärbäumen und stellt Funktionen
 * darauf zur Verfügung.
 *
 * @author  Ulrike Griefahn
 * @date    2010-09-02
 */

#ifndef _BTREENODE_H
#define _BTREENODE_H

/* ===========================================================================
 * Header-Dateien
 * ======================================================================== */

#include "common.h"

/* ========================================================================= *
 * Strukturen, Typdefinitionen
 * ========================================================================= */

/**
 * Struktur für ein Knoten den Binärbaumes.
 */
typedef struct _BTREE_NODE {
    /**
     * Verweis auf den Inhalt des Knoten
     */
    void *data;
    /**
     * Verweis auf das linke Kindelement
     */
    struct _BTREE_NODE *left;
    /**
     * Verweis auf das recht Kindelement
     */
    struct _BTREE_NODE *right;
} BTREE_NODE;

/**
 * Typdefinition für Funktionen, mit denen Knoteninhalte gelöscht werden können.
 */
typedef void (*DESTROY_FCT)(void **);

/**
 * Typdefinition für Funktionen, mit denen Knoteninhalte angezeigt werden können.
 */
typedef void (*PRINT_FCT)(void *);

/* ===========================================================================
 * Funktionsprototypen
 * ======================================================================== */

/**
 * Erzeugt einen neuen Knoten mit den übergebenen Daten. Der neue Knoten hat 
 * keine Nachfolger.
 *
 * @param data      Daten des neuen Knotens(*tree)->destroy_dat
 * @return          Der neu erzeugte Knoten
 */
extern BTREE_NODE *btreenode_new(void *data);

/**
 * Erzeugt eine Kopie (deep copy) des Knotens und seiner direkten und 
 * indirekten Nachfolger. Die Daten in den Knoten werden nicht kopiert, 
 * sondern nur ihre Referenz in die neuen Knoten übernommen.
 * 
 * @param node  Knoten, ab dem der Binärbaum kopiert werden soll
 * @return      Die neu erzeugte Kopie des übergebenen Knotens
 */
extern BTREE_NODE *btreenode_clone(BTREE_NODE *node);

/**
 * Liefert TRUE, wenn die beiden übergebenen Knoten dieselben Daten beinhalten
 * und ihre Nachfolgerknoten ebenfalls gleich sind.
 * 
 * @param node1     der erste zu vergleichende Knoten
 * @param node2     der zweite zu vergleichende Knoten
 * @return          TRUE, wenn die beiden Knoten gleich sind, FALSE sonst
 */
extern BOOL btreenode_equals(BTREE_NODE *node1, BTREE_NODE *node2);

/**
 * Löscht den übergebenen Knoten und alle seine direkten und indirekten
 * Nachfolger. Im Parameter destroy_data kann eine Funktion übergeben werden,
 * mit der die Daten der Knoten gelöscht werden. Die Daten werden nicht
 * gelöscht, wenn in diesem Parameter NULL übergeben wird.
 *
 * @param node          Der zu löschende Knoten
 * @param destroy_data  Funktion zum Löschen der Daten, NULL sonst
 */
extern void btreenode_destroy(BTREE_NODE **node, DESTROY_FCT destroy_data);

/**
 * Liefert die Daten des Knotens.
 * 
 * @param node      Knoten, dessen Daten geliefert werden sollen
 * @return          Daten des Knoten oder NULL, wenn der Knoten keine Daten hat
 *                  oder kein Knoten übergeben wurde.
 */
extern void *btreenode_get_data(BTREE_NODE *node);

/**
 * Liefert den linken Nachfolger des Knotens.
 * 
 * @param node      Knoten, dessen linker Nachfolger geliefert werden soll
 * @return          linker Nachfolger des Knotens oder NULL, wenn der Knoten
 *                  keinen linken Nachfolger hat oder kein Knoten übergeben
 *                  wurde
 */
extern BTREE_NODE *btreenode_get_left(BTREE_NODE *node);

/**
 * Liefert den rechten Nachfolger des Knotens.
 * 
 * @param node      Knoten, dessen rechter Nachfolger geliefert werden soll
 * @return          rechter Nachfolger des Knotens oder NULL, wenn der Knoten
 *                  keinen rechten Nachfolger hat oder kein Knoten 
 *                  übergeben wurde
 */
extern BTREE_NODE *btreenode_get_right(BTREE_NODE *node);

/**
 * Prüft, ob der übergebene Knoten ein Blatt ist.
 *
 * @param node      Knoten, der geprüft werden soll
 * @return          Liefert TRUE, wenn der Knoten ein Blatt ist, FALSE sonst
 */
extern BOOL btreenode_is_leaf(BTREE_NODE *node);

/**
 * Fügt einen Knoten als linken Nachfolger an einen Elternknoten an, falls 
 * dieser Knoten noch keinen linken Nachfolger hat.
 *
 * @param parent_node   der Elternknoten
 * @param node          der neue Nachfolgerknoten
 * @return              TRUE, wenn der Nachfolger gesetzt werden konnte,
 *                      FALSE, wenn es schon einen Nachfolger gibt
 */
extern BOOL btreenode_set_left(BTREE_NODE *parent_node, BTREE_NODE *node);

/**
 * Fügt einen Knoten als rechten Nachfolger an einen anderen an, falls der 
 * Knoten noch keinen rechten Nachfolger hat.
 *
 * @param parent_node   der Elternknoten
 * @param node          der neue Nachfolgerknoten
 * @return              TRUE, wenn der Nachfolger gesetzt werden konnte,
 *                      FALSE, wenn es schon einen Nachfolger gibt
 */
extern BOOL btreenode_set_right(BTREE_NODE *parent_node, BTREE_NODE *node);

/**
 * Gibt den Knoten auf dem Bildschirm aus.
 * Im zweiten Argument kann eine Funktion zum Ausgeben der im Knoten
 * enthaltenen Daten übergeben werden. Wird NULL übergeben, werden die Daten
 * des Knotens nicht ausgegeben.
 *
 * @param node          der auszugebende Knoten
 * @param print_data    Funktion zur Ausgabe der entahltenen Daten
 * @return              Textdarstellung des Knotens
 */
extern void btreenode_print(BTREE_NODE *node, PRINT_FCT print_data);

/* ------------------------------------------------------------------------ */
#endif
