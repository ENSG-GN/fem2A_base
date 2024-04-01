# Travail à rendre

Individuellement, vous rendrez **sur ARCHE avant le 15 mai à minuit** :

- le code (src/, third_party/, main.cpp, FEM2A.cbp et Makefile) complété par
vos soins. Référez-vous aux conseils donnés dans [Code FEM2A](code.md) pour
davantage de détails sur les implémentations demandées et la progression à
suivre.

- le dossier data contenant les maillages ainsi que le résultat de vos
simulations (.mesh et .bb) que vous placerez dans output/.

- un fichier au format pdf détaillant les tests et les simulations que vous
avez mis en oeuvre. Vous prendrez soin d'analyser et de commenter vos résultats
(valeurs obtenues, cohérence avec la physique, qualité de la solution calculée,
etc).

- Un petit fichier README.txt (de 20 à 30 lignes) expliquant comment compiler
votre code et lancer l'exécutable, et détaillant toutes les options possibles
pour permettre de reproduire vos tests et vos simulations.

Vous mettrez tous ces éléments **dans un seul et unique dossier que vous nommerez
*fem2a_nom*** et que vous compresserez en .zip avant de le déposer sur
ARCHE.

# Évaluation

Les deux compétences évaluées sont données ci-dessous. Les notes (A, B, C, D, E, Fx et F)
qui indiqueront votre niveau d'apprentissage sont données à titre indicatif ;
elles pourront être ajustées en fonction de la progression de l'ensemble du
groupe.

- AF1 : Être capable d'écrire un programme informatique implémentant une méthode d'éléments finis pour la résolution d'un problème aux limites

```
A : L'implémentation fonctionne en gérant le terme de Dirichlet, le terme source et le terme de Neumann. Elle est appliquée à une géométrie complexe (mug et/ou géothermie) et des fonctionnalités étendues sont proposées (e.g. interpolation de degré supérieur).
B : L'implémentation fonctionne en gérant le terme de Dirichlet, le terme source et le terme de Neumann. Elle est appliquée à une géométrie complexe (mug et/ou géothermie).
C : L'implémentation fonctionne en gérant le terme de Dirichlet, le terme source et le terme de Neumann.
D : L'implémentation fonctionne en gérant le terme de Dirichlet et le terme source.
E : L'implémentation fonctionne en gérant le terme de Dirichlet.

```
    
- AF2 : Être capable de valider un programme de simulation numérique et d'en analyser les résultats

```
A : Les fonctions implémentées sont validées par des tests et au moins quatre simulations sont proprement analysées. Une étude quantitative de l'erreur d'approximation en fonction de la résolution du maillage et/ou du degré d'interpolation est proposée en utilisant une solution analytique connue.
B : Les fonctions implémentées sont validées par des tests et au moins quatre simulations sont proprement analysées. Une étude qualitative de l'erreur d'approximation en fonction de la résolution du maillage et/ou du degré d'interpolation est proposée en utilisant une solution analytique connue.
C : Les fonctions implémentées sont validées par des tests et trois simulations sont proprement analysées. Une étude qualitative de l'erreur d'approximation en fonction de la résolution du maillage et/ou du degré d'interpolation est proposée en utilisant une solution analytique connue.
D : Les fonctions implémentées sont validées par des tests et deux simulations sont proprement analysées.
E : Les fonctions implémentées sont validées par des tests et une simulation est proprement analysée. 

```

