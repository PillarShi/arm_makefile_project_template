#include "app.h"
#include "led.h"
#include "system.h"

void app()
{
    while (1)
    {
        delay_ms(500);
        led_toggle(LED1);
        LOG("...led toggle\r\n");
    }
}
