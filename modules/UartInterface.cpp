#include "../main.h"
using namespace std;

class UartTCM2209Interface {
    public: uint rx;
    public: uint tx;
    private: uint8_t sync = 0xA0;
    private: uint8_t writeAddition = 0x80;

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
        uart_init(uart1, 9600);
        gpio_set_function(rx, GPIO_FUNC_UART);
        gpio_set_function(rx, GPIO_FUNC_UART);
    }
    public: uint8_t* read_register(uint8_t address, uint8_t registerAddress) {
        // don't except addresses over range 0...3
        if(address > 3) return {0};

        uint8_t msg[3];

        msg[0] = sync;
        msg[1] = address;
        msg[2] = registerAddress;

        uint8_t returnMessage[8];

        uart_puts(uart1, ((const char*) msg));

        uart_read_blocking(uart1, returnMessage, 8);

        return returnMessage;
        
    }
    public: void write_to_register(uint8_t address, uint8_t registerAddress, uint8_t message[4]) {
        //don't except address over range 0...3
        if(address > 3) return;
        // 3 for sync address and register. 4 for data. 1 for crc these are the three main parts of our data message to the tcm chip;
        // tcm takes 8 byte 8 bit messages over uart. the 4 byte data message can also be recognized as one 32 bit integer. and is interpretted that way.
        uint8_t msg[3 + 4 + 1];

        msg[0] = sync;
        msg [1] = address;
        msg[2] = registerAddress >> 1;

        for(int i = 0; i < 4;i++) {
            msg[i + 3] = message[i];
        }

        uart_puts(uart1, ((const char*) msg));

    }
    void swuart_calcCRC(unsigned char* datagram, unsigned char datagramLength) {
        int i,j;
        unsigned char* crc = datagram + (datagramLength-1); // CRC located in last byte of message UCHAR currentByte;
        unsigned char currentByte;

        *crc = 0;
        for (i=0; i<(datagramLength-1); i++) {
            currentByte = datagram[i];
            for (j=0; j<8; j++) {
                if ((*crc >> 7) ^ (currentByte&0x01)) {
                    *crc = (*crc << 1) ^ 0x07;
                } else {
                    *crc = (*crc << 1);
                }       
              currentByte = currentByte >> 1;
            } // for CRC bit
          } // for message byte
        };
};
