#include "system.h"
#include "led.h"
#include "app.h"


int main(void)
{
    system_init();
    LOG_INIT(115200);
    led_init();
    LOG("Loop Start!!!\r\n");
    while (1)
    {
        app();
    }
}
