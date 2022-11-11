#include "../main.h"

class Stepper {
    private: uint en;
    private: uint step;
    private: uint dir;
    public: Stepper(uint en, uint step, uint dir) {
        //assign passed pins to local class
        this->en = en;
        this->step = step;
        this->dir = dir;
        // activate and set pins up to be used for stepper
        gpio_init(en);
        gpio_set_dir(en, GPIO_OUT);

        gpio_init(step);
        gpio_set_dir(step, GPIO_OUT);

        gpio_init(dir);
        gpio_set_dir(dir, GPIO_OUT);

        gpio_put(this->en, 1);
        
    }
    private: void enable(uint lock) {
        //high disables and low enables; due to this 
        gpio_put(this->en, !lock);
    }
    public: void move_by_step(uint steps, char direction, uint speed = 2, bool hold = false) {
       
        enable(true);

        if(direction = 'r') gpio_put(this->dir, 0);
        if(direction = 'f') gpio_put(this->dir, 1);

        for(int i = 0; i < steps; i ++) {
            uint newStepState = gpio_get(this->step) == 1 ? 0 : 1;
            gpio_put(this->step, newStepState);
            sleep_ms(speed);
        }

        if(hold == false) enable(false);

    }
};
