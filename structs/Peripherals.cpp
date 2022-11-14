#include <stdio.h>
#include <string.h>
#include <sstream>
#include "../main.h"
using namespace std;

class Peripherals {
    public: uint L[1] = {0};
    public: uint S[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    public: float ST[4] = {0.f, 0.f, 0.f, 0.f};
    public: int stepper_speed = 2; //milliseconds between quarter step
    public: uint stepper_one_gear[2] = {1, 4};
    public: uint stepper_two_gear[2] = {1, 4};
    public: float temperature = 0.f;
    public: float touch_one = 0.f;
    public: float touch_two = 0.f;
    public: float touch_three = 0.f;
    public: float touch_four = 0.f;
    public: int endstop_one = 0;
    public: int endstop_two = 0;
    public: int endstop_three = 0;
    public: int endstop_four = 0;
    public: uint extraInputs[3] = { 0, 0 ,0 };
    public: uint extraOutPuts[2] = { 0, 0 };
    public: bool positionTransmit = true;
    public: bool sensorTransmit = true;
    public: bool home = false;
    public: bool pause_updates = false;
    public: bool updates_occurring = false;
    public: int process_command(uint8_t *command, int commandLength) {
        try {

            int lastIteration;

            while(command[0] =='\001') {
                shift_left(command, commandLength);
            }

            if(command[0] == 'L') {

                int index = set_command_index(&lastIteration, command, commandLength, 1);

                int value = set_command_value(&lastIteration, command, commandLength);

                L[index] = value;

                return 0;
            } else if(command[0] == 'S' && command[1] == ' ') {
                
                int index = set_command_index(&lastIteration, command, commandLength, 1);

                int value = set_command_value(&lastIteration, command, commandLength);

                if(index > 15) return 1; //ensure that bad servo index access is not attempted

                S[index] = value;

                return 0;

            } else if(command[0] == 'S' && command[1] == 'T') {

                int index = set_command_index(&lastIteration, command, commandLength, 2);

                int value = set_command_value(&lastIteration, command, commandLength);

                if(index > 3) return 1; //ensure that bad stepper index access is not attempted.

                ST[index] = value;

                return 0;

            } else if(command[0] == 'S' && command[1] == 'G' && command[2] == '1') {

                int index = set_command_index(&lastIteration, command, commandLength, 3);

                int value = set_command_value(&lastIteration, command, commandLength);

                if(index > 1) return 1; // ensure bad index can't be accessed for stepper gear

                stepper_one_gear[index] = value;

                return 0;
                
            } else if(command[0] == 'S' && command[1] == 'G' && command[2] == '2') {

                int index = set_command_index(&lastIteration, command, commandLength, 3);

                int value = set_command_value(&lastIteration, command, commandLength);

                if(index > 1) return 1; // ensure bad index can't be accessed for stepper gear

                stepper_two_gear[index] = value;

                return 0;
                
            } else if (command[0] == 'E' && command [1] == ' ') {
                
                int index = set_command_index(&lastIteration, command, commandLength, 1);

                int value = set_command_value(&lastIteration, command, commandLength);

                if(index > 1) return 1;

                extraOutPuts[index] = value;

                return 0;

            } else if(command[0] == 'H') {
                home = true;
            } else if(command[0] == 'P' && command[1] == 'O') {
                positionTransmit = !positionTransmit;
                return 0;
            } else if(command[0] == 'S' && command[1] == 'E') {
                sensorTransmit = !sensorTransmit;
                return 0;
            } else {
                return 1;
            }
        } catch (...) {
            return 1;
        }
        return 1;
    }
    public: string generate_new_response() {
        std::stringstream message;
        if(positionTransmit == true) {
            message << "L " << L[0] << ";S ";
            for (int i = 0; i < 15; i++) {
                message << S[i] << " ";
            }
            message << ";" << "ST ";
            for (int i = 0; i < 2; i++) {
                message << ST[i] << " ";
            }
            message << ";";
        }
        if(sensorTransmit == true) {
            message << "TEM " << temperature << ";";
            message << "T " << touch_one << " " << touch_two << " " << touch_three << " " << touch_four << ";";
            message << "EDSP " << endstop_one << " " << endstop_two << " " << endstop_three << " " << endstop_four << ";E";
            for(int i = 0; i < 3; i++) {
                message << " " << extraInputs[i];
            }
            message << ";";
        }

        return message.str();
    }
    private: int set_command_value(int *lastIndex, uint8_t *set, int setLength) {
        
        string valueString = "";

         for(int i = *lastIndex; i < setLength; i++) {
            if(set[i] == ';') break;
            valueString = valueString + (char)set[i];   
        }
        
        return atoi(valueString.c_str());

    }
    private: int set_command_index(int *lastIndex, uint8_t *set, int setLength, int startIndex) {

        string indexString = "";

        for(int i = startIndex; i < setLength; i++) {
            *lastIndex = i;
            if(set[i] == ' ' && indexString == "") continue;
            if(set[i] == ' ' && indexString != "") break;
            indexString = indexString + (char)set[i];
        }

        return atoi(indexString.c_str());

    } 
    private: void shift_left(uint8_t *list, int listLength) {
        uint8_t temp = list[0];
        for(int i = 0; i < listLength - 1; i++) {
            list[i] = list[i + 1];
        }
        list[listLength - 1] = temp;
    }
};
