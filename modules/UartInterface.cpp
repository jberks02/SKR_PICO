#include "../main.h"
using namespace std;

class UartTCM2209Interface {
    public: uint rx;
    public: uint tx;

    //The slave address SLAVEADDR is selected by MS1 (bit 0) and MS2 (bit 1) in the range 0 to 3. 
    //Bit 7 of the register address identifies a Read (0) or a Write (1) access. 
    //Example: Address 0x10 is changed to 0x90 for a write access.
    public: UartTCM2209Interface(uint rx = 9, uint tx = 8) {
        this->rx = rx;
        this->tx = tx;
        // init pins for uart interface
        gpio_init(rx);
        gpio_init(tx);
        gpio_pull_up(rx);
        gpio_pull_up(tx);
        //Set up uart init
        uart_init(uart1, 115200);
        gpio_set_function(rx, GPIO_FUNC_UART);
        gpio_set_function(rx, GPIO_FUNC_UART);
    }
    public: void read_register(uint8_t *read, uint8_t *fullReturn) {

        uint8_t read_as_list[4];

        for(int i = 0;i < 4; i++) {
            read_as_list[i] = read[i];
        }
        
        uart_puts(uart1, ((const char*) read_as_list));

        uart_read_blocking(uart1, fullReturn, 4);

    }
    public: void write_to_register(unsigned char *write) {

        uart_puts(uart1, ((const char*) write));

    }
};