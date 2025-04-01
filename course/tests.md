# Tests proposés

Les idées et valeurs suivantes sont données à titre indicatif. Vous n'êtes pas
obligés de vous en servir, mais vous devez développer des tests de validation
de votre code quoi qu'il arrive.

*Rappel : implémentez vos tests dans le fichier src/tests.h*

### Vérification des quadratures fournies

Calculer à la main l'intégrale d'une fonction simple sur le triangle de
référence (ex : $f(\xi,\eta)=1$) et comparer à la valeur retournée par les
différentes quadratures.

### Résultats sur le triangle 4 de *square.mesh*

```
- Coordonnées des sommets :
  v0 : 0.625    1
  v1 : 0.639235 0.846329
  v2 : 0.734711 0.900627

- Mapping du point (xi=0.2 , eta=0.4) :
  0.671731 0.929517

- Matrice jacobienne au point (xi=0.2 , eta=0.4) :
   0.0142  0.11
  -0.154  -0.0994
  déterminant = 0.0155

- Ke (pour k = 1) :
   0.391  -0.164  -0.226
  -0.164   0.709  -0.545
  -0.226  -0.545   0.771

- Fe (pour f = 1) :
  0.00257
  0.00257
  0.00257

- Assemblage :
  Le numéro global du point 0 est 20.
  Le numéro global du point 1 est 49.
  Le numéro global du point 2 est 65.

```

### Résultats sur le bord 4 de *square.mesh*

```
- Coordonnées des sommets :
  v0 : 0.5    0
  v1 : 0.625  0

- Mapping du point xi=0.2 :
  0.525 0

- Matrice jacobienne au point xi=0.2 :
   0.125
   0
  déterminant = 0.125

- Fe (pour h = 1) :
  0.0625
  0.0625

- Assemblage :
  Le numéro global du point 0 est 7.
  Le numéro global du point 1 est 8.

```
