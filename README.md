# Algorithmes de compression de fichiers   

<img alt="C" src="https://img.shields.io/badge/programming_language%20-%2300599C.svg?&style=flat-square&logo=c&logoColor=white"/>    

*Projet réalisé en binôme dans le cadre du cours "Projet Math-Info"*   
**Auteurs : [Joël Hamilcaro](https://github.com/Joel-Hamilcaro/) et [Jie Tu](https://github.com/jie-tu)**

Le but de se projet était de comprendre les concepts mathématiques derrière trois algorithmes de compression de fichiers,
puis, d'implémenter des compresseurs de fichiers textuels utilisant ces algorithmes.  Il s'agissait ici de montrer
que nous avions compris le fonctionnement des algorithmes et le principe de leur implémentation pour des cas simples.

## Résumé

Dans le cadre de notre projet de L3 Maths-Info, nous avons
développé trois compresseurs de fichiers. Pour chacun des pro-
grammes, l’algorithme de compression utilisé est différent. Ils
sont basés respectivement sur, l’algorithme de Huffman statique,
l’algorithme de Huffman dynamique, et, l’algorithme de Ziv-
Lempel. Ce document est le rapport de ce projet. Il a pour but
de présenter les fonctionnalités qui ont été implémentés, d’expliquer nos choix et l’approche utilisée, et
enfin, de comparer les différents algorithmes d’un point de vue
pratique.

## 1 – Fonctionnalités du projet

### 1.1 – Implémentions effectuées
Tout d’abord, nous avons implémenté tous les algorithmes de compression proposés par l'énoncé :
— L’algorithme de Huffman statique.
— L’algorithme de Huffman dynamique.
— L’algorithme de Ziv-Lempel.
Nous avons, en plus de cela, fait les algorithmes de décompression par la mé-
thode de Huffman statique et la méthode de Huffman Dynamique. Nos programmes sont donc capables de compresser et
décompresser un fichier texte par la méthode de Huffman Statique et Huffman
Dynamique, et seulement compresser un fichier par l’algorithme de Ziv-
Lempel.

### 1.2 – Limitations

Notre programme de compression-décompression basé sur l’algorithme
de Huffman Statique est capable de compresser et décompresser tout fichier
texte. Pour l’algorithme de Huffman Dynamique, nous nous limitons
à un alphabet de taille fixe (les 128 premiers caractères ASCII), pour nous
faciliter l’implémentation de certains algorithmes. Quand au compresseur Ziv-Lempel,
il fonctionne sur des fichiers textuels constitués des deux symboles
'a' et 'b' uniquement (exemple de fichier fourni : 01-original.txt).
Il nous était demandé d'implémenter une version simplifiée d'un compresseur Ziv-Lempel.
(e.g. L'encodage du EOF (end of file) n'était pas demandé). Il s'agissait ici de montrer
que nous avions compris le fonctionnement de l'algorithme et comment l'implémenter.

## 2 – Choix techniques

### 2.1 – Langage de programmation

Nous avons choisi d’utiliser le langage C pour l’implémentation de nos
programmes, à la fois, par goût personnel, parce que la manipulation di-
recte de fichiers dans ce langage est assez simple, et pour avoir une gestion
manuelle, plus fine, de la mémoire.

### 2.2 – Structure des programmes

Chaque compresseur-décompresseur de fichiers fonctionne de manière
indépendante. Chacun possède son propre exécutable, chaque programme
possède ses propres fonctions/modules, sauf pour la bibliothèque de listes
qu’ils se partagent. Un makefile est fourni pour effectuer la compilation des
programmes (Les commandes de compilation et d’exécution sont précisées
dans le fichier README.md.)

### 2.3 – Bibliothèques utilisées

Dans nos algorithmes, nous avions souvent besoin d’utiliser des struc-
tures de listes. Nous avons donc implémenté une bibliothèque de liste
doublement-chaînée en langage C. Cette bibliothèque était adaptée à nos
besoins. Les trois programmes de compression et décompression utilisent3
cette bibliothèque. Nous n’avons importé aucune bibliothèque externe pour
ce projet.

### 2.4 – Principe d’exécution du programme
Nous avons décidé que l’exécution d’une compression et d’une décom-
pression se ferait de manière similaire à une commande du shell. On doit
spécifier la commande, avec, en arguments, le nom du fichier source et le
nom du nouveau fichier. (Les commandes exactes sont précisées dans le
fichier README.md)

### 2.5 – Encodage de l’en-tête pour l’algorithme de Huffman statique

Dans l’algorithme de Huffman statique, la conversion de chaque carac-
tère dépend de leur fréquence d’apparition dans le texte. Ainsi, nous avions
besoin de stocker dans le fichier compressé, un préambule contenant la des-
cription de la fonction de décompression. Pour cela, nous avions plusieurs
choix possibles :
— Stocker le dictionnaire, c’est-à-dire, la liste des caractères et le code
binaire correspondant.
— Stocker l’arbre de Huffman.
Nous avons choisi ce dernier choix, afin de gagner de la place dans le fichier
compressé. Pour encoder notre arbre dans le préambule, on utilise la re-
présentation succincte des arbres binaires, basée sur le parcours préfixe de
notre arbre de Huffman :
— On effectue un parcours préfixe.
— À chaque noeud qui n’est pas une feuille, on ajoute un 0.
— À chaque feuille (qui représente donc une lettre dans l’arbre de Huff-
man), on ajoute un 1, et la lettre correspondante.
Ainsi, nous avons un en-tête de fichier qui décrit, de manière concise, notre
arbre de Huffman.

### 2.6 – Encodage de fin de fichier

Les fichiers compressés, ont une taille correspondant à un nombre entier
d’octets (autrement dit, un multiple de 8 bits). Or, si la conversion lettre par
lettre de notre texte, aboutit à un nombre de bits qui n’est pas un multiple
de 8, il reste des bits ambigüs en fin de fichier. Pour résoudre ce problème,
nous ajoutons en fin de texte un caractère EOT (end of text).4

## 3 – Analyse pratique des algorithmes

Voici le resultats de quelques mesures de nos algorithmes sur des fi-
chiers de taille différentes. Le compresseur-decompresseur de fichiers par
Ziv-Lempel n’étant pas complet au moment de la rédaction de ce rapport,
ces mesures ont été faîtes sur les algorithmes de Huffman statique et dynamique.
| Fichier original  | Compressé (Huffman statique)          | Compressé (Huffman dynamique) |
| :---------------: |:---------------:|:-----:|
|  12o  |   26o        |  10o |
| 304o | 330o             |   241o |
|3,3ko  | 2.1ko          | 2.0ko |
| 43,3ko | 25.8ko | 25.8ko |
| 259,5ko | 139,2ko | 139,2ko |

On peut voir, que pour des fichiers très légers, le taux de compression est
plus fort pour l’algorithme de Huffman Dynamique. En effet, pour Huffman
Statique, la taille de l’en-tête n’est pas négligeable pour des fichiers trop lé-
gers (le fichier compressé peut être plus lourd que l’original pour des fichier
extrêmement légers, à cause de l’en-tête). Pour des fichiers plus lourds, le
taux de compression est similaire, si l’on compare les deux algorithmes.

## 4 – Bilan et pistes d’améliorations

Pour réaliser ce projet, nous devions nous familiariser avec trois algorithmes de compression de fichiers.
Nous avions dû étudier et comprendre
ces algorithmes, à la fois dans leurs aspects techniques et mathématiques.
Grâce aux choix techniques que nous avons décrit dans ce rapport, nous
avons pu développé des programmes de compression de fichiers qui sont
concrètement utilisables pour compresser des fichiers texte. Des pistes d’extensions à ce projet seraient envisagables.
Une première piste d’amélioration serait l’extension de l’algorithme de Huff-
man Dynamique à tous les caractères ASCII. Une autre extension possible
serait un affichage détaillé en cours d’exécution du programme : comme
la durée de la compression, le taux de compression par rapport au fichier
original, et pourquoi pas, via un affichage graphique décrivant l’évolution
du déroulement du programme.

## Annexe : Commandes pour démarrer le projet

### Huffman statique

#### Compiler

```
make
```

ou

```
make huffman
```

#### Exécuter

##### Compression

```
./huffman -c 00-original.txt 00-compressed
```

##### Decompression

```
./huffman -d 00-compressed 00-decompressed.txt
```

### Huffman dynamique

#### Compiler

Compresseur :
```
make hufDy
```

Décompresseur :
```
make deHufDy
```

#### Exécuter

##### Compression
```
./hufDy 00-original.txt 00-compressed
```

##### Decompression
```
./deHufDy 00-compressed 00-decompressed.txt
```

### Ziv-Lempel

#### Remarque

Cette version fonctionne avec des fichiers textes constitués des deux symboles
'a' et 'b' uniquement (exemple de fichier fourni : 01-original.txt).
Il nous était demandé d'implémenter une version simplifiée d'un compresseur Ziv-Lempel.
(e.g. L'encodage du EOF (end of file) n'était pas demandé). Il s'agissait ici de montrer
que nous avions compris le fonctionnement de l'algorithme et comment l'implémenter.


#### Compiler

Compresseur :
```
make newZl
```

Décompresseur :
```
make deZl
```

#### Exécuter

##### Compression

```
./newZl 01-original.txt 01-compressed
```

##### Décompression

```
./deZl 01-compressed 01-decompressed.txt
```

### Nettoyer les fichiers .o
```
make clean
```
