#include <stdio.h>
#include <mraa.h>
#include <mraa/gpio.h>


	mraa_init();
	mraa_gpio_context m_gpio;
	m_gpio = mraa_gpio_init(5);
	mraa_gpio_dir(m_gpio, MRAA_GPIO_OUT);
	
int main()
{

	int i;
	for (i=0;i<10;i++)
	{
		mraa_gpio_write(m_gpio,1);
		sleep(1);

		mraa_gpio_write(m_gpio,0);
		sleep(1);
	}

	mraa_gpio_close(m_gpio);
	return(0);
}
