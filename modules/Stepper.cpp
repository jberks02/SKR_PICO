#include "../main.h"

#define GCONF_REG _u(0x00)
#define GSTAT_REG _u(0x01)
#define BITTIME_REG _u(0x03)

class Stepper {
    private: uint en;
    private: uint step;
    private: uint dir;
    private: uint address;
    private: float anglePerStep = 1.8;
    private: float fullStepAnglePerStep = 1.8;
    private: uint gearReduction = 1;
    private: uint stepMode = 1;
    private: float stepAngleMultiplier;
    private: float currentAngle = 0.f;
    public: bool homed = false;
    private: UartTCM2209Interface *interface;
    private: int gconf[32];
    private: int gstat[32];
    public: Stepper(uint en, uint step, uint dir, uint addresss, UartTCM2209Interface *interface) {
        //assign passed pins to local class
        this->interface = interface;
        this->en = en;
        this->step = step;
        this->dir = dir;
        this->address = address;
        // activate and set pins up to be used for stepper
        gpio_init(en);
        gpio_set_dir(en, GPIO_OUT);

        gpio_init(step);
        gpio_set_dir(step, GPIO_OUT);

        gpio_init(dir);
        gpio_set_dir(dir, GPIO_OUT);
        gpio_pull_up(dir);

       enable(false);

        setStepAngleMultiplier();

        uint8_t max_delay[4] = { 255, 255, 255, 255 };

        // interface->write_to_register(address, BITTIME_REG, max_delay);

        readAndPostResponse(gconf, GCONF_REG);
        readAndPostResponse(gstat, GSTAT_REG);
        
    }
    private: void setStepAngleMultiplier() {
        stepAngleMultiplier = anglePerStep/gearReduction;
    }
    public: void setStepMode(uint mode) {
        // use uart to set mode in tcm2209 cip
        anglePerStep = fullStepAnglePerStep/mode;
        setStepAngleMultiplier();
    }
    public: void setGearReductionResult(uint reduction) {
        gearReduction = reduction;
        setStepAngleMultiplier();
    }
    private: void enable(uint lock) {
        //high disables and low enables; due to this 
        gpio_put(this->en, !lock);
    }
    public: StepMove calculateStepAndDirectionForAngle(float newAngle, int speed = 2, bool hold = true) {
        float angleDifference;
        char newDirection;
        int steps;

        if(newAngle == currentAngle) {
            StepMove noOp(0, 'r', 2, true);
            return noOp;
        };
        if (newAngle > currentAngle) {
            newDirection = 'f';
            angleDifference = newAngle - currentAngle;
        }
        else {
            newDirection = 'r';
            angleDifference = currentAngle - newAngle;
        }

        steps = angleDifference / stepAngleMultiplier;

        currentAngle = newAngle;

        StepMove nm(newDirection, steps, speed, hold);

        return nm;
    }
    public: void move_by_step(uint steps, char direction, uint speed = 2, bool hold = false) {
       
        enable(true);

        if(direction == 'r') gpio_put(this->dir, 0);
        if(direction == 'f') gpio_put(this->dir, 1);

        for(int i = 0; i < steps; i ++) {
            uint newStepState = gpio_get(this->step) == 1 ? 0 : 1;
            gpio_put(this->step, newStepState);
            sleep_ms(speed);
        }

        if(hold == false) enable(false);
    }
    private: void readAndPostResponse(int bits[32], uint8_t reg) {

        enable(true);
        
        uint8_t *msg = interface->read_register(address, reg);

        TcmReadResponse response(msg);

        for(int i = 0; i < 32; i++) {
            bits[i] = response.dataBits[i];
        }

        enable(false);

    }
};
