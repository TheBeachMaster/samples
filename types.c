#include <stdio.h>
#include <stdlib.h>
#include "string.h"


int datai = 254;

char mydata = (char *)datai;


int main()
{
    printf("Getting started...\n");
    printf("Your int is %i ,your char is %c",datai,mydata);
    return 0;
}