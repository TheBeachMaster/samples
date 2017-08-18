#include <stdio.h>
#include <stdlib.h>
#include "string.h"


uint16_t datai = 254;

char mydata [41];



int main()
{
    printf("Getting started...\n");
    sprintf(mydata,"%d",datai);
    printf("Your int is %i ,your char is %s\n",datai,mydata);
   
    return 0;
}