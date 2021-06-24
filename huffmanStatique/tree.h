/*************************************************************************/
# ifndef TREE_H
# define TREE_H
/*************************************************************************/

typedef struct node node;
typedef node* tree;

struct node{
  tree left;
  tree right;
  unsigned char letter;
  int weight;
  int is_leaf;
};

tree new_tree(unsigned char l, int p);
tree add_tree(tree a1, tree a2);
void print_tree(void* tr);
void destroy_tree(tree tr);

/*************************************************************************/
# endif
/*************************************************************************/
