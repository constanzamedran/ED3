/*
 * @file main.c
 * @brief  Una famosa empresa de calzados a incorporado a sus zapatillas 10 luces leds
 *comandadas por un microcontrolador LPC1769 y ha pedido a su grupo de
 *ingenieros que diseñen 2 secuencias de luces que cada cierto tiempo se vayan
 *intercalando(secuenciaA-secuenciaB-secuenciaA-...). Como todavía no se
 *ha definido la frecuencia a la cual va a funcionar el CPU del microcontrolador, las
 *funciones de retardos que se incorporen deben tener como parametros de
 *entrada variables que permitan modificar el tiempo de retardo que se vaya a
 *utilizar finalmente.Se pide escribir el código que resuelva este pedido,
 *considerando que los leds se encuentran conectados en los puertos P0,0 al P0.9
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#ifdef __USE_MCUEXPRESSO
#include <cr_section_macros.h> /* MCUXpresso-specific macros */
#endif

#include "lpc17xx_gpio.h"   /* GPIO handling */
#include "lpc17xx_pinsel.h" /* Pin function selection */


/* GPIO Direction Definitions */
#define INPUT  0
#define OUTPUT 1

/* Boolean Values */
#define TRUE  1
#define FALSE 0

#define TIEMPO_RETARDO 1000000

/**
 * @brief Initialize the GPIO peripheral
 *
 */
void configure_port(void)
{
    PINSEL_CFG_Type led_pin_cfg; /* Create a variable to store the configuration of the pin */

    /* We need to configure the struct with the desired configuration */
    led_pin_cfg.Portnum = PINSEL_PORT_0;           /* The port number is 0 */
    led_pin_cfg.Funcnum = PINSEL_FUNC_0;           /* The function number is 0 */
    led_pin_cfg.Pinmode = PINSEL_PINMODE_PULLUP;   /* The pin mode is pull-up */
    led_pin_cfg.OpenDrain = PINSEL_PINMODE_NORMAL; /* The pin is in the normal mode */

    for(uint32_t pin = 0; pin < 10; pin++){
        led_pin_cfg.Pinnum = pin; /* Change the pin number */
        PINSEL_ConfigPin(&led_pin_cfg); /* Configure the pin */
        GPIO_SetDir(PINSEL_PORT_0, (1<<pin), OUTPUT);
    }

}

void SecuenciaA(void){
    
        if (GPIO_ReadValue(PINSEL_PORT_0))
        {
            for(uint32_t pin = 0; pin < 10; pin+=2){
                GPIO_SetValue(PINSEL_PORT_0,(1<<pin));
                GPIO_ClearValue(PINSEL_PORT_0,(1<<pin+1));
            }
            retardo(TIEMPO_RETARDO);
        }
        else
        {
            for(uint32_t pin = 0; pin < 10; pin+=2){
                GPIO_ClearValue(PINSEL_PORT_0,(1<<pin));
                GPIO_SetValue(PINSEL_PORT_0,(1<<pin+1));
            }
            retardo(TIEMPO_RETARDO);
        }
        return;
}

void SecuenciaB(void){
        if(GPIO_ReadValue(PINSEL_PORT_0)){
            for(uint32_t pin = 0; pin < 10 ; pin++){
                GPIO_SetValue(PINSEL_PORT_0,(1<<pin));
            }
            retardo(TIEMPO_RETARDO);
        }
        else{
            for(uint32_t pin = 0; pin < 10 ; pin++){
                GPIO_ClearValue(PINSEL_PORT_0,(1<<pin));
            }
            retardo(TIEMPO_RETARDO);
        }
        return;
}

/**
 * @brief Main function.
 * Initializes the system and toggles the LEDS 
 */
int main(void)
{
    SystemInit(); /* Initialize the system clock (default: 100 MHz) */

    configure_port(); /* Configure GPIO pins */

    while (TRUE)
    {
        SecuenciaA();
        SecuenciaB();
    }

    return 0; /* Program should never reach this point */
}
