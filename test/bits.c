#include <stdio.h>

#include "../include/markdown.h"

int main(int argc, char *argv[]) {

    int i;
    printf("INIT: %i\n", i);

    SET_BIT(i, IS_HEADER);
    printf("SET_BIT(%i): %i\n", IS_HEADER, i);

    printf("CHECK_BIT(%i): %i\n", IS_HEADER, CHECK_BIT(i, IS_HEADER));

    TOGGLE_BIT(i, IS_HEADER2);
    printf("TOGGLE_BIT(%i): %i\n", IS_HEADER2, i);

    printf("CHECK_BIT(%i): %i\n", IS_HEADER2, CHECK_BIT(i, IS_HEADER2));

    TOGGLE_BIT(i, IS_HEADER2);
    printf("TOGGLE_BIT(%i): %i\n", IS_HEADER2, i);

    CLEAR_BIT(i, IS_HEADER);
    printf("CLEAR_BIT(%i): %i\n", IS_HEADER, i);

    printf("CHECK_BIT(%i): %i\n", IS_HEADER, CHECK_BIT(i, IS_HEADER));
    printf("CHECK_BIT(%i): %i\n", IS_HEADER2, CHECK_BIT(i, IS_HEADER2));
}

