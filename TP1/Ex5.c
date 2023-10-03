#include <stdio.h>

int main() {
    int a, b, temp;

    printf("Veuillez entrer deux entiers : ");
    scanf("%d %d", &a, &b);

    printf("Avant permutation : a = %d, b = %d\n", a, b);

    // Échange des valeurs en utilisant une variable temporaire
    temp = a;
    a = b;
    b = temp;

    printf("Après permutation : a = %d, b = %d\n", a, b);

    return 0;
}
