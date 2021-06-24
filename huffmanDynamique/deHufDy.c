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



void encodage (Noeud* current,char* newfilename,int existe){
    FILE *dst = fopen(newfilename,"a+");//ajouter a la fin du ficher
    rewind(dst);
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
          int quot = code / 10;
          int rest = code % 10;
          /*
          char str[1] ;
          if (rest==1){
            str[0]='1';
          }else{
            str[0]='0';
          }
          printf("%c",str[0] );
          */

          lenOut++;
          out = out*2+rest;
          if (lenOut==8){
            //printf("ascci %d\n",out );
            unsigned char uc[1];
            uc[0] = (unsigned char) out;
            fwrite(uc,sizeof(uc),1,dst);
            //printf("ascii %c\n",uc[0] );
            out=0;
            lenOut=0;
          }//si on a rempli 8 bin pour un lettre, alors on fait fwrite

          code = quot;
        }

        //transformer une lettre en bin
        char str [8] ;
        int l = current->lettre;
        for (int i=0;i<8;i++){
          if ((l%2)==1){
            str[7-i]='1';
          }else{
            str[7-i]='0';
          }
          l = l/2;
        }
        //fwrite(str,sizeof(str),1,dst);
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
            //printf("ascii %c\n",uc[0] );
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
          int quot = code / 10;
          int rest = code % 10;
          /*
          char str[1] ;
          if (rest==1){
            str[0]='1';
          }else{
            str[0]='0';
          }
          printf("%c",str[0] );
          */

          lenOut++;
          out = out*2+rest;
          if (lenOut==8){
            //printf("ascci %d\n",out );
            unsigned char uc[1];
            uc[0] = (unsigned char) out;
            fwrite(uc,sizeof(uc),1,dst);

            out=0;
            lenOut=0;
          }//si on a rempli 8 bin pour un lettre, alors on fait fwrite

          code = quot;
        }
        //printf("%s\n","" );
      }
    fclose(dst);
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

char * print_binary2(unsigned char c){
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
    int flag = 0;
    current=root;
    while(rd>0){
        rd = fread(buf,sizeof(unsigned char),1,pf); // Lecture d'un caractère à la fois
        if (rd==0) break;

        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        if (flag ==0){
          tmpNyt =root;

          //initialiser feuille
          p = (Noeud*)malloc(sizeof(Noeud));
          p->gauche=NULL;
          p->droit=NULL;
          p->parent = tmpNyt;
          p->poids=0;
          p->lettre = *buf;
          p->ordre = tmpNyt->ordre-1;

          //initialiserNyt
          nyt = (Noeud*)malloc (sizeof(Noeud));
          initialiserNyt(nyt);
          nyt->ordre= tmpNyt->ordre-2;
          nyt->parent = tmpNyt;

          //renouveler gauche et droit de tmpNyt
          tmpNyt->gauche = nyt;
          tmpNyt->droit= p;

          //existe =0;//le lettre n'existait pas avant, on va compresser comme cheminLettre, par exemple 1001b pour b
          //renouveler la liste feuille et liste noeud (tree)
          current =p;
          list_add(feuilles,p,feuilles->size);
          list_add(listeNoeuds,nyt,listeNoeuds->size);
          list_add(listeNoeuds,p,listeNoeuds->size);

          tmpNyt=nyt;

          //printf("%s\n",buf );
          fwrite(buf,sizeof(buf),1,dst);
          additionerPoidsParFeuille(current);
          //commence root , on renouveler le poid de tous les noeud

          //on fait equilibrage
          echangerNoeud(current);
          renouvelerPoids();

          //printf("%s\n", "------------");
          //print_tree();
          //printf("%s\n", "=============");
          current = root;

          flag++;
        }else{
          unsigned char lettre_compresse = buf[0];
          char *bin =malloc(sizeof(char)*8);
          bin = print_binary2(lettre_compresse);


          for(int i=0; i<8;i++){

            if (bin[i]=='0'){
              current = current->gauche;
            }else{
              current = current->droit;
            }

            if ((current->codage) == -1){
              if (current->gauche == NULL){
                //alors c'est un nyt
                //on prends les 8 bin suivant,
                //transformer en lettre
                // a faire
                //fseek avec flag  pour prendre suivant lettre

                //construre next fauille
                flag++;
                fseek(pf,flag,SEEK_SET);
                unsigned char nextBuf[1];
                int nextRd = fread(nextBuf,sizeof(unsigned char),1,pf);
                if (nextRd==0) break;
                char *nextBin =malloc(sizeof(char)*8);
                nextBin = print_binary2(nextBuf[0]);
                //printf("next bin %s\n", nextBin );
                int nextFeuille = 0;
                for (int j=i+1;j<8; j++){
                  if(bin[j]=='0'){
                    nextFeuille = nextFeuille*2+0;
                  }else{
                    nextFeuille = nextFeuille*2+1;
                  }
                }
                for (int k=0;k<i+1;k++){
                  if(nextBin[k]=='0'){
                    nextFeuille = nextFeuille*2+0;
                  }else{
                    nextFeuille = nextFeuille*2+1;
                  }
                }
                //il y qlq bin qui rest dans nextBuf, a faire

                //on a lettre, inserer le lettre
                tmpNyt= current;
                //initialiser feuille
                p = (Noeud*)malloc(sizeof(Noeud));
                p->gauche=NULL;
                p->droit=NULL;
                p->parent = tmpNyt;
                p->poids=0;
                p->lettre = (char ) nextFeuille;
                //printf(" nextFeuille lettre %d\n",nextFeuille );
                p->ordre = tmpNyt->ordre-1;

                //initialiserNyt
                nyt = (Noeud*)malloc (sizeof(Noeud));
                initialiserNyt(nyt);
                nyt->ordre= tmpNyt->ordre-2;
                nyt->parent = tmpNyt;

                //renouveler gauche et droit de tmpNyt
                tmpNyt->gauche = nyt;
                tmpNyt->droit= p;

                //renouveler la liste feuille et liste noeud (tree)
                current =p;
                list_add(feuilles,p,feuilles->size);
                list_add(listeNoeuds,nyt,listeNoeuds->size);
                list_add(listeNoeuds,p,listeNoeuds->size);

                tmpNyt=nyt;

                //printf("%c\n", current->lettre );
                char str[1];
                str[0]=current->lettre;
                fwrite(str,sizeof(str),1,dst);
                //on a ajoute la feilles;

                //on renouveler le poids de tous les pere
                additionerPoidsParFeuille(current);

                //on fait equilibrage
                echangerNoeud(current);

                //commence root , on renouveler le poid de tous les noeud
                renouvelerPoids();

                //printf("%s\n", "------------");
                //print_tree();
                //printf("%s\n", "=============");

                //revient
                current =root;
                //printf("bin rest%s","" );
                for (int l = i+1 ; l<8;l++){
                  bin[l]=nextBin[l];
                  //printf("%c",bin[l] );
                }
                //printf("%s\n","" );


                if (i==7){
                  flag++;
                }
              }else{
                //alors c'est un node , on continue
                if (i==7){
                  flag++;
                }
              }
            }else{
              if (current->gauche ==NULL){
                //alors c'est une fauille
                //on s'arret ici , met poid ++
                //et on revient root

                if (i==7){
                  flag++;
                }
                //printf("%c\n",current->lettre );
                char str [1];
                str[0]=current->lettre;
                fwrite(str,sizeof(str),1,dst);
                //current->poids ++;
                additionerPoidsParFeuille(current);

                //on fait equilibrage
                echangerNoeud(current);
                renouvelerPoids();

                //printf("%s\n", "------------");
                //print_tree();
                //printf("%s\n", "=============");

                current = root;
              }
            }
          }
        }//fermer for
      }//fermer while
    fclose(pf);

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
    printf("%s\n","Décompression en cours.\n...\n" );
    tree (text,dst);
    printf("%s\n","Décompression effectuée." );
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
