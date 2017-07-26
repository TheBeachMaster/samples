
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "mraa/gpio.h"

#include <stdio.h>

int running = 0;

void sig_handler(int signo)
{
	if(signo == SIGINT)
	{
		printf("Closing down\n");
		running = -1;
	}
}

int main(int argc, char** argv)
{
	mraa_platform_t platform = mraa_get_platform_type();
	mraa_gpio_context gpio,gpio_in =NULL;
	const char* board_name = mraa_get_platform_name();
	int ledstate = 0;
	mraa_result_t r = MRAA_SUCCESS;

	switch(platform)
	{
		case MRAA_INTEL_GALILEO_GEN1:
			gpio = mraa_gpio_init_raw(3);
			break;
		case MRAA_INTEL_MINNOWBOARD_MAX:
			gpio =mraa_gpio_init(21);
			break;
		case MRAA_INTEL_JOULE_EXPANSION:
			gpio = mraa_gpio_init(101);
			break;

		default:
			gpio = mraa_gpio_init(33);
	}
	
	fprintf(stdout, "LibMraa\n Version: %s\n On %s (Ctrl+C to Exit)\n",mraa_get_version(),board_name);

	if(gpio == NULL)
	{
		fprintf(stdout, "Unable to Initialize GPIO");
		return 1;
	}	

	//On Minnowboard
	if(platform == MRAA_INTEL_MINNOWBOARD_MAX)
	{
		gpio_in = mraa_gpio_init(14);
		if(gpio_in != NULL)
		{
			mraa_gpio_dir(gpio_in,MRAA_GPIO_IN);
			fprintf(stdout,"Press and Hold S1 to stop or SW1 to shutdown\n");
		}
	}

	mraa_gpio_dir(gpio,MRAA_GPIO_OUT);

	signal(SIGINT,sig_handler);

	while(running == 0)
	{
		if(gpio_in != NULL && mraa_gpio_read(gpio_in) == 0)
		{
			mraa_gpio_close(gpio_in);
			return 0;
		}
		ledstate = !ledstate;
		mraa_gpio_write(gpio,!ledstate);
		sleep(2500);
	}

	r = mraa_gpio_close(gpio);
	if(r != MRAA_SUCCESS)
	{
		mraa_result_print(r);
	}

	return r;

}

			
