#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../list/list.h"

#define MAX_ORDRE 512
int out =0;
int lenOut=0;
typedef struct Noeud Noeud ;

struct Noeud{
    char lettre;
    int poids;
    int ordre;
    long long int codage;

    Noeud* gauche;
    Noeud* droit;
    Noeud* parent;
};

Noeud * root;
list* listeNoeuds;//une liste Noeuds
list* feuilles;//une liste feuilles
void initialiserRoot (){

    root->lettre='\0';
    root->poids = 0;
    root->ordre = MAX_ORDRE;
    root->codage = -1;

    root->gauche = NULL;
    root->droit = NULL;
    root->parent = NULL;
}
//Nyt not yet transmitted
//inlitialier un nyt
void initialiserNyt (Noeud* nyt){
    nyt->lettre='\0';
    nyt->poids = 0;
    nyt->ordre = 0;
    nyt->codage = -1;

    nyt->gauche = NULL;
    nyt->droit = NULL;
    nyt->parent = NULL;
}

void printNoeud(Noeud* n){
    printf("lettre %c ",n->lettre );
    printf("poid %d ",n->poids);
    printf("ordre %d",n->ordre );
    printf("codage %lld\n", n->codage );

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


void additionerPoidsParFeuille(Noeud* feuille){
    Noeud* tmp = feuille;
    while ( tmp != root ){
        if (tmp==NULL)break;
        (tmp->poids) +=1;
        tmp =tmp->parent;
    }//pour chaque noeud,jusqu'a root, le poids plus 1
    (root->poids)+=1;
    //printf("%s\n","additionerPoidsParFeuille" );
}

Noeud * maxNoeud(Noeud* current){
    //printf("%s\n","chercher max" );
    Noeud* max =current;
    int len = listeNoeuds->size;
    for (int j =0;j<len;j++){
        Noeud * m = (Noeud*)(list_get_by_index(listeNoeuds,j)->data);
        if (m!=root && m->poids !=0){
            if(m->poids < max->poids && m->ordre >max->ordre){
                max =m;
            }
        }
    }
    return max;
}


void echangerNoeud(Noeud* current){
    //printf("%s\n","echanger noeud" );

    Noeud* tmp = current;
    Noeud * max;
    Noeud * p  =NULL;
    Noeud *p1,*p2;
    int t;
    if (current == root  || current->poids==0){
        return;
    } else{
        while(tmp !=NULL &&tmp!=root){
            max = maxNoeud(tmp);
            if (max != tmp->parent && max->ordre > tmp->ordre){

                p1= max->parent;
                p2= tmp->parent;
                if (p1->ordre==p2->ordre){
                  t= tmp->ordre;
                  tmp->ordre = max->ordre;
                  max->ordre= t;
                  if (p1->gauche ==max ){
                      p1-> gauche = tmp;
                      p1->droit = max;
                  }else{
                    p1->droit=tmp;
                    p1->gauche = max;
                  }

                }else{
                  p=p1;
                  t= tmp->ordre;
                  tmp->ordre = max->ordre;
                  max->ordre= t;
                  tmp->parent = p1;
                  max->parent = p2;
                  if (p1->gauche ==max ){
                      p1-> gauche = tmp;
                  }else{
                      p1-> droit = tmp ;
                  }

                  if (p2->gauche == tmp){
                      p2 ->gauche = max;
                  }else{
                      p2 ->droit =max;
                  }
                }

            }else{
                p = tmp->parent;
            }
            tmp=p;
        }
    }
}



void encodage (Noeud* current,FILE *dst,int existe){
    //FILE *dst = fopen(newfilename,"a+");//ajouter a la fin du ficher
    //rewind(dst);
    Noeud* tmp ;
    Noeud* p;

    //chaque fois on remplit 8 bin, on fait fwrite, c'est ca la compression
    //si le lettre n'existait pas avant, on afficher la production comme cheminLettre,par exemple 1001b
      if (existe ==0){
        tmp =current;
        long long int code =1;
        //generer codage par inverse
        while(tmp!= root){
            p = tmp->parent;
            if (p->gauche ==tmp ){
                code = code*10;
            }else{
                code = code*10+1;
            }//a gauche, codage est 0, a droit codage est 1
            tmp =p;
        }
        current->codage= code;
        //afficher production,c'est inverse de codage
        while(code>100){
          //int quot = code / 10;
          int rest = code % 10;
          //char str[1] ;
          //if (rest==1){
          //  str[0]='1';
          //}else{
          //  str[0]='0';
          //}
          //printf("%c",str[0] );

          lenOut++;
          out = out*2+rest;
          if (lenOut==8){
            //printf("ascci %d\n",out );
            unsigned char uc[1];
            uc[0] = (unsigned char) out;
            fwrite(uc,sizeof(uc),1,dst);
            //printf("ascii %d , %c\n",out ,uc[0] );
            out=0;
            lenOut=0;
          }//si on a rempli 8 bin pour un lettre, alors on fait fwrite

          code = code /10;
        }

        //transformer une lettre en bin
        char str [8] ;
        int l = (int ) current->lettre;
        for (int i=0;i<8;i++){
          if ((l%2)==1){
            str[7-i]='1';
          }else{
            str[7-i]='0';
          }
          l = l/2;
        }

        //printf("%c\n",current->lettre);
        for (int i=0;i<8;i++){
          char substr[1];
          substr[0] = str[i];
          //fwrite(substr,sizeof(substr),1,dst);
          lenOut++;
          if (substr[0]=='0'){
            out = out*2;
          }else{
            out = out*2+1;
          }
          //out = out*2+substr;
          if (lenOut==8){
            //printf("ascci %d\n",out );
            unsigned char uc[1];
            uc[0] = (unsigned char) out;
            fwrite(uc,sizeof(uc),1,dst);
            out=0;
            lenOut=0;
          }
        }


      }else {
        //si le lettre existait avant, on afficher la production comme le chemin , par exemple 1001
        tmp =current;
        long long int code =1;
        //generer codage par inverse
        while(tmp!= root){
            p = tmp->parent;
            if (p->gauche ==tmp ){
                code = code*10;
            }else{
                code = code*10+1;
            }//a gauche, codage est 0, a droit codage est 1
            tmp =p;
        }
        current->codage= code;

        //afficher production,c'est inverse de codage
        while(code>1){

          int rest = code % 10;
          //char str[1] ;
          //if (rest==1){
          //  str[0]='1';
          // }else{
          //  str[0]='0';
          //}
          //printf("%c",str[0] );

          lenOut++;
          out = out*2+rest;
          if (lenOut==8){

            unsigned char uc[1];
            uc[0] = (unsigned char) out;
            fwrite(uc,sizeof(uc),1,dst);

            out=0;
            lenOut=0;
          }//si on a rempli 8 bin pour un lettre, alors on fait fwrite

          code = code / 10;
        }
        //printf("%s\n","" );
      }
    //fclose(dst);
}

int renouvelerPoids_aux(Noeud* n){
    if (n==NULL&&n->gauche!=NULL && n->droit!=NULL)return 0;
    if (n->gauche==NULL || n->droit==NULL) return n->poids;
    n->poids = renouvelerPoids_aux(n->gauche)+renouvelerPoids_aux(n->droit);
    return n->poids;
}
void renouvelerPoids(){
    //printf("%s\n","renouvelerPoids root" );
    root->poids=renouvelerPoids_aux (root);
}


void tree (char* filename,char* newfilename){
    FILE *pf = fopen(filename,"r");
    if (pf==NULL){
        printf("Le fichier \"%s\" n'a pas été trouvé.\n",filename);
        exit(EXIT_FAILURE);
    }

    FILE *dst = fopen(newfilename,"w");//ajouter a la fin du ficher
    rewind(dst);

    Noeud * p;
    Noeud* tmpNyt;
    Noeud* nyt;
    Noeud* current;

    tmpNyt=(Noeud*)malloc(sizeof(Noeud));
    current=(Noeud*)malloc(sizeof(Noeud));
    p = (Noeud*)malloc(sizeof(Noeud));
    nyt = (Noeud*)malloc (sizeof(Noeud));

    unsigned char buf[1];
    // Stream du fichier et construction de la forêt
    int rd = 1;

    while(rd>0){
        rd = fread(buf,sizeof(unsigned char),1,pf); // Lecture d'un caractère à la fois
        if (rd==0) break;
        int booleen = 0;
        int k=0;

        for (int j = 0;j<feuilles->size;j++){
            if (((Noeud*)list_get_by_index(feuilles,j)->data)->lettre == *buf){
                booleen =1;
                k=j;
                break;
            }
        }//verifier la liste  feuilles comtient la lettre ou pas
        //si ca contient pas la lettre, on l'a ajouter

        int existe = 0;//un booleen pour indiquer que le lettre est deja dans l'arbre ou pas
        if (booleen==0){
            if (root->poids == 0){
                tmpNyt =root;
            }
            p = (Noeud*)malloc(sizeof(Noeud));
            p->gauche=NULL;
            p->droit=NULL;
            p->parent = tmpNyt;
            p->poids=0;
            p->lettre = *buf;
            p->ordre = tmpNyt->ordre-1;

            nyt = (Noeud*)malloc (sizeof(Noeud));
            initialiserNyt(nyt);
            nyt->parent = tmpNyt;
            nyt->ordre= tmpNyt->ordre-2;

            tmpNyt->gauche = nyt;
            tmpNyt->droit= p;

            current =p;
            existe =0;//le lettre n'existait pas avant, on va compresser comme cheminLettre, par exemple 1001b pour b
            list_add(feuilles,p,feuilles->size);
            list_add(listeNoeuds,nyt,listeNoeuds->size);
            list_add(listeNoeuds,p,listeNoeuds->size);

            tmpNyt=nyt;
        }else{
            current = (Noeud*)(list_get_by_index(feuilles,k)->data);
            existe =1; //le lettre existait avant , on va compresser comme chemin, par exemple 10011 pour b
        }//si c'est existe, on mettre current egale la feuilles

        //on renouveler le poids de tous les pere
        additionerPoidsParFeuille(current);

        encodage(current,dst,existe);

        //on fait equilibrage
        echangerNoeud(current);

        //commence root , on renouveler le poid de tous les noeud
        renouvelerPoids();

        //printf("%s\n", "------------");
        //print_tree();
        //printf("%s\n", "=============================");
    }

    //On ajouter un EOF ('\n')a la fin, il va compresser dans l'arbre
    //quand on decompresse, EOF peut etre perdu qlq bite, et ca affiche pas
    //mais ca bien proteger la vraie fin du ficher
    int eof = 3;
    buf[0]=(unsigned char) eof;
    int booleen = 0;
    int k=0;
    for (int j = 0;j<feuilles->size;j++){
        if (((Noeud*)list_get_by_index(feuilles,j)->data)->lettre == *buf){
            booleen =1;
            k=j;
            break;
        }
    }//verifier la liste  feuilles comtient la lettre ou pas
    //si ca contient pas la lettre, on l'a ajouter
    int existe = 0;//un booleen pour indiquer que le lettre est deja dans l'arbre ou pas
    if (booleen==0){
        if (root->poids == 0){
            tmpNyt =root;
        }
        p = (Noeud*)malloc(sizeof(Noeud));
        p->gauche=NULL;
        p->droit=NULL;
        p->parent = tmpNyt;
        p->poids=0;
        p->lettre = *buf;
        p->ordre = tmpNyt->ordre-1;

        nyt = (Noeud*)malloc (sizeof(Noeud));
        initialiserNyt(nyt);
        nyt->parent = tmpNyt;
        nyt->ordre= tmpNyt->ordre-2;

        tmpNyt->gauche = nyt;
        tmpNyt->droit= p;
        current =p;
        existe =0;//le lettre n'existait pas avant, on va compresser comme cheminLettre, par exemple 1001b pour b

        list_add(feuilles,p,feuilles->size);
        list_add(listeNoeuds,nyt,listeNoeuds->size);
        list_add(listeNoeuds,p,listeNoeuds->size);

        tmpNyt=nyt;
    }else{
        current = (Noeud*)(list_get_by_index(feuilles,k)->data);
        existe =1; //le lettre existait avant , on va compresser comme chemin, par exemple 10011 pour b
    }//si c'est existe, on mettre current egale la feuilles

    //on renouveler le poids de tous les pere
    additionerPoidsParFeuille(current);

    //afficher la production
    encodage(current,dst,existe);

    //on fait equilibrage
    echangerNoeud(current);

    //commence root , on renouveler le poid de tous les noeud
    renouvelerPoids();



    //printf("%s\n", "------------");
    //print_tree();
    //printf("%s\n", "=============================");

    fclose(pf);
    fclose(dst);

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
    feuilles=new_list();
    printf("%s\n","Compression en cours.\n..." );
    tree (text,dst);
    printf("%s\n","Compression effectuée." );
    //compresser(text,dst);

    // Lecture du fichier compressé : juste pour les tests (verifier le texte attendu)
    /*
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
    */
    return 0;
}
