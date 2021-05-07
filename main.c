#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "Text_Index_App.c"


int main()
{
    t_Index *index;
    index = creer_index();
    menuPrincipal(index);

    return 0;
}
