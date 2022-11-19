#include "../main.h"
using namespace std;

class TcmReadResponse {
    public: uint8_t sync;
    public: uint8_t address;
    public: uint8_t registerAddress;
    public: uint32_t data;
    public: uint8_t CRC;
    public: int dataBits[32];
    public: uint8_t allBytes[8];
    public: bitset<32> bitdata;
    public: TcmReadResponse(uint8_t *message){
        sync = message[0];
        address = message[1];
        registerAddress = message[2];
        data = message[3] | (message[4] << 8) | (message[5] << 16) | (message[6] << 24);
        CRC = message[7];
        setBitsOfData();
        for(int i = 0; i < 8; i++) {
            allBytes[i] = message[i];
        }
    }
    private: void setBitsOfData() {
        for(int k = 0; k < 32; k++) {
            int mask = 1 << k;
            int masked_data = data & mask;
            dataBits[k] = masked_data >> k;
        }
    }
};
