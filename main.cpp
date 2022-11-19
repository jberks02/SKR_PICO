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

    Peripherals machine;

    UartTCM2209Interface tcmCommunication;

    Stepper stepper_one(15, 14, 13, 0, &tcmCommunication);
    Stepper stepper_two(12,11,10, 1, &tcmCommunication);
    Stepper stepper_three(7,6,5, 2, &tcmCommunication);
    Stepper stepper_four(2,19,28, 3, &tcmCommunication);

    SynchronousMovement steppers(&stepper_one, &stepper_two, &stepper_three, &stepper_four, &machine);

    while (true) {
        cout << "Deadly Virus";
        gpio_put(HE, 1);
        stepper_one.move_by_step(500, 'r');
        stepper_two.move_by_step(500, 'r');
        stepper_three.move_by_step(500, 'r');
        stepper_four.move_by_step(500, 'r');
        sleep_ms(500);
        gpio_put(HE, 0);
        stepper_one.move_by_step(500, 'f');
        stepper_two.move_by_step(500, 'f');
        stepper_three.move_by_step(500, 'f');
        stepper_four.move_by_step(500, 'f');
        sleep_ms(500);
    }

    return 0;
}
