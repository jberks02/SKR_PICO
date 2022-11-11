/*
 * Blink for Raspberry Pi Pico
 *
 * @version     1.0.0
 * @author     Justin Berkshire
 * @copyright   2022
 * @licence     MIT
 *
 */
#include "main.h"
using namespace std;

int main()
{
    const uint LED = PICO_DEFAULT_LED_PIN;

    gpio_init(LED);

    gpio_set_dir(LED, GPIO_OUT);

    stdio_init_all();

    int a = 4;

    int b = 8;

    int c = a + b; 

    cout << c << '\n';

    while (true)
    {
        cout << "Deadly Virus";
        gpio_put(LED, 1);
        sleep_ms(a * 100);
        gpio_put(LED, 0);
        sleep_ms(b * 100);
    }

    return 0;
}
