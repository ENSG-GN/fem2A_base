# Code FEM2A

Télécharger et extraire **fem2a_base.zip** dans un dossier de travail.

### Organisation des dossiers et des fichiers

- main.cpp :

Il s'agit du programme principal utilisé pour lancer les [tests](tests.md)
et les [simulations](simus.md). Lisez-le pour comprendre son fonctionnement
et pour savoir le modifier à chaque nouveau test ou simulation.

- src/tests.h :

Contient la définition des différents tests qui seront appelés dans main.cpp.
C'est ici que vous écrirez les instructions permettant de tester les méthodes
que vous implémenterez au fur et à mesure de votre progression.

- src/simu.h :

Contient la définition des différentes simulations qui seront appelées dans
main.cpp. C'est ici que vous écrirez les instructions permettant de lancer les
simulations proposées.

- src/mesh.h et src/mesh.cpp :

Contiennent la classe Mesh qui permet de charger et de sauver un maillage,
d'accéder à ses composants (sommets, bords et triangles) et de connaître ou de
changer leur attribut. Contiennent également une structure *vertex* et une
fonction permettant d'écrire la solution du système linéaire. Vous n'aurez pas
besoin de modifier ces fichiers mais vous devrez sûrement y jeter un oeil pour
comprendre et utiliser ce qu'ils contiennent.

- src/solver.h et src/solver.cpp :

Contiennent une méthode *solve* qui permet de résoudre des systèmes linéaires
en appelant la librairie
[OpenNL](https://github.com/BrunoLevy/geogram/wiki/OpenNL).
Contiennent également les classes DenseMatrix et SparseMatrix que le code vous
invite à utiliser.

- src/fem.h et src/fem.cpp :

Contiennent les classes et fonctions propres à la méthode des éléments finis.
C'est ici que se fera le gros de votre travail.

La structure *Quadrature* est donnée ; elle ne nécessite aucune modification.

Les déclarations (i.e. signatures) des classes *ElementMapping* et
*ShapeFunctions* sont données ; vous devez implémenter leurs méthodes.

Les déclarations des fonctions nécessaires à la résolution du problème de
Poisson par la méthode des éléments finis sont données ; vous devez les
implémenter.

Si vous avez le temps, vous pourrez implémenter la fonction
*solve_poisson_problem* qui, en factorisant les ingrédients communs à toutes
les simulations, permet de résoudre le problème de Poisson 2D de façon très
générale.

- third_party/ :

Contient la librairie
[OpenNL](https://github.com/BrunoLevy/geogram/wiki/OpenNL)
qui permet de résoudre le système linéaire. Vous n'avez pas à y toucher.

- FEM2A.cbp :

Fichier contenant le projet Code::Blocks, environnement de développement
intégré qu'il est conseillé d'utiliser pour développer sous Windows.

- Makefile :

Il s'agit d'un fichier contenant les instructions de compilation pour la
fonction *make* sous Linux. Il n'est à modifier que si la structure des
fichiers est elle-même modifiée.

- data/ :

Contient les différents maillages que vous aurez à utiliser.

### Compilation et exécution

#### Sous Windows

Une fois extrait fem2a_base.zip dans votre dossier de travail, cliquez sur
**FEM2A.cbp** pour ouvrir le projet avec **Code::Blocks**. Si tous les fichiers
Headers et Sources ne sont pas là, faites un clic droit sur FEM2A puis cliquez
sur *Add files recursively* et sélectionnez votre dossier de travail.

Pour compiler le programme, cliquez sur *Build* (la petite roue dentelée jaune).

Pour lancer le programme, cliquez sur *Run* (la flèche verte). Une fenêtre s'ouvre alors, qui vous précise les options qu'il est possible de fournir à l'exécutable (-h, -t, -s et -v). Indiquez la ou les options qui vous intéressent dans *Project* puis *Set arguments*. Pour en savoir davantage sur ces options, regardez le main.cpp.

<!--
Pour lancer le programme, ouvrez un terminal (*cmd.exe* dans la barre d'adresse
de l'explorateur de fichier) et appelez votre exécutable :
```
cd C:\fem2a_base
..\bin\Debug\fem2a.exe -h

```
-->

#### Sous Linux

Pour compiler le programme, tapez simplement **make** en ligne de commande dans
le dossier de travail. L'exécutable **fem2a** sera alors généré dans le dossier
*build*. Il est également possible d'utiliser Code::Blocks comme sous Windows.

Pour exécuter le programme, tapez **./build/fem2a -option** en ligne de
commande.

### Visualisation des maillages et des résultats de simulation

Pour visualiser un maillage (ex : square.mesh), nous utiliserons le logiciel [Medit](https://www.ljll.fr/frey/logiciels/Docmedit.dir/index.html).

#### Sous Windows

Télécharger l'exécutable medit-win.exe et la bibliothèque glut32.dll au bas de [cette page](https://www.ljll.math.upmc.fr/frey/software.html), puis exécuter
```
medit-win.exe square.mesh
```
Si un fichier solution (extension .bb) avec le même nom se trouve dans le même
dossier que le maillage visualisé, alors on peut afficher la solution en
appuyant sur la touche **m** dans Medit.

#### Sous Linux

Cloner le [repo GitHub de Medit](https://github.com/ISCDtoolbox/Medit). Suivre le README pour l'installation. Sans doute vous faudra-t-il aussi installer la bibliothèque glut :
```
sudo apt-get install freeglut3 freeglut3-dev
sudo apt-get install binutils-gold
```
Si la compilation ne fonctionne toujours pas, changer votre CMakeLists.txt comme indiqué [ici](https://github.com/ISCDtoolbox/Medit/issues/5).

Exécution :
```
./medit square.mesh
```
Si un fichier solution (extension .bb) avec le même nom se trouve dans le même
dossier que le maillage visualisé, alors on peut afficher la solution en
appuyant sur la touche **m** dans Medit.

### Conseils

Vous pouvez modifier l'organisation des fichiers (en ajouter de nouveaux, en diviser 
d'autres, etc) mais ce n'est pas recommandé. Il est par ailleurs conseiller d'écrire les implémentations dans l'ordre suivant :

```
- Tester la structure Quadrature
- Implémenter et tester la classe ElementMapping
- Implémenter et tester la classe ShapeFunctions
- Implémenter et tester la fonction assemble_elementary_matrix()
- Implémenter et tester la fonction local_to_global_matrix()
- Implémenter et tester la fonction apply_dirichlet_boundary_conditions()
- Effectuer une simulation qui résout le problème de Dirichlet pur
- Implémenter et tester la fonction assemble_elementary_vector()
- Implémenter et tester la fonction local_to_global_vector()
- Effectuer une simulation qui résout le problème de Dirichlet avec terme source
- Effectuer une simulation qui résout le problème du sinus bump
- Implémenter et tester la fonction assemble_elementary_neumann_vector()
- Effectuer une simulation qui résout le problème de Neumann
- Effectuer des simulations sur des géométries plus complexes (mug et geothermie)
- Implémenter la fonction solve_poisson_problem()
```

