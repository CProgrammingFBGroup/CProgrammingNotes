/*

     dlink_demo.c

     This is the demo program that uses the dlink_list.c library file.

     This program is for educational purposes
     and may be freely copied and distributed.

     Written by Matthew Campbell on Monday August 13, 2018.

     Updated on Tuesday August 14, 2018 to accommodate changes made to
     the dlink_list.c library file and to make some improvements.

     This file is 44,019 bytes long and contains 1,689 lines.

*/

#include <stdio.h>
#include <stdlib.h>

#include "dlink_list.h"

extern node_t *free_nodes, *last_free_node, *last_list_node, *list;
extern int error_code, free_size, list_size, using_circular;
extern FILE *outfile;

#ifdef USE_MAGIC_NUMBER

extern const int num_error_codes;

#else

extern const int num_error_codes;

#endif

/* Functions in this file: */

int process_menu( int menu );

/* Main menu level prototypes: */

void print_main_menu( void );
void print_add_nodes_menu( void );
void print_file_options_menu( void );
void show_registers( node_t *r1, node_t *r2 );
void release_node_r1( node_t *r1 );
void extract_node_r2( node_t **r2 );
void swap_nodes_r1_and_r2( node_t **r1, node_t **r2 );
void display_node_data_for_registers( node_t *r1, node_t *r2 );
void convert_list_to_circular_or_linear( void );
void release_all_the_nodes( void );
void free_all_the_nodes( void );
void count_the_list_nodes( void );
void count_the_free_nodes( void );
void find_the_nth_node( node_t **r1, node_t **r2 );
void find_a_node_by_number( node_t **r1, node_t **r2 );
void check_the_list_integrity( void );
void check_the_free_list_integrity( void );

/* Add nodes menu level prototypes: */

void create_new_nodes_before_r1( node_t *r1, int *last_num_used );
void create_new_nodes_after_r1( node_t *r1, int *last_num_used );
void add_new_nodes_before_r1( node_t *r1, int *last_num_used );
void add_new_nodes_after_r1( node_t *r1, int *last_num_used );
void add_node_r2_before_r1( node_t *r1, node_t *r2 );
void add_node_r2_after_r1( node_t *r1, node_t *r2 );

/* File options menu level prototypes: */

void load_from_a_file( node_t **r1, node_t **r2, int *last_num_used );
void save_to_a_file( void );

/* This will be linked in later: */

int read_stdin( char *buffer, const int length, const char *prompt );

/* Function definitions: */

int main( void )
{
     node_t *r1, *r2;
     int exit_program, last_num_used, menu, ret;

     /* Initialize important variables. */

     outfile = NULL;
     free_nodes = NULL;
     last_free_node = last_list_node = list = r1 = r2 = NULL;
     error_code = free_size = list_size = ret = using_circular = 0;

     last_num_used = 0;

     /* This is needed in the dlink_list.c library file. */

     if ( LENGTH < 110 )
     {
          printf( "The string buffer length is too short.\n" );
          exit( EXIT_FAILURE );
     }

#if 1

     ret = sizeof( int * );

#ifdef _32BITS

     if ( ret != 4 )
     {
          fprintf( stderr,
                   "Error: Invalid pointer size selected _32BITS.\n" );
          exit( EXIT_FAILURE );
     }

#endif

#ifdef _64BITS

     if ( ret != 8 )
     {
          fprintf( stderr,
                   "Error: Invalid pointer size selected _64BITS.\n" );
          exit( EXIT_FAILURE );
     }

#endif

#endif

     print_main_menu();
     exit_program = 0;
     menu = 0;  /* Main menu. */

     while( exit_program == 0 )
     {
          ret = process_menu( menu );
          if ( menu == 0 )
          {
               switch( ret )
               {
                    case -1: break;
                    case  0: print_main_menu();
                             break;
                    case  1: show_registers( r1, r2 );
                             break;
                    case  2: printf( "\n" );
                             list_nodes();
                             break;
                    case  3: printf( "\n" );
                             list_free_nodes();
                             break;
                    case  4: print_add_nodes_menu();
                             menu = 1;  /* Add nodes menu. */
                             break;
                    case  5: release_node_r1( r1 );
                             break;
                    case  6: extract_node_r2( &r2 );
                             break;
                    case  7: swap_nodes_r1_and_r2( &r1, &r2 );
                             break;
                    case  8: display_node_data_for_registers( r1, r2 );
                             break;
                    case  9: convert_list_to_circular_or_linear();
                             break;
                    case 10: release_all_the_nodes();
                             break;
                    case 11: free_all_the_nodes();
                             break;
                    case 12: count_the_list_nodes();
                             break;
                    case 13: count_the_free_nodes();
                             break;
                    case 14: find_the_nth_node( &r1, &r2 );
                             break;
                    case 15: find_a_node_by_number( &r1, &r2 );
                             break;
                    case 16: check_the_list_integrity();
                             break;
                    case 17: check_the_free_list_integrity();
                             break;
                    case 18: print_file_options_menu();
                             menu = 2;  /* File options menu. */
                             break;
                    case 19: printf( "\n" );
                             exit_program = 1;
                             break;
                    default: printf( "\nInvalid menu option.\n" );
                             printf( "Use option 0 to print the menu.\n" );
                             printf( "\n" );
                             break;
               }    /* switch( ret ) */
          }
          else if ( menu == 1 )
          {
               switch( ret )
               {
                    case -1: break;
                    case  0: print_add_nodes_menu();
                             break;
                    case  1: print_main_menu();
                             menu = 0;  /* Main menu. */
                             break;
                    case  2: show_registers( r1, r2 );
                             break;
                    case  3: create_new_nodes_before_r1( r1,
                                                         &last_num_used );
                             break;
                    case  4: create_new_nodes_after_r1( r1,
                                                        &last_num_used );
                             break;
                    case  5: add_new_nodes_before_r1( r1, &last_num_used );
                             break;
                    case  6: add_new_nodes_after_r1( r1, &last_num_used );
                             break;
                    case  7: add_node_r2_before_r1( r1, r2 );
                             break;
                    case  8: add_node_r2_after_r1( r1, r2 );
                             break;
                    default: printf( "\nInvalid menu option.\n" );
                             printf( "Use option 0 to print the menu.\n" );
                             printf( "\n" );
                             break;
               }    /* switch( ret ) */
          }
          else if ( menu == 2 )
          {
               switch( ret )
               {
                    case -1: break;
                    case  0: print_file_options_menu();
                             break;
                    case  1: print_main_menu();
                             menu = 0;  /* Main menu. */
                             break;
                    case  2: load_from_a_file( &r1, &r2, &last_num_used );
                             break;
                    case  3: save_to_a_file();
                             break;
                    default: printf( "\nInvalid menu option.\n" );
                             printf( "Use option 0 to print the menu.\n" );
                             printf( "\n" );
                             break;
               }    /* switch( ret ) */
          }
          else
          {
               printf( "\nThe menu selection variable is invalid.\n\n" );
               exit_program = 1;
          }
     }    /* while( exit_program == 0 ) */

     if ( release_all_nodes() != 0 )
     {
          printf( "\
Failed to release all the nodes in the list.\n" );
          if ( error_code != success )
          {
               printf( "Error: %s.\n\n", error_string() );
          }
          else
          {
               printf( "\n" );
          }
          exit( EXIT_FAILURE );
     }

     if ( free_all_nodes() != 0 )
     {
          printf( "\
Failed to free all the nodes in the list of available nodes.\n" );
          if ( error_code != success )
          {
               printf( "Error: %s.\n\n", error_string() );
          }
          else
          {
               printf( "\n" );
          }
          exit( EXIT_FAILURE );
     }

     printf( "Exiting.\n\n" );
     exit( EXIT_SUCCESS );
}

/* This function processes the user's menu choices. */

int process_menu( int menu )
{
     char str[ 4 ];
     int choice, ret;

     if ( menu == 0 )
     {
          ret = read_stdin( str, 4, "dlink: Main menu >> " );
          if ( ret != 0 )
          {
               printf( "\n\
Sorry, something went wrong while trying to read your response.\n" );
               printf( "Please try again.\n\n" );
               return ( -1 );  /* Silently loop around again. */
          }
          ret = sscanf( str, "%d", &choice );
          if ( ret != 1 )
          {
               printf( "\n\
Sorry, that is not a valid choice.  Please try again.\n\n" );
               return ( -1 );  /* Silently loop around again. */
          }
          if ( choice < 0 || choice > 19 )
          {
               printf( "\n\
Sorry, that is not a valid choice.  Please try again.\n\n" );
               return ( -1 );  /* Silently loop around again. */
          }
     }
     else if ( menu == 1 )
     {
          ret = read_stdin( str, 4, "dlink: Add nodes menu >> " );
          if ( ret != 0 )
          {
               printf( "\n\
Sorry, something went wrong while trying to read your response.\n" );
               printf( "Please try again.\n\n" );
               return ( -1 );  /* Silently loop around again. */
          }
          ret = sscanf( str, "%d", &choice );
          if ( ret != 1 )
          {
               printf( "\n\
Sorry, that is not a valid choice.  Please try again.\n\n" );
               return ( -1 );  /* Silently loop around again. */
          }
          if ( choice < 0 || choice > 8 )
          {
               printf( "\n\
Sorry, that is not a valid choice.  Please try again.\n\n" );
               return ( -1 );  /* Silently loop around again. */
          }
     }
     else if ( menu == 2 )
     {
          ret = read_stdin( str, 4, "dlink: File options menu >> " );
          if ( ret != 0 )
          {
               printf( "\n\
Sorry, something went wrong while trying to read your response.\n" );
               printf( "Please try again.\n\n" );
               return ( -1 );  /* Silently loop around again. */
          }
          ret = sscanf( str, "%d", &choice );
          if ( ret != 1 )
          {
               printf( "\n\
Sorry, that is not a valid choice.  Please try again.\n\n" );
               return ( -1 );  /* Silently loop around again. */
          }
          if ( choice < 0 || choice > 3 )
          {
               printf( "\n\
Sorry, that is not a valid choice.  Please try again.\n\n" );
               return ( -1 );  /* Silently loop around again. */
          }
     }
     else
     {
          return ( -2 );
     }
     return choice;
}

/* Functions used at the main menu level: */

/* This function prints the main menu to the screen. */

void print_main_menu( void )
{
     printf( "\n\
Main menu\n\n\
 0) Print this menu.\n\
 1) Show the registers.\n\
 2) Display the list.\n\
 3) Display the list of available nodes.\n\
 4) Add nodes to the list.\n\
 5) Release the node pointed to by R1 from the list.\n\
 6) Extract the node pointed to by R2 from the list and store it in R2.\n\
 7) Swap nodes R1 and R2 in the list.\n\
 8) Display the node data for the node pointed to by R1 or R2.\n\
 9) Make the list circular or linear.\n\
10) Release all the nodes in the list.\n\
11) Free all the nodes in the list of available nodes.\n\
12) Count the number of nodes in the list.\n\
13) Count the number of available nodes.\n\
14) Find the nth node in the list.\n\
15) Find a node in the list by its number.\n\
16) Check the integrity of the list.\n\
17) Check the integrity of the list of available nodes.\n\
18) Load a list from a file or save the list to a file.\n\
19) Exit.\n\n\
" );
}

/* This function prints the menu for adding nodes to the screen. */

void print_add_nodes_menu( void )
{
     printf( "\n\
Add nodes menu\n\n\
 0) Print this menu.\n\
 1) Return to the main menu.\n\
 2) Show the registers.\n\
 3) Create new nodes and add them to the list before R1.\n\
 4) Create new nodes and add them to the list after R1.\n\
 5) Add new nodes to the list before R1 using available nodes first.\n\
 6) Add new nodes to the list after R1 using available nodes first.\n\
 7) Add the node pointed to by R2 to the list before R1.\n\
 8) Add the node pointed to by R2 to the list after R1.\n\n\
" );
     return;
}

/* This function prints the file options menu to the screen. */

void print_file_options_menu( void )
{
     printf( "\n\
File options menu\n\n\
 0) Print this menu.\n\
 1) Return to the main menu.\n\
 2) Load a list from a file.\n\
 3) Save the list to a file.\n\n\
" );
     return;
}

/* This function prints the two user registers to the screen. */

void show_registers( node_t *r1, node_t *r2 )
{
     if ( r1 == NULL )
     {
          printf( "\nR1 is empty.\n" );
     }
     else
     {

#ifdef _32BITS

          printf( "\nR1 points to a node at %08X", ( unsigned int )r1 );

#else  /* _64BITS */

          printf( "\nR1 points to a node at %016X", ( unsigned int )r1 );

#endif

#ifdef USE_MAGIC_NUMBER

          if ( r1->magic != NODE_MAGIC_NUMBER )
          {
               printf( "\nIt does not appear to be a valid node.\n" );
          }
          else
          {
               printf( "  R1->num: %d\n", r1->num );
          }

#else

          printf( "  R1->num: %d\n", r1->num );

#endif

     }

     if ( r2 == NULL )
     {
          printf( "R2 is empty.\n\n" );
     }
     else
     {

#ifdef _32BITS

          printf( "R2 points to a node at %08X", ( unsigned int )r2 );

#else  /* _64BITS */

          printf( "R2 points to a node at %016X", ( unsigned int )r2 );

#endif

#ifdef USE_MAGIC_NUMBER

          if ( r2->magic != NODE_MAGIC_NUMBER )
          {
               printf( "\nIt does not appear to be a valid node.\n\n" );
          }
          else
          {
               printf( "  R2->num: %d\n\n", r2->num );
          }

#else

          printf( "  R2->num: %d\n\n", r2->num );

#endif

     }

     return;
}

/* This function releases node R1. */

void release_node_r1( node_t *r1 )
{
     int ret;

     if ( r1 == NULL )
     {
          printf( "\n\
R1 is empty.  You cannot use an empty register as an argument.\n\n" );
          return;
     }
     ret = release_node( r1 );
     if ( ret != 0 )
     {
          printf( "\n\
Failed to release node R1.  Error: %s.\n\n", error_string() );
          return;
     }
     printf( "\nSuccessful.\n\n" );
     return;
}

/* This function extracts node R2 and stores it in R2. */

void extract_node_r2( node_t **r2 )
{
     int ret;

     if ( r2 == NULL )
     {
          printf( "\nR2 is an invalid pointer.\n\n" );
          return;
     }
     if ( *r2 == NULL )
     {
          printf( "\n\
R2 is empty.  You cannot use an empty register as an argument.\n\n" );
          return;
     }
     ret = extract_node( *r2 );
     if ( ret != 0 )
     {
          printf( "\n\
Failed to extract node R2.  Error: %s.\n\n", error_string() );
          return;
     }
     printf( "\nSuccessful.\n\n" );
     return;
}

/* This function swaps nodes R1 and R2 in the list. */

void swap_nodes_r1_and_r2( node_t **r1, node_t **r2 )
{
     int ret;
     node_t *tmp;

     if ( r1 == NULL )
     {
          printf( "\nR1 is an invalid pointer.\n\n" );
          return;
     }
     if ( r2 == NULL )
     {
          printf( "\nR2 is an invalid pointer.\n\n" );
          return;
     }
     if ( *r1 == NULL )
     {
          printf( "\n\
R1 is empty.  You cannot use an empty register as an argument.\n\n" );
          return;
     }
     if ( *r2 == NULL )
     {
          printf( "\n\
R2 is empty.  You cannot use an empty register as an argument.\n\n" );
          return;
     }

     ret = swap_nodes( *r1, *r2 );
     if ( ret != 0 )
     {
          printf( "\n\
Sorry, that didn't work.  Error: %s.\n\n", error_string() );
          return;
     }
     tmp = *r1;
     *r1 = *r2;
     *r2 = tmp;
     tmp = NULL;
     printf( "\nSucessful.\n\n" );
     return;
}

/* This function displays the node data for the register nodes. */

void display_node_data_for_registers( node_t *r1, node_t *r2 )
{
     if ( r1 == NULL && r2 == NULL )
     {
         printf( "\nR1 is empty.  No data to display.\n" );
         printf( "R2 is empty.  No data to display.\n\n" );
         return;
     }
     if ( r1 == NULL )  /* R2 is not. */
     {
         printf( "\nR1 is empty.  No data to display.\n\nR2:\n" );
         display_node_data( r2 );
     }
     else if ( r2 == NULL ) /* R2 is empty. */
     {
         printf( "\nR1:\n" );
         display_node_data( r1 );
         printf( "R2 is empty.  No data to display.\n\n" );
     }
     else
     {
          printf( "\nR1:\n" );
          display_node_data( r1 );
          printf( "R2:\n" );
          display_node_data( r2 );
     }
     return;
}

/* This function converts the list to circular or linear. */

void convert_list_to_circular_or_linear( void )
{
     int ret;

     if ( using_circular == 0 )
     {
          ret = convert_list_to_circular();
          if ( ret != 0 )
          {
               printf( "\n\
Failed to convert the linear list to a circular list.\n" );
               if ( error_code != success )
               {
                    printf( "  Error: %s.\n\n", error_string() );
               }
               else
               {
                    printf( "\n\n" );
               }
          }
          else
          {
               printf( "\n\
Successfully converted the linear list to a circular list.\n\n" );
          }
     }
     else if ( using_circular == 1 )
     {
          ret = convert_list_to_linear();
          if ( ret != 0 )
          {
               printf( "\n\
Failed to convert the circular list to a linear list.\n" );
               if ( error_code != success )
               {
                    printf( "  Error: %s.\n\n", error_string() );
               }
               else
               {
                    printf( "\n\n" );
               }
          }
          else
          {
               printf( "\n\
Successfully converted the circular list to a linear list.\n\n" );
          }
     }
     else
     {
          printf( "\nThe variable using_circular is invalid.\n\n" );
     }
     return;
}

/*

     This function moves all the nodes in
     the list to the list of available nodes.

*/

void release_all_the_nodes( void )
{
     int ret;

     ret = release_all_nodes();
     if ( ret != 0 )
     {
          printf( "\nSorry, that didn't work." );
          if ( error_code != success )
          {
               printf( "  Error: %s.\n\n", error_string() );
          }
          else
          {
               printf( "\n\n" );
          }
          return;
     }
     printf( "\nSuccessful.\n\n" );
     return;
}

/* This function frees all the nodes in the list of available nodes. */

void free_all_the_nodes( void )
{
     int ret;

     ret = free_all_nodes();
     if ( ret != 0 )
     {
          printf( "\nSorry, that didn't work." );
          if ( error_code != success )
          {
               printf( "  Error: %s.\n\n", error_string() );
          }
          else
          {
               printf( "\n\n" );
          }
          return;
     }
     printf( "\nSuccessful.\n\n" );
     return;
}

/* This function counts the nodes in the list. */

void count_the_list_nodes( void )
{
     int ret;

     ret = count_list_nodes();
     if ( ret < 0 )
     {
          printf( "\nSorry, that didn't work.  Error: %s.\n\n",
                  error_string() );
          return;
     }
     else if ( ret == 0 )
     {
          printf( "\nThe list is empty.\n\n" );
     }
     else if ( ret == 1 )
     {
          printf( "\nThere is 1 node in the list.\n\n" );
     }
     else
     {
          printf( "\nThere are %d nodes in the list.\n\n", ret );
     }
     return;
}

/* This function counts the number of available nodes. */

void count_the_free_nodes( void )
{
     int ret;

     ret = count_free_nodes();
     if ( ret < 0 )
     {
          printf( "\nSorry, that didn't work.  Error: %s.\n\n",
                  error_string() );
          return;
     }
     else if ( ret == 0 )
     {
          printf( "\nThe list of available nodes is empty.\n\n" );
     }
     else if ( ret == 1 )
     {
          printf( "\n\
There is 1 node in the list of available nodes.\n\n" );
     }
     else
     {
          printf( "\n\
There are %d nodes in the list of available nodes.\n\n", ret );
     }
     return;
}

/*

     This function finds the nth node in
     the list and stores it in R1 or R2.

*/

void find_the_nth_node( node_t **r1, node_t **r2 )
{
     char str[ 13 ];
     int num, ret;
     node_t *tmp;

     if ( r1 == NULL )
     {
          printf( "\nR1 is an invalid pointer.\n\n" );
          return;
     }
     if ( r2 == NULL )
     {
          printf( "\nR2 is an invalid pointer.\n\n" );
          return;
     }

     printf( "\n" );
     ret = read_stdin( str, 13,
                       "What node position would you like to find? >> " );
     if ( ret != 0 )
     {
          printf( "\n\
Sorry, something went wrong while trying to read your response.\n" );
          return;
     }
     ret = sscanf( str, "%d", &num );
     if ( ret != 1 )
     {
          printf( "\nThat is not a valid integer number.\n\n" );
          return;
     }
     if ( num < 1 )
     {
          printf( "\nThat is not a valid choice.\n\n" );
          return;
     }
     tmp = find_nth_node( num );
     if ( tmp == NULL )
     {
          printf( "\nSorry, that didn't work." );
          if ( error_code != success )
          {
               printf( "  Error: %s.\n\n", error_string() );
          }
          else
          {
               printf( "\n\n" );
          }
          return;
     }
     printf( "\nWhere would you like to store the result?\n\n" );
     printf( "1) R1\n2) R2\n\n" );
     ret = read_stdin( str, 4, ">> " );
     if ( ret != 0 )
     {
          printf( "\n\
Sorry, something went wrong while trying to read your response.\n\n" );
          tmp = NULL;
          return;
     }
     ret = sscanf( str, "%d", &num );
     if ( ret != 1 )
     {
          printf( "\nThat is not a valid response.\n\n" );
          tmp = NULL;
          return;
     }
     if ( num == 1 )
     {
          *r1 = tmp;
          printf( "\nThe result has been stored in R1.\n\n" );
     }
     else if ( num == 2 )
     {
          *r2 = tmp;
          printf( "\nThe result has been stored in R2.\n\n" );
     }
     else
     {
          printf( "\nThat is not a valid option.\n\n" );
     }
     tmp = NULL;
     return;
}

/* This function finds a node by its number and stores it in R1 or R2. */

void find_a_node_by_number( node_t **r1, node_t **r2 )
{
     char str[ 13 ];
     int num, ret;
     node_t *tmp;

     if ( r1 == NULL )
     {
          printf( "\nR1 is an invalid pointer.\n\n" );
          return;
     }
     if ( r2 == NULL )
     {
          printf( "\nR2 is an invalid pointer.\n\n" );
          return;
     }

     printf( "\n" );
     ret = read_stdin( str, 13,
                       "What node number would you like to find? >> " );
     if ( ret != 0 )
     {
          printf( "\n\
Sorry, something went wrong while trying to read your response.\n" );
          return;
     }
     ret = sscanf( str, "%d", &num );
     if ( ret != 1 )
     {
          printf( "\nThat is not a valid integer number.\n\n" );
          return;
     }
     if ( num < 1 )
     {
          printf( "\nThat is not a valid choice.\n\n" );
          return;
     }
     tmp = find_node_by_number( num );
     if ( tmp == NULL )
     {
          printf( "\nSorry, that didn't work." );
          if ( error_code != success )
          {
               printf( "  Error: %s.\n\n", error_string() );
          }
          else
          {
               printf( "\n\n" );
          }
          return;
     }
     printf( "\nWhere would you like to store the result?\n\n" );
     printf( "1) R1\n2) R2\n\n" );
     ret = read_stdin( str, 4, ">> " );
     if ( ret != 0 )
     {
          printf( "\n\
Sorry, something went wrong while trying to read your response.\n\n" );
          tmp = NULL;
          return;
     }
     ret = sscanf( str, "%d", &num );
     if ( ret != 1 )
     {
          printf( "\nThat is not a valid response.\n\n" );
          tmp = NULL;
          return;
     }
     if ( num == 1 )
     {
          *r1 = tmp;
          printf( "\nThe result has been stored in R1.\n\n" );
     }
     else if ( num == 2 )
     {
          *r2 = tmp;
          printf( "\nThe result has been stored in R2.\n\n" );
     }
     else
     {
          printf( "\nThat is not a valid option.\n\n" );
     }
     tmp = NULL;
     return;
}

/* This function will check the integrity of the list. */

void check_the_list_integrity( void )
{
     int ret;

     printf( "\n" );
     ret = check_list_integrity();
     if ( ret < 0 )
     {
          printf( "\nSorry, that didn't work." );
          if ( error_code != success )
          {
               printf( "  Error: %s.\n\n", error_string() );
          }
          else
          {
               printf( "\n\n" );
          }
          return;
     }
     if ( ret == 0 )
     {
          printf( "The list is intact.  No errors were found.\n\n" );
     }
     else if ( ret == 1 )
     {
          printf( "\nFound 1 error in the list.\n\n" );
     }
     else
     {
          printf( "\nFound %d errors in the list.\n\n", ret );
     }
     return;
}

/* This function will check the integrity of the list of available nodes. */

void check_the_free_list_integrity( void )
{
     int ret;

     printf( "\n" );
     ret = check_free_list_integrity();
     if ( ret < 0 )
     {
          printf( "\nSorry, that didn't work." );
          if ( error_code != success )
          {
               printf( "  Error: %s.\n\n", error_string() );
          }
          else
          {
               printf( "\n\n" );
          }
          return;
     }
     if ( ret == 0 )
     {
          printf( "\
The list of available nodes is intact.  No errors were found.\n\n" );
     }
     else if ( ret == 1 )
     {
          printf( "\n\
Found 1 error in the list of available nodes.\n\n" );
     }
     else
     {
          printf( "\n\
Found %d errors in the list of available nodes.\n\n", ret );
     }
     return;
}

/* Functions used at the add nodes nenu level: */

/*

     This is a front-end function to create new nodes and add them to
     the list before R1 unless the list is empty in which case the new
     nodes will be added to the beginning of the list.

*/

void create_new_nodes_before_r1( node_t *r1, int *last_num_used )
{
     char str[ 11 ];
     int count, empty_list, nodes, ret;
     node_t *current, *new_node;

     if ( list != NULL )
     {
          if ( r1 == NULL )
          {
               printf( "\n\
R1 is empty.  You cannot use an empty register as an argument.\n\n" );
               return;
          }
          else
          {
               current = r1;
               empty_list = 0;
          }
     }
     else
     {
          current = NULL;
          empty_list = 1;
     }

     printf( "\n" );
     ret = read_stdin( str, 11, "\
How many new nodes would you like to add to the list? >> " );
     if ( ret != 0 )
     {
          printf( "\n\
Sorry, something went wrong while reading your response.\n\n" );
          return;
     }
     if ( sscanf( str, "%d", &nodes ) != 1 )
     {
          printf( "\nThat is not a valid integer number.\n\n" );
          return;
     }
     if ( nodes < 1 )
     {
          printf( "\nThat is not a valid response.\n\n" );
          return;
     }

     printf( "\n\
Attempting to create and add %d new nodes to the list...\n", nodes );

     for( count = 0; count < nodes; count++ )
     {
          new_node = make_node();
          if ( new_node == NULL )
          {
               printf( "\nFailed to create new node %d in this batch.\n",
                       count );
               printf( "Error: %s.\n\n", error_string() );
               return;
          }
          ( *last_num_used )++;
          new_node->num = *last_num_used;
          if ( empty_list == 0 )
          {
               ret = insert_node_as_previous( current, new_node );
          }
          else
          {
               ret = insert_node_as_next( current, new_node );
          }
          if ( ret != 0 )
          {
               printf( "\n\
Sorry, something went wrong while trying to add the new node to the list.\
\n" );
               printf( "Error: %s.\n\n", error_string() );
               ret = release_node( new_node );
               if ( ret != 0 )
               {
                    printf( "\n\
Failed to move the new node to the list of available nodes.\n" );
                    printf( "Error: %s.\n\n", error_string() );
                    ret = free_node( new_node );
                    if ( ret != 0 )
                    {
                         printf( "\n\
Failed to free the new node.  Error: %s.\n\n", error_string() );
                         printf( "Exiting.\n\n" );
                         exit( EXIT_FAILURE );
                    }
               }
               return;
          }
          else  /* ret == 0 */
          {
               if ( empty_list == 1 )
               {
                    current = new_node;
               }
          }
     }    /* for() */
     printf( "Successful.\n\n" );
     return;
}

/*

     This is a front-end function to create new nodes and add them to
     the list after R1 unless the list is empty in which case the new
     nodes will be added to the beginning of the list.

*/

void create_new_nodes_after_r1( node_t *r1, int *last_num_used )
{
     char str[ 11 ];
     int count, nodes, ret;
     node_t *current, *new_node;

     if ( list != NULL )
     {
          if ( r1 == NULL )
          {
               printf( "\n\
R1 is empty.  You cannot use an empty register as an argument.\n\n" );
               return;
          }
          else
          {
               current = r1;
          }
     }
     else
     {
          current = NULL;
     }

     printf( "\n" );
     ret = read_stdin( str, 11, "\
How many new nodes would you like to add to the list? >> " );
     if ( ret != 0 )
     {
          printf( "\n\
Sorry, something went wrong while reading your response.\n\n" );
          return;
     }
     if ( sscanf( str, "%d", &nodes ) != 1 )
     {
          printf( "\nThat is not a valid integer number.\n\n" );
          return;
     }
     if ( nodes < 1 )
     {
          printf( "\nThat is not a valid response.\n\n" );
          return;
     }

     printf( "\n\
Attempting to create and add %d new nodes to the list...\n", nodes );

     for( count = 0; count < nodes; count++ )
     {
          new_node = make_node();
          if ( new_node == NULL )
          {
               printf( "\nFailed to create new node %d in this batch.\n",
                       count );
               printf( "Error: %s.\n\n", error_string() );
               return;
          }
          ( *last_num_used )++;
          new_node->num = *last_num_used;
          ret = insert_node_as_next( current, new_node );
          if ( ret != 0 )
          {
               printf( "\n\
Sorry, something went wrong while trying to add the new node to the list.\
\n" );
               printf( "Error: %s.\n\n", error_string() );
               ret = release_node( new_node );
               if ( ret != 0 )
               {
                    printf( "\n\
Failed to move the new node to the list of available nodes.\n" );
                    printf( "Error: %s.\n\n", error_string() );
                    ret = free_node( new_node );
                    if ( ret != 0 )
                    {
                         printf( "\n\
Failed to free the new node.  Error: %s.\n\n", error_string() );
                         printf( "Exiting.\n\n" );
                         exit( EXIT_FAILURE );
                    }
               }
               return;
          }
          else  /* ret == 0 */
          {
               current = new_node;
          }
     }    /* for() */
     printf( "Successful.\n\n" );
     return;
}

/*

     This is a front-end function to add new nodes to the list before R1
     unless the list is empty in which case it will add the new nodes to
     the beginning of the list.

*/

void add_new_nodes_before_r1( node_t *r1, int *last_num_used )
{
     char str[ 11 ];
     int count, empty_list, nodes, ret;
     node_t *current, *new_node;

     if ( list != NULL )
     {
          if ( r1 == NULL )
          {
               printf( "\n\
R1 is empty.  You cannot use an empty register as an argument.\n\n" );
               return;
          }
          else
          {
               current = r1;
               empty_list = 0;
          }
     }
     else
     {
          current = NULL;
          empty_list = 1;
     }

     printf( "\n" );
     ret = read_stdin( str, 11, "\
How many new nodes would you like to add to the list? >> " );
     if ( ret != 0 )
     {
          printf( "\n\
Sorry, something went wrong while reading your response.\n\n" );
          return;
     }
     if ( sscanf( str, "%d", &nodes ) != 1 )
     {
          printf( "\nThat is not a valid integer number.\n\n" );
          return;
     }
     if ( nodes < 1 )
     {
          printf( "\nThat is not a valid response.\n\n" );
          return;
     }

     printf( "\n\
Attempting to create and add %d new nodes to the list...\n", nodes );

     for( count = 0; count < nodes; count++ )
     {
          new_node = get_new_node();
          if ( new_node == NULL )
          {
               printf( "\nFailed to create new node %d in this batch.\n\n",
                       count );
               printf( "Error: %s.\n", error_string() );
               return;
          }
          ( *last_num_used )++;
          new_node->num = *last_num_used;
          if ( empty_list == 0 )
          {
               ret = insert_node_as_previous( current, new_node );
          }
          else
          {
               ret = insert_node_as_next( current, new_node );
          }
          if ( ret != 0 )
          {
               printf( "\n\
Sorry, something went wrong while trying to add the new node to the list.\
\n" );
               printf( "Error: %s.\n\n", error_string() );
               ret = release_node( new_node );
               if ( ret != 0 )
               {
                    printf( "\n\
Failed to move the new node to the list of available nodes.\n" );
                    printf( "Error: %s.\n\n", error_string() );
                    ret = free_node( new_node );
                    if ( ret != 0 )
                    {
                         printf( "\n\
Failed to free the new node.  Error: %s.\n\n", error_string() );
                         printf( "Exiting.\n\n" );
                         exit( EXIT_FAILURE );
                    }
               }
               return;
          }
          else  /* ret == 0 */
          {
               if ( empty_list == 1 )
               {
                    current = new_node;
               }
          }
     }    /* for() */
     printf( "Successful.\n\n" );
     return;
}

/*

     This is a front-end function to add new nodes to the list after R1
     unless the list is empty in which case it will add the new nodes
     to the beginning of the list.

*/

void add_new_nodes_after_r1( node_t *r1, int *last_num_used )
{
     char str[ 11 ];
     int count, nodes, ret;
     node_t *current, *new_node;

     if ( list != NULL )
     {
          if ( r1 == NULL )
          {
               printf( "\n\
R1 is empty.  You cannot use an empty register as an argument.\n\n" );
               return;
          }
          else
          {
               current = r1;
          }
     }
     else
     {
          current = NULL;
     }

     printf( "\n" );
     ret = read_stdin( str, 11, "\
How many new nodes would you like to add to the list? >> " );
     if ( ret != 0 )
     {
          printf( "\n\
Sorry, something went wrong while reading your response.\n\n" );
          return;
     }
     if ( sscanf( str, "%d", &nodes ) != 1 )
     {
          printf( "\nThat is not a valid integer number.\n\n" );
          return;
     }
     if ( nodes < 1 )
     {
          printf( "\nThat is not a valid response.\n\n" );
          return;
     }

     printf( "\n\
Attempting to create and add %d new nodes to the list...\n", nodes );

     for( count = 0; count < nodes; count++ )
     {
          new_node = get_new_node();
          if ( new_node == NULL )
          {
               printf( "\nFailed to create new node %d in this batch.\n\n",
                       count );
               printf( "Error: %s.\n\n", error_string() );
               return;
          }
          ( *last_num_used )++;
          new_node->num = *last_num_used;
          ret = insert_node_as_next( current, new_node );
          if ( ret != 0 )
          {
               printf( "\n\
Sorry, something went wrong while trying to add the new node to the list.\
\n" );
               printf( "Error: %s.\n\n", error_string() );
               ret = release_node( new_node );
               if ( ret != 0 )
               {
                    printf( "\n\
Failed to move the new node to the list of available nodes.\n" );
                    printf( "Error: %s.\n\n", error_string() );
                    ret = free_node( new_node );
                    if ( ret != 0 )
                    {
                         printf( "\n\
Failed to free the new node.  Error: %s.\n\n", error_string() );
                         printf( "Exiting.\n\n" );
                         exit( EXIT_FAILURE );
                    }
               }
               return;
          }
          else
          {
               current = new_node;
          }
     }    /* for() */
     printf( "Successful.\n\n" );
     return;
}

/*

     This is a front-end function to add the node R2 before R1.

*/

void add_node_r2_before_r1( node_t *r1, node_t *r2 )
{
     int ret;

     if ( r1 == NULL )
     {
          printf( "\n\
R1 is empty.  You cannot use an empty register as an argument.\n\n" );
          return;
     }
     if ( r2 == NULL )
     {
          printf( "\n\
R2 is empty.  You cannot use an empty register as an argument.\n\n" );
          return;
     }
     ret = insert_node_as_previous( r1, r2 );
     if ( ret != 0 )
     {
          printf( "\nSorry, that didn't work." );
          if ( error_code != success )
          {
               printf( "  Error: %s\n\n", error_string() );
          }
          else
          {
               printf( "\n\n" );
          }
     }
     else
     {
          printf( "\nInsertion successful.\n\n" );
     }
     return;
}

/*

     This is a front-end function to add the node R2 after R1.

*/

void add_node_r2_after_r1( node_t *r1, node_t *r2 )
{
     int ret;

     if ( r1 == NULL )
     {
          printf( "\n\
R1 is empty.  You cannot use an empty register as an argument.\n\n" );
          return;
     }
     if ( r2 == NULL )
     {
          printf( "\n\
R2 is empty.  You cannot use an empty register as an argument.\n\n" );
          return;
     }
     ret = insert_node_as_next( r1, r2 );
     if ( ret != 0 )
     {
          printf( "\nSorry, that didn't work." );
          if ( error_code != success )
          {
               printf( "  Error: %s\n\n", error_string() );
          }
          else
          {
               printf( "\n\n" );
          }
     }
     else
     {
          printf( "\nInsertion successful.\n\n" );
     }
     return;
}

/* Functions used at the file options menu level: */

/* This function is a front-end that loads a list from a file. */

void load_from_a_file( node_t **r1, node_t **r2, int *last_num_used )
{
     char str[ 1024 ];
     int ret;

     printf( "\nLoading a new list from a file...\n\n" );

     printf( "What is the name of the file that the list is stored in?\n" );
     ret = read_stdin( str, 1024, ">> " );
     if ( ret != 0 )
     {
          printf( "\n\
Sorry, something went wrong while reading your response.\n\n" );
          return;
     }
     ret = read_list_from_file( str );
     if ( ret != 0 )
     {
          printf( "\n\
Sorry, something went wrong while trying to load the list from the file.\
\n\n" );
          if ( list == NULL )
          {
               *last_num_used = 0;
               *r1 = *r2 = NULL;
          }
          return;
     }

     ret = find_highest_node_number();
     if ( ret == 0 && list_size > 0 )
     {
          ret = ( -1 );
     }
     if ( ret < 0 )
     {
          printf( "\nThe list found in the file is corrupt.\n\n" );
          if ( release_all_nodes() != 0 )
          {
               printf( "\
Failed to release all the nodes in the list.\n" );
               if ( error_code != success )
               {
                    printf( "Error: %s.\n\n", error_string() );
               }
               else
               {
                    printf( "\n" );
               }
               exit( EXIT_FAILURE );
          }

          if ( free_all_nodes() != 0 )
          {
               printf( "\
Failed to free all the nodes in the list of available nodes.\n" );
               if ( error_code != success )
               {
                    printf( "Error: %s.\n\n", error_string() );
               }
               else
               {
                    printf( "\n" );
               }
               exit( EXIT_FAILURE );
          }
          last_list_node = list = *r1 = *r2 = NULL;
          *last_num_used = list_size = 0;
          error_code = success;
          return;
     }

     *last_num_used = ret;
     *r1 = *r2 = NULL;
     printf( "\nThe new list has been loaded.\n\n" );
     return;
}

/* This function is a front-end that saves the list to a file. */

void save_to_a_file( void )
{
     char str[ 1024 ];
     int ret;

     printf( "\nSaving the list to a file...\n\n" );

     printf( "What is the name of the file that you would like to use?\n" );
     ret = read_stdin( str, 1024, ">> " );
     if ( ret != 0 )
     {
          printf( "\n\
Sorry, something went wrong while reading your response.\n\n" );
          return;
     }
     ret = write_list_to_file( str );
     if ( ret != 0 )
     {
          printf( "\n\
Sorry, something went wrong while trying to save the list to the file.\
\n\n" );
          return;
     }
     printf( "\nThe list has been saved to \"%s\".\n\n", str );
     return;
}

/* EOF dlink_demo.c */
