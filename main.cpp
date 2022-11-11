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

int main() {

    stdio_init_all();

    const uint HE = 23;

    gpio_init(HE);
    gpio_set_dir(HE, GPIO_OUT);

    Stepper stepper_one(12, 11, 10);

    while (true) {
        cout << "Deadly Virus";
        gpio_put(HE, 1);
        stepper_one.move_by_step(500, 'r');
        sleep_ms(1000);
        gpio_put(HE, 0);
        stepper_one.move_by_step(500, 'f');
        sleep_ms(5000);
    }

    return 0;
}
