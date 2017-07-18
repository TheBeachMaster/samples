#include <stdio.h>
#include <unistd.h>
#include "mraa/aio.h"
#include <signal.h>

int running = 0;

void sig_handler(int signo)
{
if(signo == SIGINT){
printf("Gracefully shutting Down\n");
running =-1;
	}
}

int main()
{
mraa_aio_context adc_a0;
int adc_value = 0;
float adc_value_float =0.0;
mraa_result_t r = MRAA_SUCCESS;

adc_a0 = mraa_aio_init(0);
if(adc_a0 == NULL){
return 1;
}

signal(SIGINT,sig_handler);

while(running == 0){
adc_value = mraa_aio_read(adc_a0);
adc_value_float = mraa_aio_read_float(adc_a0);
fprintf(stdout, "ADC A0 read %X - %i\n",adc_value,adc_value);
fprintf(stdout, "ADC A0 read float - %.5f (Ctrl+C to exit)\n",adc_value_float);
usleep(1500000);
}
r =  mraa_aio_close(adc_a0);
if(r != MRAA_SUCCESS){
mraa_result_print(r);
}
return r;

}
