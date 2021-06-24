#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../list/list.h"

typedef struct Noeud Noeud ;

struct Noeud{
    int ordre;
    long long int codage;
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
    root->codage = -1;

    root->gauche = NULL;
    root->droit = NULL;
    root->parent = NULL;
}

void printNoeud(Noeud* n){
    printf("ordre %d",n->ordre );
    printf(" codage %lld\n", n->codage );
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
void tree (char* filename,char* newfilename){
    FILE *pf = fopen(filename,"r");
    if (pf==NULL){
        printf("Le fichier \"%s\" n'a pas été trouvé.\n",filename);
        exit(EXIT_FAILURE);
    }

    FILE *dst = fopen(newfilename,"w");//ajouter a la fin du ficher
    rewind(dst);

    unsigned char buf[1];
    int rd = 1;
    int count =0;
    int flag=0;

    Noeud * p;
    Noeud* tmpNyt;
    Noeud* current;

    tmpNyt=(Noeud*)malloc(sizeof(Noeud));
    current=(Noeud*)malloc(sizeof(Noeud));
    p = (Noeud*)malloc(sizeof(Noeud));

    current=root;
    while(rd>0){
      rd = fread(buf,sizeof(unsigned char),1,pf); // Lecture d'un caractère à la fois
      if (rd==0) break;

      if ((current==root)&&(((buf[0]=='a')&&current->gauche==NULL)||((buf[0]=='b')&&current->droit==NULL))){
        tmpNyt =root;
        //initialiser feuille
        count ++;
        p = (Noeud*)malloc(sizeof(Noeud));
        p->gauche=NULL;
        p->droit=NULL;
        p->parent = tmpNyt;
        p->ordre = count;

        if (buf[0]=='a'){
          tmpNyt->gauche = p;
          //char out [1];
          //out[0]='0';
          //fwrite(out,sizeof(out),1,dst);
          lenOut++;
          out=out*2+0;
        }else if (buf[0]=='b'){
          tmpNyt->droit = p;

          //char out [2];
          //out[0]='0';
          //out[1]='1';
          //fwrite(out,sizeof(out),1,dst);
          lenOut++;
          out=out*2+0;
          if (lenOut==8){
            unsigned char uc[1];
            uc[0] = (unsigned char) out;
            fwrite(uc,sizeof(uc),1,dst);
            out=0;
            lenOut=0;
          }

          lenOut++;
          out=out*2+1;
          if (lenOut==8){
            unsigned char uc[1];
            uc[0] = (unsigned char) out;
            fwrite(uc,sizeof(uc),1,dst);
            out=0;
            lenOut=0;
          }

        }else{
          //printf("%s\n","error" );
          return;
        }
        current =p;
        list_add(listeNoeuds,current,listeNoeuds->size);

        //printf("%s\n", "------------");
        //print_tree();
        //printf("%s\n", "=============");
        current = root;

        flag++;
      }else{

        if (buf[0]=='a'){
          current = current->gauche;
        }else if (buf[0]=='b'){
          current = current->droit;

        }else{
          //printf("%s\n","error" );
          return;
        }

        flag++;
        fseek(pf,flag,SEEK_SET);
        unsigned char nextBuf[1];
        int nextRd = fread(nextBuf,sizeof(unsigned char),1,pf);
        if (nextRd==0) break;

        tmpNyt= current;
        count++;
        p = (Noeud*)malloc(sizeof(Noeud));
        p->gauche=NULL;
        p->droit=NULL;
        p->parent = tmpNyt;
        p->ordre = count;

        if (nextBuf[0]=='a'&& current->gauche==NULL){
          //on cree une feuille gauche, et on revient root
          //current=current->gauche;
          flag++;
          tmpNyt->gauche = p;
          list_add(listeNoeuds,p,listeNoeuds->size);
          current = root;
          //+++++++++++++++++++++++++++++++++++++++++
          int tmpOrdre = p->ordre -1;
          int exponent = 0;
          while(tmpOrdre>0){
            //int rest = tmpOrdre % 2;
            tmpOrdre = tmpOrdre / 2;
            exponent ++;
          }

          int tmpParentOrdre = p->parent->ordre;
          char  parentCodage [exponent];
          for (int i = 0;i<exponent;i++){
             int rest = tmpParentOrdre %2;
             tmpParentOrdre = tmpParentOrdre/2;
             if (rest==0){
               parentCodage[exponent-i-1]='0';
             }else{
               parentCodage[exponent-i-1]='1';
             }
          }
          //printf("parentCodage %s\n",parentCodage );
          //printf("sub codage %s","");
          for (int i = 0;i<exponent;i++){
            //char subCodage [1];
            //subCodage[0]=parentCodage[i];
            //fwrite(subCodage,sizeof(subCodage),1,dst);
            //printf("%c",parentCodage[i] );
            lenOut++;
            if (parentCodage[i]=='0'){
              out=out*2+0;
            }else{
              out=out*2+1;
            }

            if (lenOut==8){
              unsigned char uc[1];
              uc[0] = (unsigned char) out;
              fwrite(uc,sizeof(uc),1,dst);
              out=0;
              lenOut=0;
            }
          }
          //char subCodage [1];
          //subCodage[0]='0';
          //fwrite(subCodage,sizeof(subCodage),1,dst);
          lenOut++;
          out=out*2+0;
          if (lenOut==8){
            unsigned char uc[1];
            uc[0] = (unsigned char) out;
            fwrite(uc,sizeof(uc),1,dst);
            out=0;
            lenOut=0;
          }
          //+++++++++++++++++++++++++++++++++++++++++++++

          //printf("%s\n", "------------");
          //print_tree();
          //printf("%s\n", "=============");

        }else if (nextBuf[0]=='b' && current->droit==NULL){
          //in cree une feuille droit
          //current=current->droit;
          flag++;
          tmpNyt->droit= p;
          list_add(listeNoeuds,p,listeNoeuds->size);
          current = root;

          //++++++++++++++++++++++++++++++++++
          int tmpOrdre = p->ordre -1;
          int exponent = 0;
          while(tmpOrdre>0){
            //int rest = tmpOrdre % 2;
            tmpOrdre = tmpOrdre / 2;
            exponent ++;
          }

          int tmpParentOrdre = p->parent->ordre;
          char  parentCodage [exponent];
          for (int i = 0;i<exponent;i++){
             int rest = tmpParentOrdre %2;
             if (rest==0){
               parentCodage[exponent-i-1]='0';
             }else{
               parentCodage[exponent-i-1]='1';
             }
             tmpParentOrdre = tmpParentOrdre/2;
          }
          //printf("parentCodage %s\n",parentCodage );
          //printf("sub codage %s","");
          for (int i = 0;i<exponent;i++){
            //char subCodage [1];
            //subCodage[0]=parentCodage[i];
            //fwrite(subCodage,sizeof(subCodage),1,dst);
            //printf("%c",parentCodage[i] );
            lenOut++;
            if (parentCodage[i]=='0'){
              out=out*2+0;
            }else{
              out=out*2+1;
            }

            if (lenOut==8){
              unsigned char uc[1];
              uc[0] = (unsigned char) out;
              fwrite(uc,sizeof(uc),1,dst);
              out=0;
              lenOut=0;
            }
          }
          //char subCodage [1];
          //subCodage[0]='1';
          //fwrite(subCodage,sizeof(subCodage),1,dst);
          lenOut++;
          out=out*2+1;
          if (lenOut==8){
            unsigned char uc[1];
            uc[0] = (unsigned char) out;
            fwrite(uc,sizeof(uc),1,dst);
            out=0;
            lenOut=0;
          }
          //++++++++++++++++++++++++++++++++

          //printf("%s\n", "------------");
          //print_tree();
          //printf("%s\n", "=============");

        }else{
          //c'est une node milieux,on continue
          count--;
          fseek(pf,flag,SEEK_SET);
        }

    }
  }

      //printf("out queue %d\n",out );
      //printf("lenOut queue %d\n",lenOut );
      if (lenOut>0){
        for(int i=0;i<8-lenOut;i++){
          out = out*2;
        }
      }
      unsigned char uc2[1];
      uc2[0] = (unsigned char) out;
      //printf("queue %c %d \n",uc2[0],uc2[1] );
      fwrite(uc2,sizeof(uc2),1,dst);

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
    printf("%s\n","Compression en cours.\n..." );
    tree (text,dst);
    printf("%s\n","Compression effectuée." );

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
