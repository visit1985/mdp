#include <stdio.h>

#include "../include/markdown.h"

int main(int argc, char *argv[]) {

    int i;
    printf("INIT: %i\n", i);

    SET_BIT(i, IS_H1);
    printf("SET_BIT(%i): %i\n", IS_H1, i);

    printf("CHECK_BIT(%i): %i\n", IS_H1, CHECK_BIT(i, IS_H1));

    TOGGLE_BIT(i, IS_H2);
    printf("TOGGLE_BIT(%i): %i\n", IS_H2, i);

    printf("CHECK_BIT(%i): %i\n", IS_H2, CHECK_BIT(i, IS_H2));

    TOGGLE_BIT(i, IS_H2);
    printf("TOGGLE_BIT(%i): %i\n", IS_H2, i);

    CLEAR_BIT(i, IS_H1);
    printf("CLEAR_BIT(%i): %i\n", IS_H1, i);

    printf("CHECK_BIT(%i): %i\n", IS_H1, CHECK_BIT(i, IS_H1));
    printf("CHECK_BIT(%i): %i\n", IS_H2, CHECK_BIT(i, IS_H2));
}

