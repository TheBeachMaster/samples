#include <stdio.h>
#include <stdlib.h>
#include "string.h"

char _data [10] ;
int data_ = 254;

_data = itoa(data_,_data,10);


int main()
{
    printf("Getting started...\n");
    printf("Your int is %i ,your char is %c",data_,_data);
    return 0;
}