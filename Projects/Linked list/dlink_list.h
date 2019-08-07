/*

     dlink_list.h

     Written by Matthew Campbell.

*/

#ifndef _DLINK_LIST_H
#define _DLINK_LIST_H

/* How many bytes in the string buffer passed to write_output()? */

#define LENGTH 200

/*

     It is highly recommended that you
     define USE_MAGIC_NUMBER.  If you do
     not then a loop could go crazy if a
     pointer gets screwed up.

*/

#define USE_MAGIC_NUMBER

#ifdef USE_MAGIC_NUMBER
#define NODE_MAGIC_NUMBER 0x1245
#endif

/* Define _32BITS if your system uses 32 bit pointers or */
/* define _64BITS if your system uses 64 bit pointers.   */

#define _32BITS

#ifndef _64BITS
#ifndef _32BITS
#define _32BITS
#endif
#endif

/* Defined types: */

struct node_def
{

#ifdef USE_MAGIC_NUMBER

     short int magic;  /* Make sure it's a valid node.      */
     short int pad;    /* This helps with memory alignment. */

#endif

     /* Put data fields here. */

     int num;

     /* These point to other nodes. */

     struct node_def *next;
     struct node_def *prev;
};

typedef struct node_def node_t;

#ifdef USE_MAGIC_NUMBER

enum error_codes { success, no_malloc, pointer, bad_magic,
                   in_use, bad_arg, bad_list, bad_free };

#else

enum error_codes { success, no_malloc, pointer, in_use,
                   bad_arg, bad_list, bad_free };

#endif

/* Function prototypes for dlink_list.c: */

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

#endif  /* _DLINK_LIST_H */
