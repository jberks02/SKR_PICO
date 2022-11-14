#include "../main.h"

class UartTCM2209Interface {
    public: uint rx;
    public: uint tx;
    private: uint8_t sync = 0xA0;
    private: uint8_t writeOperator = 0x10;
    //The slave address SLAVEADDR is selected by MS1 (bit 0) and MS2 (bit 1) in the range 0 to 3. 
    //Bit 7 of the register address identifies a Read (0) or a Write (1) access. 
    //Example: Address 0x10 is changed to 0x90 for a write access.
    public: UartInterface(uint rx = 9, uint tx = 8) {
        this->rx = rx;
        this->tx = tx;
        // init pins for uart interface
        gpio_init(rx);
        gpio_init(tx);
        //Set up uart init
        uart_init(uart1, 115200);
        gpio_set_function(rx, GPIO_FUNC_UART);
        gpio_set_function(rx, GPIO_FUNC_UART);
    }
    public: void read_register(uint8_t address, uint8_t registerAddress) {
        // don't except addresses over range 0...3
        if(address > 3) return;

        uint8_t msg[3];

        msg[0] = sync;
        msg[1] = address;
        msg[2] = registerAddress;

        uart_puts(uart1, ((const char*) msg));

        uint8_t returnMessage[8];

        uart_read_blocking(uart1, returnMessage, 8);

        return returnMessage;
        
    }
    public: void write_to_register(uint8_t address, uint8_t registerAddress uint8_t *message[4]) {
        //don't except address over range 0...3
        if(address > 3) return;

        uint8_t msg[3 + 4];

        msg[0] = sync;
        msg [1] = address;
        msg[2] = registerAddress >> 1;

        for(int i = 0; i < 4;i++) {
            msg[i + 3] = message[i];
        }

        uart_puts(uart1, ((const char*) msg));

    }
};
