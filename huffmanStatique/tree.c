# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "tree.h"

// ===================================================================== //

tree new_tree(unsigned char l, int p){
  tree a;
  if ( (a = malloc(sizeof(node)) ) == NULL ) return NULL;
  a->right = a->left = NULL;
  a->letter = l;
  a->weight = p;
  a->is_leaf = 1;
  return a;
}


// ===================================================================== //

tree add_tree(tree a1, tree a2){
  tree a;
  if ( (a = malloc(sizeof(node)) ) == NULL ) exit(EXIT_FAILURE);
  a->left = a1;
  a->right = a2;
  a->letter = '\0';
  a->is_leaf = 0;
  a->weight = a1->weight + a2->weight;
  return a;
}

// ===================================================================== //

void destroy_tree(tree tr){
  if (tr->left!=NULL) destroy_tree(tr->left);
  if (tr->right!=NULL) destroy_tree(tr->right);
  tr->left=NULL;
  tr->right=NULL;
  free(tr);
  tr=NULL;
}

// ===================================================================== //

void print_node(tree n){

  if (n==NULL) return;

  if (!n->is_leaf){
    if (n->weight==0){
      printf("(   )\n");
      return;
    }
    printf("( %d )\n",n->weight);
    return;
  }

  if (n->letter=='\n'){
    printf("(\\n,%d)\n",n->weight);
    return;
  }

  if (n->letter=='\t'){
    printf("(\\t,%d)\n",n->weight);
    return;
  }

  printf("(%c,%d)\n",n->letter,n->weight);

}

// ===================================================================== //

void print_tree_aux(tree n, int prof, int is_right){
  if (n==NULL){ return; }
  for (int i=0; i<prof; i++) printf("|   \t");
  if (prof==0) printf("\n-ROOT-");
  else printf("\'--%d-->",is_right);
  print_node(n);
  print_tree_aux(n->right,prof+1,1);
  print_tree_aux(n->left,prof+1,0);
}

void print_tree(void* tr){
  tree n = (tree)tr;
  print_tree_aux(n,0,0);
}
