#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../list/list.h"

typedef struct Noeud Noeud ;

struct Noeud{
    int ordre;
    char*  codage;

    Noeud* gauche;
    Noeud* droit;
    Noeud* parent;
};

Noeud * root;
list* listeNoeuds;
int out =0;
int lenOut=0;
//initialiser root
void initialiserRoot (){
    root->ordre = 0;
    root->codage = "";

    root->gauche = NULL;
    root->droit = NULL;
    root->parent = NULL;
}

void printNoeud(Noeud* n){
    printf("ordre %d",n->ordre );
    printf(" codage %s\n", n->codage );
}
void print_tree_aux(Noeud* n,int prof,int is_right ){
    if (n==NULL){ return; }
    for (int i=0; i<prof; i++) printf("|   \t");
    if (prof==0) printf("\n-ROOT-");
    else printf("\'--%d-->",is_right);
    printNoeud(n);
    print_tree_aux(n->droit,prof+1,1);
    print_tree_aux(n->gauche,prof+1,0);
}
void print_tree(){
    print_tree_aux(root,0,0);
}

Noeud*  list_get_by_ordre(list* l, int e){
  //printf("e %c\n", e);
  if (l==NULL) return NULL;
  elem* e1 = l->first;
  //elem* e2 = l->last;sizeof(out)
  for (int i=0; i<l->size; i++){
    //printf("e1e2 %c\n",(((Noeud*)(e1->data))->lettre) );
    Noeud * n = ((Noeud*)(e1->data));
    if ((n->ordre)==e) return n;
    //if ((((Noeud*)(e2->data))->lettre)==e) return e2;
    e1 = e1->next;
    //e2 = e2->prev;
  }
  return NULL;
}

char * decompose_binary(unsigned char c){
  unsigned char x = c;
  char *bin;
  bin =malloc(sizeof(char)*8);
  for(int i=0; i<8; i++){
    if (x%2==0){
      bin[7-i]='0';
    }else{
      bin[7-i]='1';
    }
    x = x/2;
  }
  for(int i=0; i < 8; i++){
    //printf("%c",bin[i]);
  }
  //printf("\n");
  return bin;
}
void tree (char* filename,char* newfilename){
    FILE *pf = fopen(filename,"r");
    if (pf==NULL){
        printf("Le fichier \"%s\" n'a pas été trouvé.\n",filename);
        exit(EXIT_FAILURE);
    }

    FILE *dst = fopen(newfilename,"w");//ajouter a la fin du ficher
    rewind(dst);

    int rd = 1;
    int length =0;
    unsigned char buf[1];
    while(rd>0){
      rd = fread(buf,sizeof(unsigned char),1,pf);
      if (rd==0) break;
      length++;
    }
    //on recommence lire le ficher
    fseek(pf,0,SEEK_SET);
    unsigned char text[length];
    rd = fread(text,sizeof(unsigned char)*length,1,pf);

    //on creer un table qui contient toues les bin
    unsigned char bin[length*8];
    for (int i=0;i<length;i++){
      char * tab = decompose_binary(text[i]);
      for(int j=0;j<8;j++){
        bin[i*8+j]=tab[j];
      }
    }
    //printf("bin %s\n",bin );
    int index=1;

    //on creer le premier noeud
    Noeud * p;
    p = (Noeud*)malloc(sizeof(Noeud));
    p->gauche=NULL;
    p->droit=NULL;
    p->parent = root;
    p->ordre = index;
    //char out [1];
    if (bin[0]=='0'){
      root->gauche = p;
      //out[0] ='a';
      p->codage="a";
      //fwrite(out,1,1,dst);
    }else{
      root->droit = p;
      //out[0] ='b';
      p->codage="b";
      //fwrite(out,1,1,dst);
    }

    index++;
    list_add(listeNoeuds,p,listeNoeuds->size);
    //print_tree();

    //les noeus suivant
    int count =1;
    int newcount=count;
    while(count<length*8){

      int exponent =0;
      int tmpIndex = index-1 ;
      while (tmpIndex>0){
        tmpIndex = tmpIndex / 2;
        exponent++;
      }
      int nombreBin = exponent+1;
      newcount =count +nombreBin;

      int ordreParent=0;
      for (int i=count ;i<newcount-1;i++){
        if (bin[i]=='0'){
          ordreParent=ordreParent*2;
        }else{
          ordreParent=ordreParent*2+1;
        }
      }
      //printf("ordre parent %d\n", ordreParent );
      count=newcount;
      if (ordreParent==0){

        Noeud * p;
        p = (Noeud*)malloc(sizeof(Noeud));
        p->gauche=NULL;
        p->droit=NULL;
        p->parent = root;
        p->ordre = index;

        //char out [1];
        if (bin[count+1]=='0'){
          root->gauche = p;
          //out[0] ='a';
          p->codage="a";
        }else{
          root->droit = p;
          //out[0] ='b';
          p->codage="b";
        }

        //fwrite(out,sizeof(out),1,dst);
        index++;
        list_add(listeNoeuds,p,listeNoeuds->size);
        //print_tree();

      }else{
        //on a ordre parent
        //printf("%s\n","_____________________" );
        //print_tree();
        Noeud * currentParent;
        //currentParent =(Noeud*)malloc(sizeof(Noeud));
        currentParent= list_get_by_ordre(listeNoeuds,ordreParent);
        //printNoeud(currentParent);
        //on creer un current
        Noeud * current;
        current = (Noeud*)malloc(sizeof(Noeud));
        current->gauche=NULL;
        current->droit=NULL;
        current->parent = currentParent;
        current->ordre = index;
        //printNoeud(current);
        //on veut son parent codage
        char  parentCodage [exponent];
        memset(parentCodage,'\0',sizeof(parentCodage));
        strcpy(parentCodage,currentParent->codage);
        //on write son parent codage
        //fwrite(parentCodage,sizeof(parentCodage),1,dst);
        //on veut codage current
        current->codage = malloc(sizeof(nombreBin));
        char  currentCodage [nombreBin];
        memset(currentCodage,'\0',sizeof(currentCodage));
        strcat(currentCodage,parentCodage);
        if (bin[newcount-1]=='0'){
          currentParent->gauche = current;
          char out [1];
          out[0]='a';
          //fwrite(out,sizeof(out),1,dst);
          strcat(currentCodage,out);
          strcpy(current->codage , currentCodage);
        }else{
          currentParent->droit = current;
          char out [1];
          out[0]='b';
          //fwrite(out,sizeof(out),1,dst);
          strcat(currentCodage,out);
          strcpy(current->codage , currentCodage);
        }
        //printNoeud(current);

        index++;
        list_add(listeNoeuds,current,listeNoeuds->size);
        //print_tree();
      }

    }
    for(int i=0;i<index-1;i++){
      //printf("i____%d\n",i );
      Noeud * current;
      //currentParent =(Noeud*)malloc(sizeof(Noeud));
      current= list_get_by_ordre(listeNoeuds,i);

      int exponent =0;
      int tmpIndex = index-1 ;
      while (tmpIndex>0){
        tmpIndex = tmpIndex / 2;
        exponent++;
      }
      int nombreBin = exponent+1;

      char  codage [nombreBin];
      memset(codage,'\0',sizeof(codage));
      strcpy(codage ,current->codage);
      for (int j=0;j<nombreBin;j++){
        char out [1];
        out[0]=codage[j];
        if (out[0]!='\0'){
          fwrite(out,sizeof(out),1,dst);
        }

      }
      //fwrite(codage,sizeof(codage),1,dst);
    }
    //print_tree();

  fclose(dst);
  fclose(pf);

}

void print_binary2(unsigned char c){
  unsigned char x = c;
  int b[8];
  int i;
  for(i=0; i<8; i++){
    b[7-i] = x%2;
    x = x/2;
  }
  for(int i=0; i < 8; i++){
    printf("%d",b[i]);
  }
printf("\n");
}

int main(int argc, char* argv[]){
    if (argc==1){
        printf("Veuillez passer le nom du fichier à compresser en premier argument.\n");
        return 0;
    }
    if (argc==2){
        printf("Veuillez choisir un nom pour le fichier de destination, passez le en deuxième argument.\n");
        return 0;
    }

    char* text = argv[1];
    char* dst = argv[2];
    root =(Noeud*)malloc(sizeof(Noeud));
    initialiserRoot();
    listeNoeuds= new_list();
    list_add(listeNoeuds,root,listeNoeuds->size);
    printf("%s\n","Décompression en cours.\n...\n" );
    tree (text,dst);
    printf("%s\n","Décompression effectuée." );
    //print_tree();
    FILE *fdst = fopen(dst,"r");
    int rd = 1;
    char buf[1];
    //printf("_____________________\n");
    while(rd>=1){
      memset(buf,0,1);
      rd = fread(buf,sizeof(unsigned char),1,fdst);
      //print_binary2((unsigned char)buf[0]);
    }
    fclose(fdst);

    return 0;
  }
