/**
 * @file
 * Dieses Modul stellt eine Struktur zur Repraesentation von Binaerbaeumen
 * und Funktionen darauf zur Verfuegung.
 *
 * @author  Ulrike Griefahn
 * @date    2010-09-02
 */ 

#ifndef _BTREE_H
#define _BTREE_H
/* ------------------------------------------------------------------------ */


/* ===========================================================================
 * Header-Dateien 
 * ======================================================================== */

#include "btreenode.h"

/* ========================================================================= *
 * Strukturen, Typdefinitionen
 * ========================================================================= */

/**
 * Struktur fuer einen Binaerbaum. Enthaelt Verweis auf Wurzelknoten.
 */
typedef struct _BTREE {
    /**
     * Fkt. zum Loeschen der enthaltenen Daten
     */
    DESTROY_FCT destroy_data;
    /**
     * Fkt. zum Anzeigen der enthaltenen Daten
     */
    PRINT_FCT print_data;
    /**
     * Verweis auf das Wurzelelement
     */    
    BTREE_NODE  *root;
} BTREE;

/* ===========================================================================
 * Funktionsprototypen
 * ======================================================================== */

/**
 * Erzeugt einen neuen Binaerbaum, dessen Wurzel die uebergebenen Daten 
 * beinhaltet. Im zweiten Parameter wird die Funktion uebergeben, mit der die
 * enthaltenen Daten geloescht werden koennen, im dritten Parameter wird die
 * Funktion Uebergeben, mit der die Daten am Bildschirm ausgegeben werden koennen.
 * In beiden Parametern kann NULL uebergeben werden, so dass keine Funktionen
 * zum Ausgeben und Loeschen der Knotendaten gesetzt sind.
 * 
 * @param data          Daten fuer die Wurzel des neuen Baums
 * @param destroy_data  Funktion, mit der die in den Knoten
 *                      enthaltenen Daten geloescht werden koennen oder NULL
 * @param print_data    Funktion, mit der die in den Knoten
 *                      enthaltenen Daten am Bildschirm ausgegeben werden
 *                      koennen oder NULL
 * @return              der neu erzeugte Binaerbaum
 */
extern BTREE *btree_new(void *data,
                        DESTROY_FCT destroy_data, PRINT_FCT print_data);

/**
 * Erzeugt eine Kopie (deep copy) des Uebergebenen Binaerbaums. Es werden die
 * Struktur des Baums und die Knoten kopiert. Die in den Knoten enthaltenen
 * Daten werden nicht kopiert, sondern nur die Referenz auf sie �bernommen.
 * Im Anschluss an die Funktion verweisen beide B�ume auf dieselben Daten.
 * 
 * Es wird davon ausgegangen, dass tree != NULL ist.
 * 
 * @param tree  Binaerbaum, der kopiert werden soll
 * @return      die neu erzeugte Kopie
 */
extern BTREE *btree_clone(BTREE *tree);

/**
 * Loescht den uebergebenen Baum, d.h. alle Knoten ausgehend von der Wurzel und 
 * die Struktur fuer den Baum. Im zweiten Argument wird TRUE uebergeben, wenn
 * die in den Knoten enthaltenen Daten ebenfalls geloescht werden sollen, sonst
 * FALSE.
 * 
 * @param tree          Baum, der geloescht werden soll
 * @param destroy_data  TRUE, wenn die Daten geloescht werden sollen,
 *                      FALSE sonst
 */
extern void btree_destroy(BTREE **tree, BOOL destroy_data);

/**
 * Vergleicht die beiden uebergebenen Binaerbaeume miteinander. Zwei Binaerbaeume
 * sind gleich, wenn  beide Baeume ab dem Wurzelknoten dieselbe Struktur
 * besitzen und in allen Knoten auf dieselben Daten verweisen. Sie koennen sich
 * in den gespeicherten Funktionen unterscheiden.
 *
 * @param tree1     der erste zu vergleichende Binaerbaum
 * @param tree2     der zweite zu vergleichende Binaerbaum
 * @return          TRUE, wenn beide B�ume gleich sind, FALSE sonst
 */
extern BOOL btree_equals(BTREE *tree1, BTREE *tree2);

/**
 * Liefert die Wurzel des uebergebenen Binaerbaums.
 * 
 * @param tree  Binaerbaum, dessen Wurzel geliefert werden soll
 * @return      Wurzel des Baums oder NULL, wenn der Baum keine Wurzel hat
 *              oder kein Baum uebergeben wurde
 */
extern BTREE_NODE *btree_get_root(BTREE *tree);

/**
 * Liefert die maximale Tiefe eines Baumes.
 * 
 * @param node Knoten ab dem die Tiefer ermittelt werden soll
 * @return -1 wenn Fehler auftreten sonst die maximale Tiefe
 */
extern int btree_get_max_depth(BTREE_NODE *node);

/**
 * Erzeugt einen neuen Binaerbaum, dessen Wurzel die uebergebenen Daten 
 * erhaelt. Die neue Wurzel bekommt die Wurzel des Binaerbaums left als
 * linken Nachfolger und die Wurzel des Binaerbaums right als rechten 
 * Nachfolger. Der neue Baum erhaelt die Funktionen aus dem Baum, der als linker
 * Nachfolger angefuegt wird.
 * 
 * Der Speicherbereich der beiden verbundenen B�ume, jedoch nicht ihrer Knoten 
 * wird freigegeben.
 * 
 * @param left      die erste zu verbindenden Baeume
 * @param right     der zweite der beiden zu verbindenden Baeume
 * @param data      die Daten fuer die Wurzel im neuen Baum
 * @return          der neu erzeugte Baum
 */
extern BTREE *btree_merge(BTREE *left, BTREE *right, void *data);

/**
 * Gibt den Binaerbaum mit seinen Daten in Preorder-Reihenfolge auf dem 
 * Bildschirm aus.
 *
 * @param tree      Binaerbaum, der ausgegeben werden soll
 */
extern void btree_print(BTREE *tree);

/* ------------------------------------------------------------------------ */
#endif
