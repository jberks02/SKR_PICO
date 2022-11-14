#include "../main.h"

class SynchronousMovement {
    Stepper *motors[4];
    Peripherals *machine;
    public: SynchronousMovement(Stepper *motor_1,Stepper *motor_2,Stepper *motor_3,Stepper *motor_4, Peripherals *machine) { 
        this->motors[0] = motor_1;
        this->motors[1] = motor_2;
        this->motors[2] = motor_3;
        this->motors[3] = motor_4;
        this->machine = machine;
    };
    public: void checkMachineStateAndMove() {
        
        StepMove *newMovements[4];

        for(int i = 0; i < 4; i++) {
            StepMove temp = motors[i]->calculateStepAndDirectionForAngle(machine->S[i], 2, true);
            newMovements[i] = &temp;
            delete &temp;
        };

        while(newMovements[0]->step > 0 || newMovements[1]->step > 0 || newMovements[2]->step > 0 || newMovements[3]->step > 0) {
            for(int i = 0;i < 4; i++) {
                if(newMovements[i]->step > 0) {
                    motors[i]->move_by_step(1, 0, newMovements[i]->dir, true);
                    newMovements[i]->step--;
                }
            }
        }

    }
};
