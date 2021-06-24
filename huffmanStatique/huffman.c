# include <stdio.h> // FILE, fopen, fread
# include <stdlib.h> // exit, free
# include <netdb.h>   // uint64_t
# include <string.h> // memset
# include "huffman.h"
# include "tree.h"

# define CHAR_MAX_NUMBER 256
# define NB_BITS 64

// ===================================================================== //
// Variables globales ================================================== //
// ===================================================================== //

// Arbre de Huffman
tree global_huffman_tree;

// Encodage de l'arbre pour l'en-tête
unsigned char binStructure[1024];

// Table de conversion
// (l'indice i correspond au caractère '\0'+i)
// (la valeur converter[i] correspond au nombre binaire sur 64 bits)
uint64_t converter[CHAR_MAX_NUMBER];

// Longueur du nombre binaire (car taille variable)
int binary_size[CHAR_MAX_NUMBER];

// Longueur de l'en-tête
int hlength;

// ===================================================================== //
// Fonctions =========================================================== //
// ===================================================================== //

// ===================================================================== //

// Retourne un noeud (dans la liste de noeuds) correspondant au char ch

elem* tree_list_get_by_char(list* l, unsigned char ch){
  if (l==NULL) return NULL;
  elem* e1 = l->first;
  elem* e2 = l->last;
  for (int i=0; i<l->size; i++){
    if ( ((tree)e1->data)->letter == ch ) return e1;
    if ( ((tree)e2->data)->letter == ch ) return e2;
    e1 = e1->next;
    e2 = e2->prev;
  }
  return NULL;
}

// ===================================================================== //

// Tri de la forêt selon l'algorithme de Huffman

void sort_forest(list* l, elem* e){
  tree tr = (tree)(e->data);
  // tri
  elem* elt = e;
  if (elt->next==NULL) return; // déja en fin de liste
  while (elt->next !=NULL){
    if (((tree)elt->next->data)->weight >= tr->weight){
      break; // break while
    }
    elt=elt->next;
  }
  // on n'est pas à la bonne position
  if (elt!=e){
    tr = list_remove(l,e);
    list_insert_after(l,tr,elt);
  }
}

// ===================================================================== //

// Contruction de la forêt à partir du fichier original

list* tree_list_fread(char* filename){

  // Ouverture du fichier en lecture seule
  list* l = new_list();
  FILE *pf = fopen(filename,"r");
  if (pf==NULL){
    printf("Le fichier \"%s\" n'a pas été trouvé.\n",filename);
    free(l);
    exit(EXIT_FAILURE);
  }

  // Initialisation du buffer
  unsigned char buf[1];

  // Stream du fichier et construction de la forêt

  int rd = 1;
  while(rd>0){
    rd = fread(buf,sizeof(unsigned char),1,pf); // Lecture d'un caractère à la fois
    if (rd==0){
      buf[0]=3;
      tree tr = new_tree(*buf,1);
      list_add(l,tr,0);
      break;
    }
    elem* e = tree_list_get_by_char(l,*buf); // on cherche un noeud

    // Si on rencontre un nouveau caractère, on créé un nouveau noeud
    if (e==NULL){
      tree tr = new_tree(*buf,1);
      list_add(l,tr,0);
    }

    // Sinon, mise à jour du poids du noeud et tri
    else {
      tree tr = (tree)(e->data);
      tr->weight++;
      sort_forest(l,e);
    }

  } // fin for

  fclose(pf);
  return l;

}

// ===================================================================== //

// Construction de l'arbre final à partir de la forêt

tree tree_calculation(list* l){

  while(l->size>1){
    elem* e1 = l->first;
    elem* e2 = e1->next;
    tree tr1 = (tree)list_remove(l,e1);
    tree tr2 = (tree)list_remove(l,e2);
    tree tr = add_tree(tr1,tr2);
    list_add(l,tr,0);
    elem* e = list_get_by_data(l,(void*)tr);
    sort_forest(l,e);
  }
  return (tree)(l->first->data);

}

// ===================================================================== //

// Remplissage du dictionnaire de conversion en fonction de l'arbre de Huffman

void fill_table_aux(tree n, uint64_t binary, int is_right, int len){
  if (n==NULL){ return; }
  if (is_right!=-1){
    binary = (binary << 1) | (uint64_t)is_right ;
    len++;
  }
  if (n->is_leaf){
    converter[(int)(n->letter)] = binary ;
    binary_size[(int)(n->letter)] = len;
  }
  fill_table_aux(n->right,binary,1,len);
  fill_table_aux(n->left,binary,0,len);

}

void fill_table(void* tr){
  memset(converter,0,CHAR_MAX_NUMBER*sizeof(uint64_t));
  tree n = (tree)tr;
  fill_table_aux(n,0,-1,0);
}

// ===================================================================== //

// Affichage du code binaire correspondant au caractère c (pour les tests)

void print_converted(unsigned char c){
  uint64_t x = converter[c];
  int b[64];
  int i;
  for(i=0; x > 0; i++){
    b[i] = x%2;
    x = x/2;
  }
  int len = binary_size[c];
  printf("\t\tlen=%d\t\t",len);
  for(int k=len; k>i; k--){
    printf("0");
  }
  for(i=i-1; i >= 0; i--){
    printf("%d",b[i]);
  }
  printf("\n");
}

// ===================================================================== //

// Décomposition d'un octet c en tableau de 8 nombres binaires.

void decompose_binary(unsigned char c, unsigned char* binary_buf){
  unsigned char x = c;
  char b[8];
  memset(binary_buf,0,8);
  memset(b,0,8);
  int i;
  for(i=0; x > 0; i++){
    b[i] = x%2;
    x = x/2;
  }
  for(i=i-1; i >= 0; i--){
    binary_buf[7-i]=b[i];
  }
}

// ===================================================================== //

// Affichage bit à bit de l'octet c (pour les tests)

void print_binary(unsigned char c){
  unsigned char x = c;
  int b[8];
  int i;
  for(i=0; x > 0; i++){
    b[i] = x%2;
    x = x/2;
  }
  if (c<128){
    printf("0");
  }
  for(i=i-1; i >= 0; i--){
    printf("%d",b[i]);
  }
  printf("\n");
}

// ===================================================================== //

// Affichage du dictionnaire de conversion (pour les tests)

void print_dictionary(){
  for (int i=0; i<CHAR_MAX_NUMBER; i++){
    if(binary_size[i]==0) continue;
    printf("T[%c]=\t",(unsigned char)i);
    print_converted((unsigned char)i);
  }
}

// ===================================================================== //

// Compression du fichier

void compress(char* filename,char* newfilename){
  // Ouverture du fichier en lecture seule
  FILE *pf = fopen(filename,"r");
  if (pf==NULL){
    printf("Le fichier \"%s\" n'a pas été trouvé.\n",filename);
    exit(EXIT_FAILURE);
  }
  rewind(pf);

  // Création du fichier compréssé
  FILE *dst = fopen(newfilename,"w");
  rewind(pf);

  // Ecriture de l'en-tête
  fwrite(binStructure,sizeof(unsigned char),hlength,dst);

  // Initialisation du buffer
  unsigned char buf[1];
  unsigned char binary_buf[1];

  // 4 - Stream du fichier et compression

  int rd = 1; // Nombre d'octets lu (initialisé à arbitrairement à un nombre > 0)
  unsigned char flag = 0; // Nombre de bits utilisés dans l'octet
  binary_buf[0] = 0; // valeur du buffer initial
  int end = 0; // Marqueur de fin de stream (pour ajouter le EOF)

  while(rd>0 && !end){
    //printf("=================\n");
    rd = fread(buf,sizeof(unsigned char),1,pf); // Lecture d'un caractère à la fois
    if (rd==0){
      buf[0]=3; // Ascii == 3 pour EOF
      end=1;
    }

    uint64_t bin = converter[(*buf)]; // code binaire du caractère
    int binlen = binary_size[(*buf)]; // longueur du code binaire du caractère
    int space = sizeof(unsigned char)*8-binlen-flag; // potentiel espace restant

    while (space < 0){ // Pas assez de bits pour mettre bin dans l'octet
      int dec = binlen-(sizeof(unsigned char)*8-flag);
      uint64_t tmp = bin >> dec;
      binary_buf[0] |= (unsigned char)tmp;
      fwrite(binary_buf,sizeof(unsigned char),1,dst);
      tmp = 0xFFFFFFFFFFFFFFFF;
      tmp >>= sizeof(tmp)*8-(dec);
      binary_buf[0] = 0;
      flag = 0;
      binlen = dec;
      bin &= tmp;
      space = sizeof(unsigned char)*8-binlen-flag;
    }

    bin <<= (sizeof(unsigned char)*8-binlen-flag);
    binary_buf[0] |= (unsigned char)bin ;
    flag = flag + binlen;

    if (flag==sizeof(unsigned char)*8){
      fwrite(binary_buf,sizeof(unsigned char),1,dst);
      binary_buf[0] = 0;
      flag = 0;
    }

  } // fin for

  fclose(pf);
  fclose(dst);

}


// ===================================================================== //

// Décompression du fichier

void decompress(char* filename,char* newfilename){
  // Ouverture du fichier compréssé en lecture seule
  FILE *pf = fopen(filename,"r");
  if (pf==NULL){
    printf("Le fichier \"%s\" n'a pas été trouvé.\n",filename);
    exit(EXIT_FAILURE);
  }
  fseek(pf,hlength,SEEK_SET);

  // Création du fichier décompréssé
  FILE *dst = fopen(newfilename,"w");
  rewind(dst);

  // Initialisation des buffers
  unsigned char octet_buf[1];
  unsigned char letter_buf[1];
  unsigned char binary_buf[8];

  // Stream du fichier compressé et decompression

  int rd = 1; // Nombre d'octets lu (initialisé à arbitrairement à un nombre > 0)

  // Noeud courant
  tree tmp = global_huffman_tree;

  while(rd>0){
    rd = fread(octet_buf,sizeof(unsigned char),1,pf); // Lecture d'un caractère à la fois
    if (rd==0) break; // fin du fichier
    decompose_binary(octet_buf[0],binary_buf); // pour faciliter le parcours bit à bit

    // Parcours dans l'arbre
    for (int i=0; i<8; i++){

      // Pas une lettre
      if ( !(tmp->is_leaf) ){
        if (binary_buf[i]==0){ // 0 pour gauche
          tmp = tmp->left;
        }
        else if (binary_buf[i]==1){ // 1 pour droite
          tmp = tmp->right;
        }
        else { // Erreur (nombre binaire différent de 0 et 1)
          printf("Error binary_buf\n");
          exit(0);
        }
      }

      // C'est une lettre
      else {
        letter_buf[0] = tmp->letter;
        if (letter_buf[0]==3){ // EOF
          fclose(pf);
          fclose(dst);
          return;
        }
        fwrite(letter_buf,sizeof(unsigned char),1,dst);
        tmp = global_huffman_tree;
        i--;
      }
    }
  }

  // Si on n'a pas détecté le EOF
  fclose(pf);
  fclose(dst);

}

// ===================================================================== //

// Liberation de la mémoire

void free_all(list* l){
  while(l->size>0){
    tree tr = list_remove(l,l->first);
    destroy_tree(tr);
  }
  free(l);
}

// ===================================================================== //

// Encodage de l'arbre en texte (pour l'en-tête)

void encodeTreeAux(tree n, unsigned char* binStructure){
  if (n==NULL){
    return;
  }
  if (n->letter == '\0'){
    binStructure[hlength]=0;
    hlength++;
  }
  else {
    binStructure[hlength]=1;
    hlength++;
    binStructure[hlength]=n->letter;
    hlength++;
  }
  encodeTreeAux(n->left,binStructure);
  encodeTreeAux(n->right,binStructure);

}

void encodeTree(tree n,  unsigned char* binStructure){

  hlength = 0;
  // Char de sécurité (début en tête)
  binStructure[0]='#';
  binStructure[1]='{';
  hlength+=2;

  encodeTreeAux(n,binStructure);

  // Char de sécurité (fin en tête)
  binStructure[hlength]='}';
  binStructure[hlength+1]='#';
  hlength+=2;

  /*
  for (int i=0; i<hlength; i++){
    if (binStructure[i]<2){
        printf("%d",binStructure[i]);
    }
    else {
      printf("%c",binStructure[i]);
    }
  }
  printf("\n");
  */

}

// ===================================================================== //

// Décodage de l'en-tête en arbre

tree decodeTreeAux(FILE* fdst, int root){

  int rd = 1;
  char buf[1];

  memset(buf,0,1);
  // fdst est sensé être open
  rd = fread(buf,sizeof(unsigned char),1,fdst);
  if (rd<0){
    printf("En-tête incorrect.\n");
    exit(0);
  }
  hlength++;

  // Affichage (pour les tests)

  /*
  if (buf[0]<2) printf("%d",buf[0]);
  else printf("%c",buf[0]);
  */

  // Decodage 0
  if (buf[0]==0){
    tree tr = new_tree(0,0);
    tr->is_leaf = 0;
    if (root) global_huffman_tree = tr;
    tr->left = decodeTreeAux(fdst,0);
    tr->right = decodeTreeAux(fdst,0);
    return tr;
  }

  // Decodage 1
  else if (buf[0]==1){
    memset(buf,0,1);
    rd = fread(buf,sizeof(unsigned char),1,fdst);
    if (rd<0){
      printf("En-tête incorrect.\n");
      exit(0);
    }
    hlength++;
    tree tr = new_tree(buf[0],0);
    return tr;
  }
  return global_huffman_tree;
}


tree decodeTree(char* dst){
  hlength = 0;
  FILE *fdst = fopen(dst,"r");
  if (fdst==NULL){
    printf("Le fichier \"%s\" n'a pas été trouvé.\n",dst);
    exit(EXIT_FAILURE);
  }
  rewind(fdst);
  char buf[1];

  // Verifier les premiers char de sécurité de l'en-tête
  int rd = fread(buf,sizeof(unsigned char),1,fdst);
  if (rd<=0 || buf[0]!='#'){
    printf("Début d'en-tête incorrect.\n");
    fclose(fdst);
    exit(0);
  }
  hlength++;

  rd = fread(buf,sizeof(unsigned char),1,fdst);
  if (rd<=0 || buf[0]!='{'){
    printf("Début d'en-tête incorrect\n");
    fclose(fdst);
    exit(0);
  }
  hlength++;

  // Construction de l'arbre
  global_huffman_tree = NULL;
  global_huffman_tree = decodeTreeAux(fdst,1);

  // Verifier la fin de l'en-tête
  rd = fread(buf,sizeof(unsigned char),1,fdst);
  if (rd<=0 || buf[0]!='}'){
    printf("Fin d'en-tête incorrect\n");
    print_tree(global_huffman_tree);
    fclose(fdst);
    exit(0);
  }
  hlength++;

  rd = fread(buf,sizeof(unsigned char),1,fdst);
  if (rd<=0 || buf[0]!='#'){
    printf("Fin d'en-tête incorrect.\n");
    fclose(fdst);
    exit(0);
  }
  hlength++;
  fclose(fdst);
  return global_huffman_tree;

}

// ===================================================================== //
// ===================================================================== //
// ===================================================================== //

int main(int argc, char* argv[]){

  if (argc==1){
    printf("Veuillez choisir une option pour la compression (-c) ou  la décompression (-d).\n");
    return 0;
  }

  if (argc==2){
    printf("Veuillez passer le nom du fichier à compresser en premier argument.\n");
    return 0;
  }

  if (argc==3){
    printf("Veuillez choisir un nom pour le fichier de destination, passez le en deuxième argument.\n");
    return 0;
  }

  // ------------------------------------------------------------------- //
  // COMPRESSION
  // ------------------------------------------------------------------- //

  if (strcmp (argv[1],"-c")==0){
    printf("Compression en cours.\n ... \n");

    char* text = argv[2];
    char* dst = argv[3];

    // Construction de la forêt initiale
    list* forest = tree_list_fread(text);
    /*
    void (*pfunc)(void*) = &print_tree;
    print_list(forest,(*pfunc));
    */

    // Calcul de l'arbre final
    global_huffman_tree = tree_calculation(forest);
    /*
    print_list(forest,(*pfunc));
    */

    // Calcul de l'en-tête
    encodeTree(global_huffman_tree,binStructure);

    // Conversion en binaire
    fill_table(global_huffman_tree);

    // Affichage du dictionnaire "caractères <-> codes binaires" (juste pour les tests)
    /*
    print_dictionary();
    */

    // Compression
    compress(text,dst);
    printf("Compression effectuée.\n");


    // Lecture du fichier compressé : juste pour les tests (verifier le texte attendu)

    /*
    FILE *fdst = fopen(dst,"r");
    int rd = 1;
    char buf[1];
    printf("_____________________\n");
    int oct = 1;
    while(rd>=1){
      if (oct==hlength+1) printf("_____________________\n");
      memset(buf,0,1);
      rd = fread(buf,sizeof(unsigned char),1,fdst);
      printf("%d\t",oct);
      print_binary((unsigned char)buf[0]);
      oct++;
    }
    fclose(fdst);
    */

    free_all(forest);
    forest=NULL;
    global_huffman_tree=NULL;
    return 0;
  }

  // ------------------------------------------------------------------- //
  // DECOMPRESSION
  // ------------------------------------------------------------------- //

  else if (strcmp (argv[1],"-d")==0) {
    printf("Décompression en cours.\n ... \n");

    // Lecture de l'en-tête
    char* dst = argv[2];
    char* text = argv[3];
    global_huffman_tree = decodeTree(dst);

    // Conversion en binaire
    fill_table(global_huffman_tree);

    // Affichage du dictionnaire "caractères <-> codes binaires" (juste pour les tests)
    /*
    print_dictionary();
    */

    // Compression

    decompress(dst,text);
    printf("Décompression effectuée.\n");

    destroy_tree(global_huffman_tree);
    return 0;
  }

  // ------------------------------------------------------------------- //
  // ERREUR DE SYNTAXE UTILISATEUR                                       //
  // ------------------------------------------------------------------- //

  else {
    printf("L'option %s est inconnue.\nVeuillez choisir une option pour la compression (-c) ou  la décompression (-d).\n",argv[1]);
    return 0;
  }

}
