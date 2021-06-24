/*************************************************************************/
# ifndef HUFFMAN_H
# define HUFFMAN_H
/*************************************************************************/

# include "../list/list.h"
# include "tree.h"

elem* tree_list_get_by_char(list* l, unsigned char ch);
void sort_forest(list* l, elem* e);
list* tree_list_fread(char* filename);
tree tree_calculation(list* l);
void free_all(list* l);

/*************************************************************************/
# endif
/*************************************************************************/
