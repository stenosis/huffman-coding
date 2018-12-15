/** 
 * File: btreenode.c
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
#include "btreenode.h"
#include "common.h"

/** ---------------------------------------------------------------------------
 *  Funktion: btreenode_new
 *  ------------------------------------------------------------------------ */
extern BTREE_NODE *btreenode_new(void *data)
{
    /* Speicher fuer Knoten allokieren. */
    BTREE_NODE *node = calloc(1,sizeof(BTREE_NODE));
    ENSURE_ENOUGH_MEMORY(node, "btreenode_new");
    
    /* Komponenten Initialisieren. */
    node->data = data;
    node->right = NULL;
    node->left = NULL;
    
    return node;
}

/** ---------------------------------------------------------------------------
 *  Funktion: btreenode_clone
 *  ------------------------------------------------------------------------ */
extern BTREE_NODE *btreenode_clone(BTREE_NODE *input_node)
{
    BTREE_NODE *node = NULL;
    
    if (input_node != NULL)
    {
        /* Neuen Wurzelknoten erzeugen. */
        node = btreenode_new(input_node->data);

        /* Komponenten kopieren (Rekursive deep copy). */
        if (input_node->left != NULL)
        {
            btreenode_set_left(node, btreenode_clone(input_node->left));
        }
            
        if (input_node->right != NULL)
        {
            btreenode_set_right(node, btreenode_clone(input_node->right));
        }
    }
    
    return node;
}

/** ---------------------------------------------------------------------------
 *  Funktion: btreenode_equals
 *  ------------------------------------------------------------------------ */
extern BOOL btreenode_equals(BTREE_NODE *node1, BTREE_NODE *node2)
{
    BOOL equals = FALSE;

    if (BOTH_NOT_NULL(node1,node2))
    {
        /* Ist der Inhalt identisch ? */
        equals = node1->data == node2->data;

        /* Wenn Inhalt gleich dann muessen auch die Kindsknoten gleich sein. */
        if (equals)
        {
            if (BOTH_NOT_NULL(node1->left, node2->left))
            {
                equals &= btreenode_equals(node1->left, node2->left);
            } 
            else 
            {
                equals &= BOTH_NULL(node1->left,node2->left);
            }
        }    

        if (equals)
        {
            if (BOTH_NOT_NULL(node1->right, node2->right))
            {
                equals &= btreenode_equals(node1->right, node2->right);
            } 
            else 
            {
                equals &= BOTH_NULL(node1->right,node2->right);
            }
        }
    }
    
    return equals;
}

/** ---------------------------------------------------------------------------
 *  Funktion: btreenode_destroy
 *  ------------------------------------------------------------------------ */
extern void btreenode_destroy(BTREE_NODE **node, DESTROY_FCT destroy_data)
{

    if (BOTH_NOT_NULL(node, *node))
    {
        /* Kindsknoten Zerstoeren */
        if (btreenode_get_left(*node) != NULL)
        {
            btreenode_destroy(&((*node)->left), destroy_data);
        }
            
        if (btreenode_get_right(*node) != NULL)
        {
            btreenode_destroy(&((*node)->right), destroy_data);
        }
            
        /* Wenn eine Funktionuebergeben wurde soll der Inhalt zerstoert werden. */
        if (destroy_data != NULL)
        {
            destroy_data(&((*node)->data));
        }

        /* Zerstoerung des Vaterelements nachdem die Kinder vernichtet wurden. */
        free(*node);
        *node = NULL;
    }
}

/** ---------------------------------------------------------------------------
 *  Funktion: btreenode_get_data
 *  ------------------------------------------------------------------------ */
extern void *btreenode_get_data(BTREE_NODE *node)
{
    void *return_data = NULL;
    
    if (node != NULL) 
    {
        return_data = node->data;
    }
    
    return return_data;
}

/** ---------------------------------------------------------------------------
 *  Funktion: btreenode_get_left
 *  ------------------------------------------------------------------------ */
extern BTREE_NODE *btreenode_get_left(BTREE_NODE *node)
{
    BTREE_NODE *return_node = NULL;
    
    if (node != NULL) 
    {
        return_node = node->left;
    }
    
    return return_node;
}

/** ---------------------------------------------------------------------------
 *  Funktion: btreenode_get_right
 *  ------------------------------------------------------------------------ */
extern BTREE_NODE *btreenode_get_right(BTREE_NODE *node)
{
    BTREE_NODE *return_node = NULL;
    
    if (node != NULL) 
    {
        return_node = node->right;
    }
    
    return return_node;
}

/** ---------------------------------------------------------------------------
 *  Funktion: btreenode_is_leaf
 *  ------------------------------------------------------------------------ */
extern BOOL btreenode_is_leaf(BTREE_NODE *node)
{
    BOOL is_leaf = FALSE;
    
    if (node != NULL) 
    {
        is_leaf = BOTH_NULL(node->left, node->right);
    }
    
    return is_leaf;
}

/** ---------------------------------------------------------------------------
 *  Funktion: btreenode_set_left
 *  ------------------------------------------------------------------------ */
extern BOOL btreenode_set_left(BTREE_NODE *parent_node, BTREE_NODE *node)
{
    BOOL success = FALSE;
    
    /* Pruefen ob der uebergebene Knoten NULL ist, dann Programm beenden. */
    if (parent_node == NULL)
    {
        ERROR_STOP_EXECUTION("btreenode_set_left",
                             "parameter parent_node == NULL");
    }
    
    /* Nur wenn kein Nachfolger existiert wird einer gesetzt. */
    if (parent_node->left == NULL && node != NULL)
    {
        parent_node->left = node;
        success = TRUE;
    }
    
    return success;
}

/** ---------------------------------------------------------------------------
 *  Funktion: btreenode_set_right
 *  ------------------------------------------------------------------------ */
extern BOOL btreenode_set_right(BTREE_NODE *parent_node, BTREE_NODE *node)
{
    BOOL success = FALSE;
    
    /* Pruefen ob der uebergebene Knoten NULL ist, dann Programm beenden. */
    if (parent_node == NULL)
    {
        ERROR_STOP_EXECUTION("btreenode_set_right",
                             "parameter parent_node == NULL");
    }
    
    /* Nur wenn kein Nachfolger existiert wird einer gesetzt. */
    if (parent_node->right == NULL && node != NULL)
    {
        parent_node->right = node;
        success = TRUE;
    }
    
    return success;
}

/** ---------------------------------------------------------------------------
 *  Funktion: btreenode_print
 *  ------------------------------------------------------------------------ */
extern void btreenode_print(BTREE_NODE *node, PRINT_FCT print_data)
{
    /* Pruefen ob der uebergebene Knoten NULL ist, dann Programm beenden */
    if (node == NULL)
    {
        ERROR_STOP_EXECUTION("btreenode_print",
                             "parameter node == NULL");
    }
    
    /* Ausgabe der Adresse */
    printf("%p: ", (void*) node);
    if (print_data != NULL) print_data(node->data);
}
