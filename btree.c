/** 
 * File: btree.c
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

#include <stdlib.h>
#include <stdio.h>
#include "btree.h"
#include "common.h"

/**
 * Ermittelt rekursiv die Tiefe ab dem uebergebenen Knoten.
 * 
 * @param node Wurzelknote ab dem die Tiefer ermittelt wird
 * @param depth Tiefe zur Uebergabe an hoehere Rekursionsstufen
 * @return Die errechnete Tiefe
 */
static unsigned int btree_get_depth_recursive(BTREE_NODE *node,
                                              unsigned int depth);

/**
 * Ausgabe eines Knotens inkl. seiner Kindsknoten in Preorder Reihenfolge.
 *  
 * @param node Knoten der ausgegeben werden soll
 * @param intent Einrueckung, zu beginn am idealerweise 0
 * @param print_data Funktion f�r das ausgeben der Inhalte eines Knotens
 */
static void print_pre_order(BTREE_NODE *node, PRINT_FCT print_data, int intent)
{
    int i;
    
    /* Einrueckung */
    printf("|");
    for (i = 0; i < intent; i++) printf("-");

    /* Ausgabe des Knoten. */
    btreenode_print(node, print_data);    
    
    /* Preorder heisst zunaechst den Linken Kindsknoten ausgeben. */
    if (node->left != NULL) print_pre_order(node->left,
                                           print_data,
                                           intent + 1);
                                           
    if (node->right != NULL) print_pre_order(node->right,
                                            print_data,
                                            intent + 1);
}

/* ---------------------------------------------------------------------------
 * Funktion: btree_new
 * ------------------------------------------------------------------------ */
extern BTREE *btree_new(void *data,
                        DESTROY_FCT destroy_data, 
                        PRINT_FCT print_data)
{
    BTREE_NODE *root_node;
    
    /* Speicher fuer Baum allokieren. */
    BTREE *btree = calloc(1,sizeof(BTREE));
    ENSURE_ENOUGH_MEMORY(btree, "btree_new");
    
    /* Wurzelknoten erzeugen. */
    root_node = btreenode_new(data);
    
    /* Komponenten Initialisieren. */
    btree->root = root_node;
    btree->destroy_data = destroy_data;
    btree->print_data = print_data;
    
    return btree;
}

/* ---------------------------------------------------------------------------
 * Funktion: btree_clone
 * ------------------------------------------------------------------------ */
extern BTREE *btree_clone(BTREE *tree)
{
    BTREE_NODE *root_node;
    BTREE_NODE *clone_root_node;
    BTREE      *clone_tree = NULL;
    
    if (tree != NULL) 
    {
        /* Kopieren der Daten aus dem Wurzelknoten. */
        root_node = btree_get_root(tree);

        /* Erzeuge neunen Baum (Klon). */
        clone_tree = btree_new(btreenode_get_data(root_node),
                               tree->destroy_data,
                               tree->print_data);    

        /* Klonen der Kinder. */
        clone_root_node = btree_get_root(clone_tree);
        if (btreenode_get_left(root_node) != NULL)
            btreenode_set_left(clone_root_node,
                               btreenode_clone(btreenode_get_left(root_node)));
        if (btreenode_get_right(root_node) != NULL)
            btreenode_set_right(clone_root_node,
                                btreenode_clone(btreenode_get_right(root_node)));
    }
    
    return clone_tree;
}

/* ---------------------------------------------------------------------------
 * Funktion: btree_destroy
 * ------------------------------------------------------------------------ */
extern void btree_destroy(BTREE **tree, BOOL destroy_data)
{
    if (tree != NULL && *tree != NULL)
    {
        /* Kinds knoten zerstoeren */
        btreenode_destroy(&((*tree)->root), (destroy_data) 
                ? (*tree)->destroy_data : NULL);

        /* Komponenten freigeben (evtl. nicht gewollt)*/
        /*if ((*tree)->destroy_data == NULL)  free(&((*tree)->destroy_data));*/
        /*if ((*tree)->print_data == NULL)    free(&((*tree)->print_data));*/

        /* Baum freigeben */
        free(*tree);
        tree = NULL;
    }
}

/* ---------------------------------------------------------------------------
 * Funktion: btree_equals
 * ------------------------------------------------------------------------ */
extern BOOL btree_equals(BTREE *tree1, BTREE *tree2)
{
    BOOL equals = FALSE;
    
    /* Pruefen ob einer der Baeume NULL ist, dann Programm beenden. */
    if (tree1 != NULL && tree2 != NULL)
    {
        equals = btreenode_equals(tree1->root, tree2->root);
    }
    
    return equals;
}

/* ---------------------------------------------------------------------------
 * Funktion: btree_get_root
 * ------------------------------------------------------------------------ */
extern BTREE_NODE *btree_get_root(BTREE *tree)
{
    BTREE_NODE *root = NULL;
    
    if (tree != NULL)
    {
        root = tree->root;
    }
    
    return root;
}

/* ---------------------------------------------------------------------------
 * Funktion: btree_get_depth_recursive
 * ------------------------------------------------------------------------ */
static unsigned int btree_get_depth_recursive(BTREE_NODE *node,
                                              unsigned int depth)
{   
    unsigned int depth_left = 0;
    unsigned int depth_right = 0;
    
    if (btreenode_get_left(node) != NULL)
    {
        depth_left = btree_get_depth_recursive(btreenode_get_left(node),
                                               depth + 1);
    }

    if (btreenode_get_right(node) != NULL)
    {
        depth_right = btree_get_depth_recursive(btreenode_get_right(node),
                                               depth + 1);
    }
    
    return (depth_left > depth_right) ? depth_left + depth : depth_left + depth;
}

/* ---------------------------------------------------------------------------
 * Funktion: btree_get_max_depth
 * ------------------------------------------------------------------------ */
extern int btree_get_max_depth(BTREE_NODE *node)
{
    if (node == NULL) return -1;
    else return (int) btree_get_depth_recursive(node, 1);
}

/* ---------------------------------------------------------------------------
 * Funktion: btree_merge
 * ------------------------------------------------------------------------ */
extern BTREE *btree_merge(BTREE *left, BTREE *right, void *data)
{
    BTREE       *merged_tree = NULL;
    BTREE_NODE  *merged_root_node;
    
    if (left != NULL && right != NULL)
    {
        /* Erzeuge einen neuen Baum mit uebergebenem Datum und Funktionen des
         * linken Baumes.
         */
        merged_tree = btree_new(data, left->destroy_data, left->print_data);

        /* Fuege beide Teilbaeume zu einer Wurzel hinzu um sie zu mergen. */
        merged_root_node = merged_tree->root;
        btreenode_set_left(merged_root_node, left->root);
        btreenode_set_right(merged_root_node, right->root);
    }
    
    return merged_tree;
}

/* ---------------------------------------------------------------------------
 * Funktion: btree_print
 * ------------------------------------------------------------------------ */
extern void btree_print(BTREE *tree)
{
    if (tree != NULL)
    {    
        print_pre_order(tree->root, tree->print_data, 0);
    }
}
