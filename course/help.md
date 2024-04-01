# Aide

#### Quand vous avez une erreur de compilation

- Regardez dans votre code si vous pouvez la trouver facilement.
- Copier-coller le message d'erreur dans un moteur de recherche. Les
  descriptions/solutions les plus rapides à comprendre se trouvent souvent sur
  le site *Stack Overflow*.

#### Quand vous avez une erreur d'exécution

- Ajouter des *std::cout* ou des *printf* pour localiser le problème.
- En cas de *segmentation fault*, il y a de grandes chances que vous essayiez
d'accéder à un élément en dehors des bornes d'un tableau.

#### Les pointeurs de fonction

- Exemple :
> ```
> #include <math.h>
> #include <stdio.h>
> 
> // Function taking a function pointer as an argument
> double compute_sum(double (*funcp)(double), double lo, double hi) {
> double sum = 0.0;
>     // Add values returned by the pointed-to function '*funcp'
>     int i;
>     for(i = 0;  i <= 100;  i++) {
>         // Use the function pointer 'funcp' to invoke the function
>         double x = i / 100.0 * (hi - lo) + lo;
>         double y = funcp(x);
>         sum += y;
>     }
>     return sum / 101.0;
> }
> 
> double square(double x) {
>     return x * x;
> }
> 
> int main(void) {
>     double  sum;
>     // Use user-defined function 'square()' as the pointed-to function
>     sum = compute_sum(square, 0.0, 1.0)
>     printf("sum(square): %g\n", sum);
>     return 0;
> }
> 
> ```

- sur [wikipedia](https://en.wikipedia.org/wiki/Function_pointer)

- sur le [site de Christian Casteyde](http://casteyde.christian.free.fr/cpp/cours/online/x2102.html)

#### Liens utiles

- [Common C++ compiler and linker errors](https://www.csee.umbc.edu/courses/undergraduate/202/fall04/Projects/CommonErrors.shtml)

- Cours de P. Collon sur [ARCHE](https://arche.univ-lorraine.fr/course/view.php?id=61374)
