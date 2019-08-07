/*

     dlink_list.c

     This is an open source library file which may be freely copied,
     distributed, and used for any lawful purpose.  If you use
     this code in a copyrighted work, your copyright only applies
     to the work you use it with and any changes that you make to
     this file.  Including this code in your work does not cause
     this code to become your property.  This stipulation supercedes
     any language to the contrary that anyone includes in their
     documentation.

     Written by Matthew Campbell.

     I started writing this file on Saturday June 16, 2018
     and didn't finish it until Tuesday August 14, 2018.

     This was a lot of work.  I couldn't seem to find the free time
     to work on it.  This code may be useful to intermediate level
     programmers who want to learn how to use structures and doubly
     linked lists.  It may be useful to intermediate and advanced
     level users that don't want to have to type in all this code.

     This file is 91,984 bytes long and contains 3,725 lines.

*/

#ifndef _DLINK_LIST_C
#define _DLINK_LIST_C

#include <math.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dlink_list.h"

/* Defined types: */

/*

     If you change the data fields of the structure node_def
     then you may have to modify the following functions to
     accomodate your changes:

     make_node()
     release_node()
     release_all_nodes()
     free_node()
     free_all_nodes()
     find_node_by_number()
     find_lowest_node_number()
     find_highest_node_number()
     check_list_integrity()
     check_free_list_integrity()
     read_list_from_file()
     write_list_to_file()

*/

/* Functions in this file: */

node_t *make_node( void );
node_t *get_new_node( void );

int extract_node( node_t *node );

int release_node( node_t *node );
int release_all_nodes( void );
int free_node( node_t *node );
int free_all_nodes( void );

int insert_node_as_next( node_t *current, node_t *new_node );
int insert_node_as_previous( node_t *current, node_t *new_node );

int swap_nodes( node_t *node1, node_t *node2 );

void list_nodes( void );
void list_free_nodes( void );

void display_node_data( node_t *ptr );

node_t *find_nth_node( const int number );
node_t *find_node_by_number( const int num );

int find_lowest_node_number( void );
int find_highest_node_number( void );

int count_list_nodes( void );
int count_free_nodes( void );

int convert_list_to_circular( void );
int convert_list_to_linear( void );

const char *error_string( void );

int check_list_integrity( void );
int check_free_list_integrity( void );

void write_input( const char *str );
void write_output( const char *str );

void open_outfile( const char *name );
void close_outfile( void );

int read_list_from_file( const char *name );
int write_list_to_file( const char *name );

/* Global variables: */

node_t *free_nodes, *last_free_node, *last_list_node, *list;
int error_code, free_size, list_size, using_circular;

#ifdef USE_MAGIC_NUMBER

static const int num_error_codes = 8;

#else

static const int num_error_codes = 7;

#endif

FILE *outfile;

/* Function definitions: */

/* Creates a new node with malloc() and returns it or NULL if it can't. */

node_t *make_node( void )
{
     node_t *node;

     node = malloc( sizeof( node_t ) );
     if ( node == NULL )
     {
          error_code = no_malloc;
          return NULL;
     }

#ifdef USE_MAGIC_NUMBER

     node->magic = NODE_MAGIC_NUMBER;
     node->pad = 0;

#endif

     node->num = 0;
     node->next = NULL;
     node->prev = NULL;
     error_code = success;
     return node;
}

/* Finds a new node and, if successful, returns it, or NULL otherwise. */

node_t *get_new_node( void )
{
     node_t *node;

     if ( free_nodes == NULL )
     {
          node = make_node();
     }
     else
     {
          node = free_nodes;
          free_nodes = free_nodes->next;
          if ( free_nodes != NULL )
          {
               free_nodes->prev = NULL;
          }
          else
          {
               last_free_node = NULL;
          }
          free_size--;
          node->next = NULL;
     }
     return node;
}

/*

     Removes 'node' from the list without
     moving it to the list of available nodes.

     Returns 0 for success or 1 if an error occured.

*/

int extract_node( node_t *node )
{
     node_t *tmp;

     if ( node == NULL )
     {
          error_code = pointer;
          return 1;
     }

#ifdef USE_MAGIC_NUMBER

     if ( node->magic != NODE_MAGIC_NUMBER )
     {
          error_code = bad_magic;
          return 1;
     }

#endif

     /* We can't remove the node from the list if we don't have a list. */

     if ( list == NULL )
     {
          error_code = bad_arg;
          return 1;
     }

     /* Find out if the node is in the list. */

     tmp = list;

     /* This method accomodates circularly linked lists. */

     while( tmp != node && tmp != last_list_node )
     {
          tmp = tmp->next;
     }
     if ( tmp != node )  /* The node was not found in the list. */
     {
          error_code = bad_arg;
          return 1;
     }

     /* Update the links. */

     tmp = node->prev;
     if ( tmp != NULL )
     {
          if ( node->next != NULL )
          {
               if ( using_circular == 1 )
               {
                    if ( list_size == 1 )
                    {
                         last_list_node = list = NULL;
                    }
                    else if ( list_size == 2 )
                    {
                         if ( list == node )  /* 'node' is first. */
                         {
                              list = node->next;
                              list->next = list;
                              list->prev = list;
                         }
                         else  /* 'node' is last. */
                         {
                              list->next = list;
                              list->prev = list;
                              last_list_node = list;
                         }
                    }
                    else  /* list_size > 2 */
                    {
                         if ( list == node )  /* 'node' is first. */
                         {
                              list = node->next;
                              list->prev = tmp;
                              tmp->next = list;
                         }
                         else if ( last_list_node == node )
                         {
                              tmp->next = list;
                              list->prev = tmp;
                              last_list_node = tmp;
                         }
                         else  /* 'node' is neither first nor last. */
                         {
                              tmp->next = node->next;
                              tmp = node->next;
                              tmp->prev = node->prev;
                         }
                    }    /* if ( list_size == 0 ) */
               }
               else  /* using_circular == 0 */
               {
                    tmp->next = node->next;
                    tmp = node->next;
                    tmp->prev = node->prev;
               }
          }
          else  /* Last node in the list. */
          {
               tmp->next = NULL;
               last_list_node = tmp;
          }
     }
     else  /* First node in the list. */
     {
          tmp = node->next;
          if ( tmp != NULL )
          {
               list = tmp;
               tmp->prev = NULL;
          }
          else  /* And the last node in the list. */
          {
               last_list_node = list = NULL;
          }
     }

     /* Clear the links on 'node' to show that it has been extracted. */

     node->next = NULL;
     node->prev = NULL;

     list_size--;
     error_code = success;
     return 0;
}

/*

     This function removes 'node' from the list and adds it to
     'free_nodes'.  If it has already been removed from the list
     with extract_node() then it will be moved to 'free_nodes'.

     Returns 0 for success or 1 if an error occurred.

*/

int release_node( node_t *node )
{
     node_t *tmp;

     if ( node == NULL )
     {
          error_code = pointer;
          return 1;
     }

#ifdef USE_MAGIC_NUMBER

     if ( node->magic != NODE_MAGIC_NUMBER )
     {
          error_code = bad_magic;
          return 1;
     }

#endif

     /* We need to see if this 'node' has already been extracted. */

     if ( node->next == NULL && node->prev == NULL )
     {
          tmp = list;
          if ( list != NULL )
          {
               while( tmp != node && tmp != last_list_node )
               {
                    tmp = tmp->next;
               }
          }

          /*

               If 'tmp' == 'node' and 'node' isn't the only node
               in the list, then the list is broken and therefore
               corrupted.

          */

          if ( tmp == node && list != node )
          {
               error_code = bad_list;
               return 1;
          }

          /* If 'tmp' != 'node' then the 'node' is not in the list. */

          if ( tmp != node )
          {

               /* Clear the node's data. */

#ifdef USE_MAGIC_NUMBER

               if ( node->magic != NODE_MAGIC_NUMBER )
               {
                    error_code = bad_magic;
                    return 1;
               }
               node->pad = 0;

#endif

               node->num = 0;

               /* Move 'node' to the list of available nodes. */

               if ( free_nodes != NULL )
               {
                    node->next = free_nodes;
                    free_nodes->prev = node;
               }
               else
               {
                    last_free_node = node;
               }
               free_nodes = node;
               free_size++;
               error_code = success;
               return 0;
          }
     }    /* if ( node->next == NULL && node->prev == NULL ) */

     /* Clear the node's data. */

#ifdef USE_MAGIC_NUMBER

     if ( node->magic != NODE_MAGIC_NUMBER )
     {
          error_code = bad_magic;
          return 1;
     }
     node->pad = 0;

#endif

     node->num = 0;

     /* Update the links. */

     tmp = node->prev;
     if ( tmp != NULL )
     {
          if ( using_circular == 1 )
          {
               if ( list == node )  /* 'node' is first. */
               {
                    if ( node->next == node )  /* Only 1 node. */
                    {
                         list = NULL;
                         last_list_node = NULL;
                    }
                    else if ( node->next == tmp )  /* Only 2 nodes. */
                    {
                         list = tmp;
                         list->next = list;
                         list->prev = list;
                    }
                    else  /* More than 2 nodes. */
                    {
                         if ( node->next == NULL )
                         {
                              error_code = bad_list;
                              return 1;
                         }
                         else
                         {
                              list = node->next;
                              list->prev = tmp;
                              tmp->next = list;
                         }
                    }
               }
               else if ( last_list_node == node )  /* 'node' is last. */
               {
                    tmp = node->prev;
                    tmp->next = list;
                    list->prev = tmp;
                    last_list_node = tmp;
               }
               else  /* 'node' is neither first nor last. */
               {
                    tmp->next = node->next;
                    tmp = node->next;
                    tmp->prev = node->prev;
               }
          }
          else  /* use_circular == 0 */
          {
               if ( node->next != NULL )
               {
                    tmp->next = node->next;
                    tmp = node->next;
                    tmp->prev = node->prev;
               }
               else  /* Last node in the list. */
               {
                    tmp->next = NULL;
                    last_list_node = tmp;
               }
          }    /* if ( use_circular == 1 ) */
     }
     else  /* First node in the list. */
     {
          tmp = node->next;
          if ( tmp != NULL )
          {
               list = tmp;
               tmp->prev = NULL;
          }
          else  /* And the last node in the list. */
          {
               list = NULL;
               last_list_node = NULL;
          }
     }

     /* Move 'node' to the list of available nodes. */

     if ( free_nodes != NULL )
     {
          node->next = free_nodes;
          free_nodes->prev = node;
     }
     else
     {
          node->next = NULL;
          last_free_node = node;
     }
     node->prev = NULL;
     free_nodes = node;
     free_size++;
     list_size--;
     error_code = success;
     return 0;
}

/*

     Releases all the nodes in 'list' and
     moves them to the 'free_nodes' list.

     Returns 0 for sucess or 1 if an error occurred.

*/

int release_all_nodes( void )
{
     node_t *tmp;

     if ( list == NULL )
     {
          error_code = success;
          return 0;  /* Nothing to release. */
     }

#ifdef USE_MAGIC_NUMBER

     /* Make sure each node in the list has a valid magic number. */

     tmp = list;
     while( tmp != last_list_node && tmp != NULL &&
            tmp->magic == NODE_MAGIC_NUMBER )
     {
          tmp = tmp->next;
     }
     if ( tmp == NULL )
     {
          error_code = bad_list;
          return 1;
     }

     /*

          The convenient thing is that this
          will check the last node as well.

     */

     if ( tmp->magic != NODE_MAGIC_NUMBER )
     {
          error_code = bad_magic;
          return 1;
     }

#endif

     tmp = list;
     while( tmp != NULL && tmp != last_list_node )
     {
          list = tmp->next;
          list_size--;
          if ( free_nodes == NULL )
          {
               free_nodes = tmp;
               last_free_node = tmp;
               free_size = 1;
               tmp->next = NULL;
               tmp->prev = NULL;
          }
          else
          {
               free_nodes->prev = tmp;
               tmp->next = free_nodes;
               tmp->prev = NULL;
               free_nodes = tmp;
               free_size++;
          }

          /* Clear the node's data. */

          tmp->pad = 0;
          tmp->num = 0;

          tmp = list;
     }

     /* Release the last node too. */

     if ( tmp != NULL )
     {
          if ( free_nodes == NULL )
          {
               free_nodes = tmp;
               last_free_node = tmp;
               free_size = 1;
               tmp->next = NULL;
               tmp->prev = NULL;
          }
          else
          {
               free_nodes->prev = tmp;
               tmp->next = free_nodes;
               tmp->prev = NULL;
               free_nodes = tmp;
               free_size++;
          }
          list_size--;

          /* Clear the node's data. */

          tmp->pad = 0;
          tmp->num = 0;

          /* And we're done. */

          if ( tmp == last_list_node )
          {
               last_list_node = list = NULL;  /* Clear the pointers. */
               list_size = 0;  /* Set the list size to zero. */
               error_code = success;
               return 0;
          }
          else  /* Unless something went wrong. */
          {
               error_code = bad_list;
               return 1;
          }
     }
     else  /* The list was screwed up. */
     {
          error_code = bad_list;
          return 1;
     }
}

/*

     Releases the memory used by 'node' to the system.

     Returns 0 for success or 1 if an error occurred.

*/

int free_node( node_t *node )
{
     node_t *tmp;

     if ( node == NULL )
     {
          error_code = pointer;
          return 1;
     }

#ifdef USE_MAGIC_NUMBER

     if ( node->magic != NODE_MAGIC_NUMBER )
     {
          error_code = bad_magic;
          return 1;
     }

#endif

     /* Make sure the node is in the list of free nodes. */

     tmp = free_nodes;
     while( tmp != node && tmp != last_free_node )
     {
          tmp = tmp->next;
     }
     if ( tmp == last_free_node && tmp != node )  /* Not found. */
     {
          /* See if it is in 'list'. */

          tmp = list;
          if ( list != NULL )
          {
               while( tmp != node && tmp != last_list_node )
               {
                    tmp = tmp->next;
               }
               if ( tmp == node )  /* Found it in the list. */
               {
                    error_code = in_use;
                    return 1;
               }
          }

          /* This 'node' may have been extracted. */
          /* Clear its data and free it.          */

#ifdef USE_MAGIC_NUMBER

          node->magic = 0;
          node->pad = 0;

#endif

          node->num = 0;
          node->next = NULL;
          node->prev = NULL;
          free( node );
          error_code = success;
          return 0;
     }

     /* Update the links. */

     tmp = node->prev;
     if ( tmp != NULL )
     {
          if ( node->next != NULL )
          {
               tmp->next = node->next;
               tmp = node->next;
               tmp->prev = node->prev;
          }
          else  /* Last node in the list. */
          {
               tmp->next = NULL;
               last_free_node = tmp;
          }
     }
     else  /* First node in the list. */
     {
          tmp = node->next;
          if ( tmp != NULL )
          {
               free_nodes = tmp;
               tmp->prev = NULL;
          }
          else  /* And the last node in the list. */
          {
               free_nodes = NULL;
               last_free_node = NULL;
          }
     }

#ifdef USE_MAGIC_NUMBER

     node->magic = 0;
     node->pad = 0;

#endif

     node->num = 0;
     node->next = NULL;
     node->prev = NULL;
     free( node );
     free_size--;
     error_code = success;
     return 0;
}

/*

     Frees all the nodes in the list of available nodes.

     Returns 0 for success or 1 if an error occurred.

*/

int free_all_nodes( void )
{
     node_t *tmp;

     if ( free_nodes == NULL )
     {
          error_code = success;
          return 0;  /* Nothing to release. */
     }

#ifdef USE_MAGIC_NUMBER

     /* Make sure each node in the free list has a valid magic number. */

     tmp = free_nodes;
     while( tmp != last_free_node && tmp != NULL &&
            tmp->magic == NODE_MAGIC_NUMBER )
     {
          tmp = tmp->next;
     }
     if ( tmp == NULL )
     {
          error_code = bad_free;
          return 1;
     }

     /*

          The convenient thing is that this
          will check the last node as well.

     */

     if ( tmp->magic != NODE_MAGIC_NUMBER )
     {
          error_code = bad_magic;
          return 1;
     }

#endif

     tmp = free_nodes;
     while( tmp != NULL && tmp != last_free_node )
     {
          free_nodes = free_nodes->next;
          free_nodes->prev = NULL;
          free_size--;

          /* Clear all structure members. */

          tmp->magic = 0;
          tmp->pad = 0;
          tmp->num = 0;
          tmp->next = NULL;
          tmp->prev = NULL;

          /* Free the node. */

          free( tmp );

          tmp = free_nodes;
     }

     /* Release the last node too. */

     if ( tmp != NULL )
     {
          /* Clear all structure members. */

          tmp->magic = 0;
          tmp->pad = 0;
          tmp->num = 0;
          tmp->next = NULL;
          tmp->prev = NULL;

          /* Free the node. */

          free( tmp );

          /* Clear the pointers and free_size. */

          free_nodes = last_free_node = tmp = NULL;
          free_size = 0;

          /* And we're done. */

          error_code = success;
          return 0;
     }
     else  /* The free list was screwed up. */
     {
          error_code = bad_free;
          return 1;
     }
}

/*

     Inserts 'new_node' after the 'current' node in the list.

     'current' is allowed to be NULL if the 'list' is NULL in
     which case 'new_node' will be the first node in the 'list'.

     Returns 0 on success and 1 on error.

*/

int insert_node_as_next( node_t *current, node_t *new_node )
{
     node_t *tmp;

     /* Handle a special case first. */

     if ( current == NULL && list == NULL && new_node != NULL )
     {

#ifdef USE_MAGIC_NUMBER

          if ( new_node->magic != NODE_MAGIC_NUMBER )
          {
               error_code = bad_magic;
               return 1;
          }

#endif

          if ( using_circular != 0 && using_circular != 1 )
          {
               error_code = bad_list;
               return 1;
          }

          last_list_node = list = new_node;
          list_size = 1;

          if ( using_circular == 1 )
          {
               list->next = list;
               list->prev = list;
          }
          else
          {
               list->next = NULL;
               list->prev = NULL;
          }

          error_code = success;
          return 0;
     }

     if ( current == NULL || new_node == NULL )
     {
          error_code = pointer;
          return 1;
     }

#ifdef USE_MAGIC_NUMBER

     if ( current->magic != NODE_MAGIC_NUMBER ||
          new_node->magic != NODE_MAGIC_NUMBER )
     {
          error_code = bad_magic;
          return 1;
     }

#endif

     /* 'current' isn't in the list if we don't have a list. */

     if ( list == NULL )
     {
          error_code = bad_arg;
          return 1;
     }

     /* We need to make sure 'current' is actually in the list. */

     tmp = list;

     /* This method accomodates circularly linked lists. */

     while( tmp != current && tmp != last_list_node )
     {
          tmp = tmp->next;
     }

     if ( tmp != current )  /* 'current' is not in the list. */
     {
          error_code = bad_arg;
          return 1;
     }

     tmp = current->next;
     current->next = new_node;
     new_node->next = tmp;
     if ( tmp != NULL )
     {
          tmp->prev = new_node;
          if ( using_circular == 1 )
          {
               if ( tmp == list )
               {
                    /* We only had one node before adding the 'new_node'. */

                    last_list_node = new_node;
               }
          }
     }
     else
     {
          last_list_node = new_node;
     }
     new_node->prev = current;
     list_size++;
     error_code = success;
     return 0;
}

/*

     Inserts 'new_node' before the 'current' node in the list.

     'current' is allowed to be NULL if the 'list' is NULL in
     which case 'new_node' will be the first node in the 'list'.

     Returns 0 on success and 1 on error.

*/

int insert_node_as_previous( node_t *current, node_t *new_node )
{
     node_t *tmp;

     /* Handle a special case first. */

     if ( current == NULL && list == NULL && new_node != NULL )
     {

#ifdef USE_MAGIC_NUMBER

          if ( new_node->magic != NODE_MAGIC_NUMBER )
          {
               error_code = bad_magic;
               return 1;
          }

#endif

          if ( using_circular != 0 && using_circular != 1 )
          {
               error_code = bad_list;
               return 1;
          }

          last_list_node = list = new_node;
          list_size = 1;

          if ( using_circular == 1 )
          {
               list->next = list;
               list->prev = list;
          }
          else
          {
               list->next = NULL;
               list->prev = NULL;
          }

          error_code = success;
          return 0;
     }

     if ( current == NULL || new_node == NULL || list == NULL )
     {
          error_code = pointer;
          return 1;
     }

#ifdef USE_MAGIC_NUMBER

     if ( current->magic != NODE_MAGIC_NUMBER ||
          new_node->magic != NODE_MAGIC_NUMBER )
     {
          error_code = bad_magic;
          return 1;
     }

#endif

     /* 'current' isn't in the list if we don't have a list. */

     if ( list == NULL )
     {
          error_code = bad_arg;
          return 1;
     }

     /* We need to make sure 'current' is actually in the list. */

     tmp = list;

     /* This method accomodates circularly linked lists. */

     while( tmp != current && tmp != last_list_node )
     {
          tmp = tmp->next;
     }

     if ( tmp != current )  /* 'current' is not in the list. */
     {
          error_code = bad_arg;
          return 1;
     }

     if ( current == list )
     {
          list = new_node;
          new_node->next = current;
          if ( using_circular == 1 )
          {
               new_node->prev = last_list_node;
               last_list_node->next = new_node;
          }
          else  /* using_circular == 0 */
          {
               new_node->prev = NULL;
          }
          current->prev = new_node;
     }
     else
     {
          tmp = current->prev;
          if ( tmp == NULL )
          {
               error_code = bad_list;
               return 1;
          }
          tmp->next = new_node;
          new_node->prev = tmp;
          new_node->next = current;
          current->prev = new_node;
     }
     list_size++;
     error_code = success;
     return 0;
}

/*

     Swaps the positions of 'node1' and 'node2' in the list.
     This allows for a programmer to write an algorithm to sort the list.

     Returns 0 for success or 1 if an error occurred.

*/

int swap_nodes( node_t *node1, node_t *node2 )
{
     int found1, found2;
     node_t *ptr1, *ptr2, *ptr3, *ptr4;

     /* We can't use NULL pointers. */

     if ( node1 == NULL || node2 == NULL )
     {
          error_code = pointer;
          return 1;
     }

#ifdef USE_MAGIC_NUMBER

     if ( node1->magic != NODE_MAGIC_NUMBER ||
          node2->magic != NODE_MAGIC_NUMBER )
     {
          error_code = bad_magic;
          return 1;
     }

#endif

     /* We can't swap them if both of them are the same node. */

     if ( node1 == node2 )
     {
          error_code = bad_arg;
          return 1;
     }

     /* The nodes won't be in the list if we don't have a list. */

     if ( list == NULL )
     {
          error_code = bad_arg;
          return 1;
     }

     /* They can't both be in the list if the list only has one node. */

     if ( list_size < 2 )
     {
          error_code = bad_arg;
          return 1;
     }

     /* We need to make sure both nodes are actually in the list. */
     /* Each node should appear once and only once.               */

     found1 = found2 = 0;
     ptr1 = list;

     /* Using this method will accomodate circularly linked lists. */

     while( ptr1 != last_list_node )
     {
          if ( ptr1 == node1 )
          {
               if ( found1 == 0 )
               {
                    found1 = 1;
               }
               else  /* 'node1' has been found twice. */
               {
                    error_code = bad_list;
                    return 1;
               }
          }
          if ( ptr1 == node2 )
          {
               if ( found2 == 0 )
               {
                    found2 = 1;
               }
               else  /* 'node2' has been found twice. */
               {
                    error_code = bad_list;
                    return 1;
               }
          }
          ptr1 = ptr1->next;
     }

     /*
          At this point ptr1 should be pointing
          to the last node in the list.

     */

     if ( ptr1 == node1 )
     {
          if ( found1 == 0 )
          {
               found1 = 1;
          }
          else  /* 'node1' has been found twice. */
          {
               error_code = bad_list;
               return 1;
          }
     }
     if ( ptr1 == node2 )
     {
          if ( found2 == 0 )
          {
               found2 = 1;
          }
          else  /* 'node2' has been found twice. */
          {
               error_code = bad_list;
               return 1;
          }
     }

     if ( ( found1 == 0 ) || ( found2 == 0 ) )
     {
          /* We couldn't find both nodes in the list. */
          error_code = bad_arg;
          return 1;
     }

     ptr1 = node1->prev;
     ptr2 = node1->next;
     ptr3 = node2->prev;
     ptr4 = node2->next;

     if ( using_circular == 0 )
     {
          /* Both nodes can't be first on the list. */

          if ( ptr1 == NULL && ptr3 == NULL )
          {
               error_code = bad_list;
               return 1;
          }

          /* Both nodes can't be last on the list. */

          if ( ptr2 == NULL && ptr4 == NULL )
          {
               error_code = bad_list;
               return 1;
          }
     }

     /* Update the 'last_list_node' pointer. */

     if ( last_list_node == node1 )
     {
          last_list_node = node2;
     }
     else if ( last_list_node == node2 )
     {
          last_list_node = node1;
     }

     if ( using_circular == 1 )
     {
          /* Update the 'list' pointer if necessary. */

          if ( list == node1 )
          {
               list = node2;
          }
          else if ( list == node2 )
          {
               list = node1;
          }

          /*

               At this point we really don't need to change anything if
               we only have two nodes in a circular list since all of
               the necessary changes have been made.  The list simply
               rotates.

          */

          if ( list_size > 2 )
          {
               if ( node1->next == node2 )
               {
                    ptr1->next = node2;
                    node2->prev = ptr1;
                    node2->next = node1;
                    node1->prev = node2;
                    node1->next = ptr4;
                    ptr4->prev = node1;
               }
               else if ( node2->next == node1 )
               {
                    ptr3->next = node1;
                    node1->prev = ptr3;
                    node1->next = node2;
                    node2->prev = node1;
                    node2->next = ptr2;
                    ptr2->prev = node2;
               }
               else
               {
                    ptr1->next = node2;
                    node2->prev = ptr1;
                    node2->next = ptr2;
                    ptr2->prev = node2;
                    ptr3->next = node1;
                    node1->prev = ptr3;
                    node1->next = ptr4;
                    ptr4->prev = node1;
               }    /* if ( node1->next == node2 ) */
          }    /* if ( list_size == 2 ) */
     }
     else  /* using_circular == 0 */
     {
          if ( list_size == 2 )
          {
               if ( list == node1 )
               {
                    list = node2;
                    node2->prev = NULL;
                    node2->next = node1;
                    node1->prev = node2;
                    node1->next = NULL;
               }
               else  /* list == node2 */
               {
                    list = node1;
                    node1->prev = NULL;
                    node1->next = node2;
                    node2->prev = node1;
                    node2->next = NULL;
               }
          }
          else  /* list_size > 2 */
          {
               if ( ptr1 == NULL )  /* 'node1' is first. */
               {
                    if ( node1->next == node2 )
                    {
                         list = node2;
                         node2->prev = NULL;
                         node2->next = node1;
                         node1->prev = node2;
                         node1->next = ptr4;
                    }
                    else
                    {
                         list = node2;
                         node2->prev = NULL;
                         node2->next = ptr2;
                         ptr2->prev = node2;
                         ptr3->next = node1;
                         node1->prev = ptr3;
                         node1->next = ptr4;

                         /* 'node2' may have been last. */

                         if ( ptr4 != NULL )
                         {
                              ptr4->prev = node1;
                         }
                    }
               }
               else  /* ptr1 != NULL */
               {
                    if ( ptr3 == NULL )  /* 'node2' is first. */
                    {
                         if ( node2->next == node1 )
                         {
                              list = node1;
                              node1->prev = NULL;
                              node1->next = node2;
                              node2->prev = node1;
                              node2->next = ptr2;
                              ptr2->prev = node1;
                         }
                         else
                         {
                              list = node1;
                              node1->prev = NULL;
                              node1->next = ptr4;
                              ptr4->prev = node1;
                              ptr1->next = node2;
                              node2->prev = ptr1;
                              node2->next = ptr2;

                              /* 'node1' may have been last. */

                              if ( ptr2 != NULL )
                              {
                                   ptr2->prev = node2;
                              }
                         }
                    }
                    else  /* Neither node is first. */
                    {
                         if ( node1->next == node2 )
                         {
                              ptr1->next = node2;
                              node2->prev = ptr1;
                              node2->next = node1;
                              node1->next = ptr4;
                              node1->prev = node2;
                         }
                         else if ( node2->next == node1 )
                         {
                              ptr3->next = node1;
                              node1->prev = ptr3;
                              node1->next = node2;
                              node2->prev = node1;
                              node2->next = ptr2;
                         }
                         else
                         {
                              ptr1->next = node2;
                              node2->prev = ptr1;
                              node2->next = ptr2;
                              ptr2->prev = node2;
                              ptr3->next = node1;
                              node1->prev = ptr3;
                              node1->next = ptr4;

                              /* 'node2' may have been last. */

                              if ( ptr4 != NULL )
                              {
                                   ptr4->prev = node1;
                              }
                         }    /* if ( node1->next == node2 ) */
                    }    /* if ( ptr3 == NULL ) */
               }    /* if ( ptr1 == NULL ) */
          }    /* if ( list_size == 2 ) */
     }    /* if ( using_circular == 1 ) */
     error_code = success;
     return 0;
}

/* Lists the nodes used in 'list'. */

void list_nodes( void )
{
     char circular_str[] = "circularly linked ";
     char linear_str[] = "linear linked ";
     char empty_str[] = "\0";
     char str[ LENGTH ], *str_ptr;
     int count, ret, space;
     node_t *ptr;

     if ( using_circular == 1 )
     {
          str_ptr = circular_str;
     }
     else if ( using_circular == 0 )
     {
          str_ptr = linear_str;
     }
     else
     {
          snprintf( str, LENGTH, "\
The variable using_circular has an invalid value: %d.\n\n",
                    using_circular );
          write_output( str );
          str_ptr = empty_str;
     }
     if ( list == NULL )
     {
          error_code = success;
          snprintf( str, LENGTH, "The %slist is empty.\n\n",
                    str_ptr );
          write_output( str );
          if ( list_size != 0 )
          {
               snprintf( str, LENGTH, "\
The list size should be 0, but instead it seems to be %d.\n\n",
                         list_size );
               write_output( str );
               error_code = bad_list;
          }
          return;
     }
     if ( list_size == 1 )
     {
          snprintf( str, LENGTH, "The %slist is using 1 node:\n\n",
                    str_ptr );
     }
     else
     {
          snprintf( str, LENGTH, "The %slist is using %d nodes:\n\n",
                    str_ptr, list_size );
     }
     write_output( str );

#ifdef USE_MAGIC_NUMBER

     count = 1;
     ptr = list;
     while( ptr != NULL && ptr != last_list_node )
     {
          if ( ptr->magic != NODE_MAGIC_NUMBER )
          {
               snprintf( str, LENGTH, "\
Node %d has a bad magic number: %04X.  Exiting list_nodes().\n\n",
                         count, ptr->magic );
               write_output( str );
               error_code = bad_magic;
               return;
          }
          count++;
          ptr = ptr->next;
     }
     if ( ptr != NULL )  /* Check the last node too. */
     {
          if ( ptr->magic != NODE_MAGIC_NUMBER )
          {
               snprintf( str, LENGTH, "\
Node %d has a bad magic number: %04X.  Exiting list_nodes().\n\n",
                         count, ptr->magic );
               write_output( str );
               error_code = bad_magic;
               return;
          }
     }
     else
     {
          error_code = bad_list;
          return;
     }

#endif

     ret = find_highest_node_number();
     if ( ret < 1 )
     {
          return;  /* The 'error_code' is already set. */
     }

     ptr = list;
     count = 1;

     /* How much space in bytes will the maximum number of digits need? */

     space = ( int )trunc( log10( ( double )ret ) ) + 1;

     do
     {

#ifdef _32BITS

          snprintf( str, LENGTH, "\
&node %*d: %08X Node number: %*d. Next: %08X Prev: %08X\n",
                    space, count, ( unsigned int )ptr, space, ptr->num,
                    ( unsigned int )ptr->next,
                    ( unsigned int )ptr->prev );

#else  /* _64BITS */

          snprintf( str, LENGTH, "\
&node %*d: %016X Node number: %*d. Next: %016X Prev: %016X\n",
                    space, count, ( unsigned int )ptr, space, ptr->num,
                    ( unsigned int )ptr->next,
                    ( unsigned int )ptr->prev );

#endif

          write_output( str );
          ptr = ptr->next;
          count++;
     }    while( count <= list_size );
     write_output( "\n" );

#ifdef _32BITS

     snprintf( str, LENGTH, "Pointer last_list_node: %08X\n\n",
               ( unsigned int )last_list_node );

#else  /* _64BITS */

     snprintf( str, LENGTH, "Pointer last_list_node: %016X\n\n",
               ( unsigned int )last_list_node );

#endif

     write_output( str );
     error_code = success;
     return;
}

/* Lists the unused nodes in 'free_nodes'. */

void list_free_nodes( void )
{
     char str[ LENGTH ];
     int count, space;
     node_t *ptr;

     if ( free_nodes == NULL )
     {
          error_code = success;
          write_output( "The list of available nodes is empty.\n\n" );
          if ( free_size != 0 )
          {
               snprintf( str, LENGTH, "\
The free node list size should be 0, but instead it seems to be %d.\n\n",
                         free_size );
               write_output( str );
               error_code = bad_free;
          }
          return;
     }
     if ( free_size == 1 )
     {
          snprintf( str, LENGTH, "\
There is 1 available node that the list is not using:\n\n" );
     }
     else
     {
          snprintf( str, LENGTH, "\
There are %d available nodes that the list is not using:\n\n",
                    free_size );
     }
     write_output( str );

#ifdef USE_MAGIC_NUMBER

     count = 1;
     ptr = free_nodes;
     while( ptr != NULL && ptr != last_free_node )
     {
          if ( ptr->magic != NODE_MAGIC_NUMBER )
          {
               snprintf( str, LENGTH, "\
Node %d has a bad magic number: %04X.  Exiting list_free_nodes().\n\n",
                         count, ptr->magic );
               write_output( str );
               error_code = bad_magic;
               return;
          }
          count++;
          ptr = ptr->next;
     }
     if ( ptr != NULL )  /* Check the last node too. */
     {
          if ( ptr->magic != NODE_MAGIC_NUMBER )
          {
               snprintf( str, LENGTH, "\
Node %d has a bad magic number: %04X.  Exiting list_free_nodes().\n\n",
                         count, ptr->magic );
               write_output( str );
               error_code = bad_magic;
               return;
          }
     }
     else
     {
          error_code = bad_free;
          return;
     }

#endif

     ptr = free_nodes;
     count = 1;

     /* How much space in bytes will the maximum number of digits need? */

     space = ( int )trunc( log10( ( double )free_size ) ) + 1;

     do
     {

#ifdef _32BITS

          snprintf( str, LENGTH,
                    "&node %*d: %08X. Next: %08X Prev: %08X\n",
                    space, count, ( unsigned int )ptr,
                    ( unsigned int )ptr->next,
                    ( unsigned int )ptr->prev );

#else  /* _64BITS */

          snprintf( str, LENGTH,
                    "&node %*d: %016X. Next: %016X Prev: %016X\n",
                    space, count, ( unsigned int )ptr,
                    ( unsigned int )ptr->next,
                    ( unsigned int )ptr->prev );

#endif

          write_output( str );
          ptr = ptr->next;
          count++;
     }    while( count <= free_size );
     write_output( "\n" );

#ifdef _32BITS

     snprintf( str, LENGTH, "Pointer last_free_node: %08X\n\n",
               ( unsigned int )last_free_node );

#else  /* _64BITS */

     snprintf( str, LENGTH, "Pointer last_free_node: %016X\n\n",
               ( unsigned int )last_free_node );

#endif

     write_output( str );
     error_code = success;
     return;
}

/* Displays a node's data. */

void display_node_data( node_t *ptr )
{
     char str[ LENGTH ];

     if ( ptr == NULL )
     {
          error_code = pointer;
          return;
     }

#ifdef _32BITS

     snprintf( str, LENGTH, "Node address: %08X\n",
               ( unsigned int )ptr );

#else  /* _64BITS */

     snprintf( str, LENGTH, "Node address: %016X\n",
               ( unsigned int )ptr );

#endif

     write_output( str );
     write_output( "Node data:\n\n" );

#ifdef USE_MAGIC_NUMBER

     if ( ptr->magic != NODE_MAGIC_NUMBER )
     {
          snprintf( str, LENGTH,
                    "node->magic: %04Xh (Bad magic number)\n",
                    ( int )ptr->magic );
     }
     else
     {
          snprintf( str, LENGTH,
                    "node->magic: %04Xh\n", ( int )ptr->magic );
     }
     write_output( str );
     snprintf( str, LENGTH, "node->pad:   %d\n", ( int )ptr->pad );
     write_output( str );

#endif

     snprintf( str, LENGTH, "node->num:   %d\n", ptr->num );
     write_output( str );

#ifdef _32BITS

     snprintf( str, LENGTH, "node->next:  %08X\n",
               ( unsigned int )ptr->next );

#else  /* _64BITS */

     snprintf( str, LENGTH, "node->next:  %016X\n",
               ( unsigned int )ptr->next );

#endif

     write_output( str );

#ifdef _32BITS

     snprintf( str, LENGTH, "node->prev:  %08X\n\n",
               ( unsigned int )ptr->prev );

#else  /* _64BITS */

     snprintf( str, LENGTH, "node->prev:  %016X\n\n",
               ( unsigned int )ptr->prev );

#endif

     write_output( str );
     return;
}

/*

     Finds the nth node in the list and returns
     a pointer to it or NULL if an error occurred.

*/

node_t *find_nth_node( const int number )
{
     int count;
     node_t *tmp;

     if ( number < 1 || number > list_size )
     {
          error_code = bad_arg;
          return NULL;
     }
     tmp = list;

     /* This shouldn't happen. */

     if ( tmp == NULL )
     {
          list_size = 0;
          error_code = bad_list;
          return NULL;
     }

     for( count = 1; count < number && tmp != last_list_node; count++ )
     {
          tmp = tmp->next;
     }
     if ( count == number )
     {

#ifdef USE_MAGIC_NUMBER

          if ( tmp->magic != NODE_MAGIC_NUMBER )
          {
               error_code = bad_magic;
               return NULL;
          }
          else
          {
               error_code = success;
               return tmp;
          }

#else

          error_code = success;
          return tmp;

#endif

     }
     else
     {
          error_code = bad_list;
          return NULL;
     }
}

/*

     This function is provided as an example.

     Since 'num' is the only data member
     in the node we'll search for that.

     Remember to use strcmp() when comparing strings.

     Returns a pointer to the correct node
     if found, otherwise returns NULL.

*/

node_t *find_node_by_number( const int num )
{
     node_t *tmp;

     if ( num < 1 )
     {
          error_code = bad_arg;
          return NULL;
     }

     /* This shouldn't happen if 'list_size' is correct. */

     if ( list == NULL )
     {
          error_code = bad_list;
          return NULL;
     }

     tmp = list;
     while( tmp->num != num && tmp != last_list_node )
     {
          tmp = tmp->next;
     }

     if ( tmp->num == num )
     {

#ifdef USE_MAGIC_NUMBER

          if ( tmp->magic != NODE_MAGIC_NUMBER )
          {
               error_code = bad_magic;
               return NULL;
          }
          else
          {
               error_code = success;
               return tmp;
          }

#else

          error_code = success;
          return tmp;

#endif

     }
     else  /* Not found. */
     {
          error_code = bad_arg;
          return NULL;
     }
}

/*

     This function finds the lowest node number.

     Returns the lowest node number found.
     If the 'list' is empty, it will return 0.
     If an error occurs, it will return -1.

*/

int find_lowest_node_number( void )
{
     int lowest;
     node_t *ptr;

     if ( list == NULL )
     {
          error_code = success;
          return 0;  /* No list available. */
     }

#ifdef USE_MAGIC_NUMBER

     ptr = list;
     while( ptr != NULL && ptr != last_list_node )
     {
          if ( ptr->magic != NODE_MAGIC_NUMBER )
          {
               error_code = bad_magic;
               return ( -1 );
          }
          ptr = ptr->next;
     }
     if ( ptr != NULL )  /* Check the last node too. */
     {
          if ( ptr->magic != NODE_MAGIC_NUMBER )
          {
               error_code = bad_magic;
               return ( -1 );
          }
     }
     else
     {
          error_code = bad_list;
          return ( -1 );
     }

#endif

     lowest = 0;
     ptr = list;
     while( ptr != NULL && ptr != last_list_node )
     {
          if ( lowest == 0 )
          {
               lowest = ptr->num;
          }
          else
          {
               if ( ptr->num < lowest )
               {
                    lowest = ptr->num;
               }
          }
          ptr = ptr->next;
     }
     if ( ptr != NULL )  /* Check the last node too. */
     {
          if ( lowest == 0 )
          {
               lowest = ptr->num;
          }
          else
          {
               if ( ptr->num < lowest )
               {
                    lowest = ptr->num;
               }
          }
     }
     else
     {
          error_code = bad_list;
          return ( -1 );
     }

     if ( lowest < 1 )
     {
          error_code = bad_list;
          return ( -1 );
     }

     error_code = success;
     return lowest;
}

/*

     This function finds the highest node number.

     Returns the highest node number found.
     If the 'list' is empty, it will return 0.
     If an error occurs, it will return -1.

*/

int find_highest_node_number( void )
{
     int highest;
     node_t *ptr;

     if ( list == NULL )
     {
          error_code = success;
          return 0;  /* No list available. */
     }

#ifdef USE_MAGIC_NUMBER

     ptr = list;
     while( ptr != NULL && ptr != last_list_node )
     {
          if ( ptr->magic != NODE_MAGIC_NUMBER )
          {
               error_code = bad_magic;
               return ( -1 );
          }
          ptr = ptr->next;
     }
     if ( ptr != NULL )  /* Check the last node too. */
     {
          if ( ptr->magic != NODE_MAGIC_NUMBER )
          {
               error_code = bad_magic;
               return ( -1 );
          }
     }
     else
     {
          error_code = bad_list;
          return ( -1 );
     }

#endif

     highest = 0;
     ptr = list;
     while( ptr != NULL && ptr != last_list_node )
     {
          if ( ptr->num > highest )
          {
               highest = ptr->num;
          }
          ptr = ptr->next;
     }
     if ( ptr != NULL )  /* Check the last node too. */
     {
          if ( ptr->num > highest )
          {
               highest = ptr->num;
          }
     }
     else
     {
          error_code = bad_list;
          return ( -1 );
     }

     if ( highest < 1 )
     {
          error_code = bad_list;
          return ( -1 );
     }

     error_code = success;
     return highest;
}

/*

     Counts the number of nodes in the list.
     Returns the count if successful or -1 if an error occurs.

*/

int count_list_nodes( void )
{
     int count;
     node_t *tmp;

     if ( list == NULL )
     {
          return 0;
     }
     tmp = list;
     count = 1;

#ifdef USE_MAGIC_NUMBER

     while( tmp != last_list_node && tmp->magic == NODE_MAGIC_NUMBER )

#else

     while( tmp != last_list_node )

#endif

     {
          count++;
          tmp = tmp->next;
     }

#ifdef USE_MAGIC_NUMBER

     if ( tmp->magic != NODE_MAGIC_NUMBER )
     {
          error_code = bad_magic;
          return -1;
     }

#endif

     return count;
}

/*

     Counts the number of nodes in the list of available nodes.
     Returns the count if successful or -1 if an error occurs.

*/

int count_free_nodes( void )
{
     int count;
     node_t *tmp;

     if ( free_nodes == NULL )
     {
          return 0;
     }
     tmp = free_nodes;
     count = 1;

#ifdef USE_MAGIC_NUMBER

     while( tmp != last_free_node && tmp->magic == NODE_MAGIC_NUMBER )

#else

     while( tmp != last_free_node )

#endif

     {
          count++;
          tmp = tmp->next;
     }

#ifdef USE_MAGIC_NUMBER

     if ( tmp->magic != NODE_MAGIC_NUMBER )
     {
          error_code = bad_magic;
          return -1;
     }

#endif

     return count;
}

/* Converts a linearly linked list to a circularly linked list.    */

/* If the list only has one node then it will be linked to itself. */

int convert_list_to_circular( void )
{
     if ( using_circular == 1 )
     {
          error_code = bad_arg;  /* It is already circular. */
          return 1;
     }
     if ( using_circular != 0 )
     {
          error_code = bad_arg;  /* Invalid value. */
          return 1;
     }
     if ( list == NULL )
     {
          /* No list to convert. */

          if ( last_list_node == NULL && list_size == 0 )
          {
               using_circular = 1;
               error_code = success;
               return 0;
          }
          else  /* Something is screwed up. */
          {
               error_code = bad_list;
               return 1;
          }
     }

     /*

          If we have a list and this pointer
          is NULL then the list is screwed up.

     */

     if ( last_list_node == NULL )
     {
          error_code = bad_list;
          return 1;
     }

#ifdef USE_MAGIC_NUMBER

     if ( list->magic != NODE_MAGIC_NUMBER ||
          last_list_node->magic != NODE_MAGIC_NUMBER )
     {
          error_code = bad_magic;
          return 1;
     }

#endif

     /* Link the ends of the list together. */

     list->prev = last_list_node;
     last_list_node->next = list;
     using_circular = 1;
     error_code = success;
     return 0;
}

/* Converts a circularly linked list to a linear linked list.      */

/* If the list only has one node then it will be linked to itself. */

int convert_list_to_linear( void )
{
     if ( using_circular == 0 )
     {
          error_code = bad_arg;  /* It is already linear. */
          return 1;
     }
     if ( using_circular != 1 )
     {
          error_code = bad_arg;  /* Invalid value. */
          return 1;
     }
     if ( list == NULL )
     {
          /* No list to convert. */

          if ( last_list_node == NULL && list_size == 0 )
          {
               using_circular = 0;
               error_code = success;
               return 0;
          }
          else  /* Something is screwed up. */
          {
               error_code = bad_list;
               return 1;
          }
     }

     /*

          If we have a list and this pointer
          is NULL then the list is screwed up.

     */

     if ( last_list_node == NULL )
     {
          error_code = bad_list;
          return 1;
     }

#ifdef USE_MAGIC_NUMBER

     if ( list->magic != NODE_MAGIC_NUMBER ||
          last_list_node->magic != NODE_MAGIC_NUMBER )
     {
          error_code = bad_magic;
          return 1;
     }

#endif

     /* Unlink the ends of the list. */

     list->prev = NULL;
     last_list_node->next = NULL;
     using_circular = 0;
     error_code = success;
     return 0;
}

/* Returns the string associated with the current error code. */

const char *error_string( void )
{
     static char str[ 25 ];

     /* The list starts at zero. */

     if ( error_code < 0 || error_code >= num_error_codes )
     {
          strcpy( str, "Invalid error code" );
     }
     else
     {
           switch( error_code )
           {
                case success   : strcpy( str, "Success" );
                                 break;
                case no_malloc : strcpy( str, "Function malloc() failed" );
                                 break;
                case pointer   : strcpy( str, "Null pointer received" );
                                 break;

#ifdef USE_MAGIC_NUMBER

                case bad_magic : strcpy( str, "Bad magic number" );
                                 break;

#endif

                case in_use    : strcpy( str, "Node is in use" );
                                 break;
                case bad_arg   : strcpy( str, "Invalid argument" );
                                 break;
                case bad_list  : strcpy( str, "List is corrupt" );
                                 break;
                case bad_free  : strcpy( str, "Free list is corrupt" );
                                 break;
                default        : strcpy( str, "Invalid error code" );
                                 break;
           }
     }
     return str;
}

/*

     This function checks the integrity of the 'list'.
     Each problem found is reported to the user.

     Returns the number of problems found.

*/

int check_list_integrity( void )
{
     char str[ LENGTH ];
     int count, nodes, tmp;
     node_t *last, *node;

     /* Keep track of how many errors are found. */

     count = 0;

     /* Not much to analyze if we don't have a list. */

     if ( list == NULL )
     {
          if ( list_size != 0 )
          {
               count++;
               snprintf( str, LENGTH, "\
(%d) The list pointer is NULL, but the list_size is %d.\n",
                         count, list_size );
               write_output( str );
          }
          if ( last_list_node != NULL )
          {
               count++;
               snprintf( str, LENGTH, "\
(%d) The list pointer is NULL, but the last_list_node isn't.\n", count );
               write_output( str );
          }
          if ( count != 0 )
          {
               error_code = bad_list;
          }
          else
          {
               error_code = success;
          }
          return count;
     }

     if ( last_list_node == NULL )
     {
          count++;
          snprintf( str, LENGTH, "\
(%d) The list pointer is not NULL, but the last_list_node is.\n", count );
          write_output( str );
     }

     if ( using_circular != 0 && using_circular != 1 )
     {
          count++;
          snprintf( str, LENGTH, "\
(%d) The variable using_circular has an invalid value: %d.\n",
                    count, using_circular );
          write_output( str );
     }

#ifdef USE_MAGIC_NUMBER

     if ( list->magic != NODE_MAGIC_NUMBER )
     {
          count++;
          snprintf( str, LENGTH, "\
(%d) The 1st node in the list has a bad magic number: %04X.\n",
                    count, list->magic );
          write_output( str );
     }

#endif

     if ( using_circular == 0 )
     {
          if ( list->prev != NULL )
          {
               count++;
               snprintf( str, LENGTH, "\
(%d) list->prev is not NULL when it should be since the list is linear.\n",
                         count );
               write_output( str );
          }

#ifdef USE_MAGIC_NUMBER

          if ( last_list_node != NULL &&
               last_list_node->magic != NODE_MAGIC_NUMBER )
          {
               count++;
               snprintf( str, LENGTH, "\
(%d) The node last_list_node has a bad magic number.\n", count );
               write_output( str );
          }

#endif

          if ( last_list_node != NULL )
          {
               if ( last_list_node->next != NULL )
               {
                    count++;
                    snprintf( str, LENGTH, "\
(%d) last_list_node->next is not NULL\n\
when it should be since the list is linear.\n",
                              count );
                    write_output( str );
               }
          }
          if ( list->next == list )
          {
               count++;
               snprintf( str, LENGTH, "\
(%d) list->next is pointing to list when\n\
it should not be since the list is linear.\n",
                         count );
               write_output( str );
          }
     }
     else if ( using_circular == 1 )
     {
          if ( last_list_node != NULL )
          {
               if ( list->prev != last_list_node )
               {
                    count++;
                    snprintf( str, LENGTH, "\
(%d) list->prev is not pointing to last_list_node\n\
when it should be since the list is circular.\n",
                              count );
                    write_output( str );
               }
               if ( last_list_node->next != list )
               {
                    count++;
                    snprintf( str, LENGTH, "\
(%d) last_list_node->next is not pointing to\n\
list when it should be since the list is circular.\n",
                              count );
                    write_output( str );
               }
          }
     }

     node = list->next;
     last = list;
     nodes = 1;

     /*

          If the list is linear we are done when we move off the end of
          the list.  If the list is circular we are done when we loop
          back around to the beginning of the list.

     */

     while( node != NULL && node != list )
     {
          nodes++;

          /* Isolate the last digit. */

          tmp = nodes / 10;
          tmp = nodes - ( tmp * 10 );

#ifdef USE_MAGIC_NUMBER

          if ( node->magic != NODE_MAGIC_NUMBER )
          {
               count++;

               if ( tmp == 1 )
               {
                    snprintf( str, LENGTH, "\
(%d) The %dst node in the list has a bad magic number: %04X.\n",
                              count, nodes, node->magic );
               }
               else if ( tmp == 2 )
               {
                    snprintf( str, LENGTH, "\
(%d) The %dnd node in the list has a bad magic number: %04X.\n",
                              count, nodes, node->magic );
               }
               else if ( tmp == 3 )
               {
                    snprintf( str, LENGTH, "\
(%d) The %drd node in the list has a bad magic number: %04X.\n",
                              count, nodes, node->magic );
               }
               else
               {
                    snprintf( str, LENGTH, "\
(%d) The %dth node in the list has a bad magic number: %04X.\n",
                              count, nodes, node->magic );
               }
               write_output( str );
          }

#endif

          if ( node->pad != 0 )
          {
               count++;
               if ( tmp == 1 )
               {
                    snprintf( str, LENGTH, "\
(%d) The pad value in the %dst node is %d when it should be zero.\n",
                              count, nodes, node->pad );
               }
               else if ( tmp == 2 )
               {
                    snprintf( str, LENGTH, "\
(%d) The pad value in the %dnd node is %d when it should be zero.\n",
                              count, nodes, node->pad );
               }
               else if ( tmp == 3 )
               {
                    snprintf( str, LENGTH, "\
(%d) The pad value in the %drd node is %d when it should be zero.\n",
                              count, nodes, node->pad );
               }
               else
               {
                    snprintf( str, LENGTH, "\
(%d) The pad value in the %dth node is %d when it should be zero.\n",
                              count, nodes, node->pad );
               }
               write_output( str );
          }

          /*

               node->num must be zero if the node is in the list of
               available nodes.  Otherwise it must be greater than zero.

          */

          if ( node->num < 1 )
          {
               count++;
               if ( tmp == 1 )
               {
                    snprintf( str, LENGTH, "\
(%d) The node member num in the %dst node has an invalid value: %d.\n",
                              count, nodes, node->num );
               }
               else if ( tmp == 2 )
               {
                    snprintf( str, LENGTH, "\
(%d) The node member num in the %dnd node has an invalid value: %d.\n",
                              count, nodes, node->num );
               }
               else if ( tmp == 3 )
               {
                    snprintf( str, LENGTH, "\
(%d) The node member num in the %drd node has an invalid value: %d.\n",
                              count, nodes, node->num );
               }
               else
               {
                    snprintf( str, LENGTH, "\
(%d) The node member num in the %dth node has an invalid value: %d.\n",
                              count, nodes, node->num );
               }
               write_output( str );
          }

          /*

               Obviously last->next points to node
               or we wouldn't be able to access it.

          */

          if ( node->prev != last )
          {
               count++;
               if ( tmp == 1 )
               {
                    snprintf( str, LENGTH, "\
(%d) The %dst node has an invalid prev pointer value.\n",
                              count, nodes );
               }
               else if ( tmp == 2 )
               {
                    snprintf( str, LENGTH, "\
(%d) The %dnd node has an invalid prev pointer value.\n",
                              count, nodes );
               }
               else if ( tmp == 3 )
               {
                    snprintf( str, LENGTH, "\
(%d) The %drd node has an invalid prev pointer value.\n",
                              count, nodes );
               }
               else
               {
                    snprintf( str, LENGTH, "\
(%d) The %dth node has an invalid prev pointer value.\n",
                              count, nodes );
               }
               write_output( str );
          }

          last = node;
          node = node->next;
     }

     if ( last != last_list_node )
     {
          count++;
          snprintf( str, LENGTH, "\
(%d) The last_list_node pointer does\n\
not point to the last node in the list.\n",
                    count );
          write_output( str );

          /* We couldn't check these things earlier. */

          if ( last_list_node == NULL )
          {
               if ( using_circular == 0 )
               {
                    if ( last->next != NULL )
                    {
                         count++;
                         snprintf( str, LENGTH, "\
(%d) The last node->next pointer should be\n\
NULL since the list is linear, but it isn't.\n",
                                   count );
                         write_output( str );
                    }
               }
               else if ( using_circular == 1 )
               {
                    if ( last->next != list )
                    {
                         count++;
                         snprintf( str, LENGTH, "\
(%d) The last node->next pointer should point to the first\n\
node in the list since the list is circular, but it doesn't.\n",
                                   count );
                         write_output( str );
                    }
                    if ( list->prev != last )
                    {
                         count++;
                         snprintf( str, LENGTH, "\
(%d) The list->prev pointer should point to the last node\n\
in the list since the list is circular, but it doesn't.\n",
                                   count );
                         write_output( str );
                    }
               }    /* if ( using_circular == 0 ) */
          }    /* if ( last_list_node == NULL ) */
     }    /* if ( last != last_list_node ) */

     if ( nodes != list_size )
     {
          count++;
          snprintf( str, LENGTH, "\
(%d) The variable list_size: %d does not\n\
equal the number of nodes in the list: %d.\n", count, list_size, nodes );
          write_output( str );
     }

     if ( count != 0 )
     {
          error_code = bad_list;
     }
     else
     {
          error_code = success;
     }
     return count;
}

/*

     This function checks the integrity of the 'free_nodes' list.
     Each problem found is reported to the user.

     Returns the number of problems found.

*/

int check_free_list_integrity( void )
{
     char str[ LENGTH ];
     int count, nodes, tmp;
     node_t *last, *node;

     /* Keep track of how many errors are found. */

     count = 0;

     /* Not much to analyze if we don't have any available nodes. */

     if ( free_nodes == NULL )
     {
          if ( free_size != 0 )
          {
               count++;
               snprintf( str, LENGTH, "\
(%d) The free_nodes pointer is NULL, but the free_size is %d.\n",
                         count, free_size );
               write_output( str );
          }
          if ( last_free_node != NULL )
          {
               count++;
               snprintf( str, LENGTH, "\
(%d) The free_nodes pointer is NULL, but the last_free_node isn't.\n",
                         count );
               write_output( str );
          }
          if ( count != 0 )
          {
               error_code = bad_free;
          }
          else
          {
               error_code = success;
          }
          return count;
     }

     if ( last_free_node == NULL )
     {
          count++;
          snprintf( str, LENGTH, "\
(%d) The free_nodes pointer is not NULL, but the last_free_node is.\n",
                    count );
          write_output( str );
     }

#ifdef USE_MAGIC_NUMBER

     if ( free_nodes->magic != NODE_MAGIC_NUMBER )
     {
          count++;
          snprintf( str, LENGTH, "\
(%d) The 1st node in the list of available\n\
nodes has a bad magic number: %04X.\n",
                    count, free_nodes->magic );
          write_output( str );
     }

#endif

     if ( free_nodes->prev != NULL )
     {
          count++;
          snprintf( str, LENGTH, "\
(%d) free_nodes->prev is not NULL when it should be.\n", count );
          write_output( str );
     }

#ifdef USE_MAGIC_NUMBER

     if ( last_free_node != NULL &&
          last_free_node->magic != NODE_MAGIC_NUMBER )
     {
          count++;
          snprintf( str, LENGTH, "\
(%d) The node last_free_node has a bad magic number: %04X.\n",
                    count, last_free_node->magic );
          write_output( str );
     }

#endif

     if ( last_free_node != NULL )
     {
          if ( last_free_node->next != NULL )
          {
               count++;
               snprintf( str, LENGTH, "\
(%d) last_free_node->next is not NULL when it should be.\n", count );
               write_output( str );
          }
     }

     node = free_nodes->next;
     last = free_nodes;
     nodes = 1;

     while( node != NULL )
     {
          nodes++;

          /* Isolate the last digit. */

          tmp = nodes / 10;
          tmp = nodes - ( tmp * 10 );

#ifdef USE_MAGIC_NUMBER

          if ( node->magic != NODE_MAGIC_NUMBER )
          {
               count++;

               if ( tmp == 1 )
               {
                    snprintf( str, LENGTH, "\
(%d) The %dst node in the list of\n\
available nodes has a bad magic number: %04X.\n",
                              count, nodes, node->magic );
               }
               else if ( tmp == 2 )
               {
                    snprintf( str, LENGTH, "\
(%d) The %dnd node in the list of\n\
available nodes has a bad magic number: %04X.\n",
                              count, nodes, node->magic );
               }
               else if ( tmp == 3 )
               {
                    snprintf( str, LENGTH, "\
(%d) The %drd node in the list of\n\
available nodes has a bad magic number: %04X.\n",
                              count, nodes, node->magic );
               }
               else
               {
                    snprintf( str, LENGTH, "\
(%d) The %dth node in the list of\n\
available nodes has a bad magic number: %04X.\n",
                              count, nodes, node->magic );
               }
               write_output( str );
          }

#endif

          if ( node->pad != 0 )
          {
               count++;
               if ( tmp == 1 )
               {
                    snprintf( str, LENGTH, "\
(%d) The pad value in the %dst node is %d when it should be zero.\n",
                              count, nodes, node->pad );
               }
               else if ( tmp == 2 )
               {
                    snprintf( str, LENGTH, "\
(%d) The pad value in the %dnd node is %d when it should be zero.\n",
                              count, nodes, node->pad );
               }
               else if ( tmp == 3 )
               {
                    snprintf( str, LENGTH, "\
(%d) The pad value in the %drd node is %d when it should be zero.\n",
                              count, nodes, node->pad );
               }
               else
               {
                    snprintf( str, LENGTH, "\
(%d) The pad value in the %dth node is %d when it should be zero.\n",
                              count, nodes, node->pad );
               }
               write_output( str );
          }

          /*

               node->num must be zero if the node is in the list of
               available nodes.  Otherwise it must be greater than zero.

          */

          if ( node->num != 0 )
          {
               count++;
               if ( tmp == 1 )
               {
                    snprintf( str, LENGTH, "\
(%d) The node member num in the %dst node has an invalid value: %d.\n",
                              count, nodes, node->num );
               }
               else if ( tmp == 2 )
               {
                    snprintf( str, LENGTH, "\
(%d) The node member num in the %dnd node has an invalid value: %d.\n",
                              count, nodes, node->num );
               }
               else if ( tmp == 3 )
               {
                    snprintf( str, LENGTH, "\
(%d) The node member num in the %drd node has an invalid value: %d.\n",
                              count, nodes, node->num );
               }
               else
               {
                    snprintf( str, LENGTH, "\
(%d) The node member num in the %dth node has an invalid value: %d.\n",
                              count, nodes, node->num );
               }
               write_output( str );
          }

          /*

               Obviously last->next points to node
               or we wouldn't be able to access it.

          */

          if ( node->prev != last )
          {
               count++;
               if ( tmp == 1 )
               {
                    snprintf( str, LENGTH, "\
(%d) The %dst node has an invalid prev pointer value.\n",
                              count, nodes );
               }
               else if ( tmp == 2 )
               {
                    snprintf( str, LENGTH, "\
(%d) The %dnd node has an invalid prev pointer value.\n",
                              count, nodes );
               }
               else if ( tmp == 3 )
               {
                    snprintf( str, LENGTH, "\
(%d) The %drd node has an invalid prev pointer value.\n",
                              count, nodes );
               }
               else
               {
                    snprintf( str, LENGTH, "\
(%d) The %dth node has an invalid prev pointer value.\n",
                              count, nodes );
               }
               write_output( str );
          }

          last = node;
          node = node->next;
     }

     if ( last != last_free_node )
     {
          count++;
          snprintf( str, LENGTH, "\
(%d) The last_free_node pointer does not point\n\
to the last node in the list of available nodes.\n",
                    count );
          write_output( str );

          /* We couldn't check these things earlier. */

          if ( last_free_node == NULL )
          {
               if ( last->next != NULL )
               {
                    count++;
                    snprintf( str, LENGTH, "\
(%d) The last node->next pointer should be NULL, but it isn't.\n",
                              count );
                    write_output( str );
               }
          }    /* if ( last_free_node == NULL ) */
     }    /* if ( last != last_free_node ) */

     if ( nodes != free_size )
     {
          count++;
          snprintf( str, LENGTH, "\
(%d) The variable free_size: %d does not equal the\n\
number of nodes in the list of available nodes: %d.\n",
                    count, free_size, nodes );
          write_output( str );
     }

     if ( count != 0 )
     {
          error_code = bad_free;
     }
     else
     {
          error_code = success;
     }
     return count;
}

/* Writes the user's input to the output file. */

void write_input( const char *str )
{
     if ( str == NULL || str[ 0 ] == 0 )
     {
          return;
     }
     if ( outfile != NULL )
     {
          if ( fprintf( outfile, str ) < 1 )
          {
               fprintf( stderr,
                        "Failed to write to the output file.\n\n" );
               close_outfile();
          }
     }
     return;
}

/* Writes the program's output to stdout and the output file. */

void write_output( const char *str )
{
     if ( str == NULL || str[ 0 ] == 0 )
     {
          return;
     }
     fprintf( stdout, str );
     if ( outfile != NULL )
     {
          if ( fprintf( outfile, str ) < 1 )
          {
               fprintf( stderr,
                        "Failed to write to the output file.\n\n" );
               close_outfile();
          }
     }
     return;
}

/* Opens the output file. */

void open_outfile( const char *name )
{
     int save_errno;

     if ( outfile != NULL )
     {
          fprintf( stderr, "\
Error: Attempted to open the output file when it is already open.\n" );
          return;
     }
     if ( name == NULL )
     {
          fprintf( stderr,
                   "Error: NULL pointer passed to open_outfile().\n" );
          return;
     }
     if ( name[ 0 ] == 0 )
     {
          fprintf( stderr,
                   "Error: Empty string passed to open_outfile().\n" );
          return;
     }
     outfile = fopen( name, "w" );
     save_errno = errno;
     if ( outfile == NULL )
     {
          fprintf( stderr, "Failed to open the output file.\n" );
          fprintf( stderr, "Used:  \"%s\"\n", name );
          fprintf( stderr, "Error: %s.\n\n", strerror( save_errno ) );
     }
     return;
}

/* Closes the output file. */

void close_outfile( void )
{
     if ( outfile != NULL )
     {
          if ( ferror( outfile ) )
          {
               clearerr( outfile );
          }
          fflush( outfile );
          if ( ferror( outfile ) )
          {
               clearerr( outfile );
          }
          fclose( outfile );
          outfile = NULL;
     }
     else
     {
          fprintf( stderr, "\
Error: Attempted to close the output file when it is already closed.\n" );
     }
     return;
}

/*

     Reads the list data from a file.

     This function does not filter out duplicate records.
     Each node's data is written on its own line.

     Returns 0 for success, or 1 if an error occurred.

*/

int read_list_from_file( const char *name )
{
     FILE *fp;
     char buffer[ 12 ], str[ LENGTH ];
     int buffer_length = 12, exit_loop, num, ret, save_errno;
     node_t *current, *new_node;

     /* Make sure we have something we can use. */

     if ( name == NULL )
     {
          error_code = pointer;
          return 1;
     }
     if ( name[ 0 ] == 0 )
     {
          error_code = bad_arg;
          return 1;
     }

     /* Open the data file. */

     fp = fopen( name, "r" );
     save_errno = errno;
     if ( fp == NULL )
     {
          write_output( "Failed to open the data file for reading.\n" );
          ret = snprintf( str, LENGTH, "Used:  \"%s\"\n", name );
          write_output( str );
          if ( save_errno != 0 )
          {
               ret = snprintf( str, LENGTH, "Error: %s.\n",
                               strerror( save_errno ) );
               write_output( str );
          }
          error_code = bad_arg;
          return 1;
     }

     /* We'll need a fresh start. */

     if ( list != NULL )
     {
          ret = release_all_nodes();
          if ( ret != 0 )
          {
               write_output( "Failed to release the current list.\n" );
               fclose( fp );
               fp = NULL;

               /* 'error_code' will already be set. */

               return 1;
          }
          using_circular = 0;
     }

     /* Read the first line from the data file. */

     if ( fgets( buffer, buffer_length, fp ) == NULL )
     {
          save_errno = errno;
          write_output( "Failed to read anything from the input file.\n" );
          if ( feof( fp ) )
          {
               write_output( "\
The input file does not contain any data.\n" );
          }
          if ( save_errno != 0 )
          {
               ret = snprintf( str, LENGTH, "Error: %s.\n",
                               strerror( save_errno ) );
               write_output( str );
          }
          if ( ferror( fp ) )
          {
               clearerr( fp );
          }
          fclose( fp );
          fp = NULL;
          error_code = bad_arg;
          return 1;
     }

     /* The first line tells us whether the list is circular or linear. */

     if ( sscanf( buffer, "%d", &num ) != 1 )
     {
          write_output( "\
Unable to determine whether the saved list should be linear or circular.\n\
The data file for this saved list is corrupt.\n" );
          fclose( fp );
          fp = NULL;
          error_code = bad_arg;
          return 1;
     }

     /* Make sure we received a valid value. */

     if ( num != 0 && num != 1 )
     {
          write_output( "\
The information provided by the data file about the list type is invalid.\
\n" );
          write_output( "The value must be 0 or 1.\n" );
          ret = snprintf( str, LENGTH, "Value received: %d.\n", num );
          write_output( str );
          fclose( fp );
          error_code = bad_arg;
          return 1;
     }
     using_circular = num;

     /* Read the value for each node in the list. */

     current = list;
     exit_loop = 0;
     while( exit_loop == 0 && !feof( fp ) && !ferror( fp ) )
     {
          if ( fgets( buffer, buffer_length, fp ) != NULL )
          {
               if ( sscanf( buffer, "%d", &num ) == 1 )
               {
                    if ( num > 0 )
                    {
                         new_node = get_new_node();
                         if ( new_node == NULL )
                         {
                              ret = snprintf( str, LENGTH, "\
Failed to create node position %d in the list.  Error: %s\n",
                                              ( list_size + 1 ),
                                              error_string() );
                              write_output( str );
                              fclose( fp );
                              fp = NULL;
                              return 1;
                         }
                         new_node->num = num;
                         ret = insert_node_as_next( current, new_node );
                         if ( ret != 0 )
                         {
                              ret = snprintf( str, LENGTH, "\
Failed to add node position %d to the list.  Error: %s\n",
                                              ( list_size + 1 ),
                                              error_string() );
                              write_output( str );
                              fclose( fp );
                              fp = NULL;
                              return 1;
                         }
                         current = new_node;
                    }
                    else  /* num < 1 */
                    {
                         ret = snprintf( str, LENGTH, "\
The value provided by the data file for node\n\
position %d in the list is invalid: %d.\n", ( list_size + 1 ), num );
                         write_output( str );
                         fclose( fp );
                         fp = NULL;
                         error_code = bad_arg;
                         return 1;
                    }
               }
               else  /* sscanf() != 1 */
               {
                    ret = snprintf( str, LENGTH, "\
The value provided by the data file for\n\
node position %d is not an integer value.\n", ( list_size + 1 ) );
                    write_output( str );
                    fclose( fp );
                    fp = NULL;
                    error_code = bad_arg;
                    return 1;
               }
          }
          else  /* fgets() == NULL */
          {
               save_errno = errno;
               if ( feof( fp ) )
               {
                    if ( ferror( fp ) )
                    {
                         clearerr( fp );
                    }
                    fclose( fp );
                    fp = NULL;
                    exit_loop = 1;
               }
               else
               {
                    if ( save_errno == 0 )
                    {
                         ret = snprintf( str, LENGTH, "\
Error while reading from the data file.\n" );
                    }
                    else
                    {
                         ret = snprintf( str, LENGTH, "\
Error while reading from the data file.  Error: %s.\n",
                                         strerror( save_errno ) );
                    }
                    write_output( str );
                    if ( ferror( fp ) )
                    {
                         clearerr( fp );
                    }
                    fclose( fp );
                    fp = NULL;
                    error_code = success;  /* Nothing wrong in the list. */
                    return 1;
               }    /* feof( fp ) */
          }    /* fgets() != NULL */
     }    /* while() */

     /* Make sure the input file is closed. */

     if ( fp != NULL )
     {
          if ( ferror( fp ) )
          {
               clearerr( fp );
          }
          fclose( fp );
          fp = NULL;
     }

     error_code = success;
     return 0;
}

/*

     Writes the list data to a file.

     Each node's data is written on its own line.

     Returns 0 for success, or 1 if an error occurred.

*/

int write_list_to_file( const char *name )
{
     FILE *fp;
     char str[ LENGTH ];
     int ret, save_errno;
     node_t *current;

     /* Make sure we have something we can use. */

     if ( name == NULL )
     {
          error_code = pointer;
          return 1;
     }
     if ( name[ 0 ] == 0 )
     {
          error_code = bad_arg;
          return 1;
     }

     /* Make sure we have a list to save. */

     if ( list == NULL )
     {
          write_output( "No list to save.\n" );
          error_code = success;  /* Nothing wrong with the list. */
          return 1;
     }

     /* Check the list and its pointers. */

     if ( last_list_node == NULL )
     {
          error_code = bad_list;
          return 1;
     }
     if ( using_circular != 0 && using_circular != 1 )
     {
          error_code = bad_list;
          return 1;
     }

#ifdef USE_MAGIC_NUMBER

     current = list;
     while( current != NULL && current != last_list_node &&
            current->magic == NODE_MAGIC_NUMBER )
     {
          current = current->next;
     }
     if ( current == NULL )
     {
          error_code = bad_list;
          return 1;
     }
     if ( current->magic != NODE_MAGIC_NUMBER )
     {
          error_code = bad_magic;
          return 1;
     }

#endif

     /* Open the data file. */

     fp = fopen( name, "w" );
     save_errno = errno;
     if ( fp == NULL )
     {
          write_output( "Failed to open the data file for writing.\n" );
          ret = snprintf( str, LENGTH, "Used:  \"%s\"\n", name );
          write_output( str );
          if ( save_errno != 0 )
          {
               ret = snprintf( str, LENGTH, "Error: %s.\n",
                               strerror( save_errno ) );
               write_output( str );
          }
          error_code = bad_arg;
          return 1;
     }

     /* Save the list type. */

     ret = fprintf( fp, "%d\n", using_circular );
     save_errno = errno;
     if ( ferror( fp ) )
     {
          if ( save_errno == 0 )
          {
               ret = snprintf( str, LENGTH, "\
Error while writing to the data file.\n" );
          }
          else
          {
               ret = snprintf( str, LENGTH, "\
Error while writing to the data file.  Error: %s.\n",
                               strerror( save_errno ) );
          }
          write_output( str );
          clearerr( fp );
          fclose( fp );
          fp = NULL;
          error_code = success;  /* Nothing wrong with the list. */
          return 1;
     }
     if ( ret < 1 )  /* We didn't write any data. */
     {
          if ( save_errno == 0 )
          {
               ret = snprintf( str, LENGTH, "\
Failed to write to the data file.\n" );
          }
          else
          {
               ret = snprintf( str, LENGTH, "\
Failed to write to the data file.  Error: %s.\n", strerror( save_errno ) );
          }
          write_output( str );
          fclose( fp );
          fp = NULL;
          error_code = success;  /* Nothing wrong with the list. */
          return 1;
     }

     /* Save the data from each node in the list in the data file. */
     /* Each node is saved on a single line.                       */

     current = list;
     while( current != NULL && current != last_list_node )
     {
          ret = fprintf( fp, "%d\n", current->num );
          save_errno = errno;
          if ( ferror( fp ) )
          {
               if ( save_errno == 0 )
               {
                    ret = snprintf( str, LENGTH, "\
Error while writing to the data file.\n" );
               }
               else
               {
                    ret = snprintf( str, LENGTH, "\
Error while writing to the data file.  Error: %s.\n",
                                    strerror( save_errno ) );
               }
               write_output( str );
               clearerr( fp );
               fclose( fp );
               fp = NULL;
               error_code = success;  /* Nothing wrong with the list. */
               return 1;
          }
          if ( ret < 1 )  /* We didn't write any data. */
          {
               if ( save_errno == 0 )
               {
                    ret = snprintf( str, LENGTH, "\
Failed to write to the data file.\n" );
               }
               else
               {
                    ret = snprintf( str, LENGTH, "\
Failed to write to the data file.  Error: %s.\n", strerror( save_errno ) );
               }
               write_output( str );
               fclose( fp );
               fp = NULL;
               error_code = success;  /* Nothing wrong with the list. */
               return 1;
          }
          current = current->next;
     }

     if ( current == NULL )
     {
          if ( ferror( fp ) )
          {
               clearerr( fp );
          }
          fclose( fp );
          fp = NULL;
          error_code = bad_list;
          return 1;
     }

     /* Save the last node too. */

     ret = fprintf( fp, "%d\n", current->num );
     save_errno = errno;
     if ( ferror( fp ) )
     {
          if ( save_errno == 0 )
          {
               ret = snprintf( str, LENGTH, "\
Error while writing to the data file.\n" );
          }
          else
          {
               ret = snprintf( str, LENGTH, "\
Error while writing to the data file.  Error: %s.\n",
                               strerror( save_errno ) );
          }
          write_output( str );
          clearerr( fp );
          fclose( fp );
          fp = NULL;
          error_code = success;  /* Nothing wrong with the list. */
          return 1;
     }
     if ( ret < 1 )  /* We didn't write any data. */
     {
          if ( save_errno == 0 )
          {
               ret = snprintf( str, LENGTH, "\
Failed to write to the data file.\n" );
          }
          else
          {
               ret = snprintf( str, LENGTH, "\
Failed to write to the data file.  Error: %s.\n", strerror( save_errno ) );
          }
          write_output( str );
          fclose( fp );
          fp = NULL;
          error_code = success;  /* Nothing wrong with the list. */
          return 1;
     }

     fflush( fp );
     save_errno = errno;
     if ( ferror( fp ) )
     {
          if ( save_errno == 0 )
          {
               ret = snprintf( str, LENGTH, "\
Error while writing to the data file.\n" );
          }
          else
          {
               ret = snprintf( str, LENGTH, "\
Error while writing to the data file.  Error: %s.\n",
                               strerror( save_errno ) );
          }
          write_output( str );
          clearerr( fp );
          fclose( fp );
          fp = NULL;
          error_code = success;  /* Nothing wrong with the list. */
          return 1;
     }
     fclose( fp );
     fp = NULL;
     error_code = success;
     return 0;
}

#endif

/* EOF dlink_list.c */
