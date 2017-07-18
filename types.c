#include <stdio.h>
#include <stdlib.h>
#include "string.h"


int datai = 254;

char mydata [41];

char inToChar(int val)
{
    char udata [41];
    sprintf(udata,"%d",val);
    return udata;
}

int main()
{
    printf("Getting started...\n");
    sprintf(mydata,"%d",datai);
    printf("Your int is %i ,your char is %s\n",datai,mydata);
    mydata = inToChar(datai);
    printf("Your func data is %s\n",mydata);
    return 0;
}