#include "../main.h"
using namespace std;

class tmc_register_base {

    private: uint8_t sync = 0b1010000;

    protected: void constructReadChar(uint8_t reg, unsigned char *datagram) {

        datagram[0] = ((unsigned char)sync);

        datagram[1] = ((unsigned char)0);

        datagram[2] = ((unsigned char)reg);

        swuart_calcCRC(datagram, 4);

    }
    protected: void constructWriteChar(uint8_t reg, unsigned char *datagram, uint8_t *data) {

        datagram[0] = ((unsigned char) sync);

        datagram[1] = ((unsigned char) 0);

        datagram[2] = ((unsigned char)reg >> 1);

        byteswap(data);

        for(int i = 0; i < 4; i++) {
            datagram[i + 3] = ((unsigned char) data[i]);
        }

        swuart_calcCRC(datagram, 8);

    }
    protected: void byteswap (uint8_t *data) {
        uint8_t tmp;

        tmp = data[0];
        data[0] = data[3];
        data[3] = tmp;
        tmp = data[1];
        data[1] = data[2];
        data[2] = tmp;
    }
    void swuart_calcCRC(unsigned char* datagram, int datagramLength) {
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
    protected: void setWrite(uint32_t data, unsigned char *write, uint8_t reg) {
        
        uint8_t dataByteArray[4] = {0,0,0,0};

        memcpy(dataByteArray, &data, 4);

        constructWriteChar(reg, write, dataByteArray);
        
    }
};

class gconf_maintainer: protected tmc_register_base {
    public: bitset<32> bitdata;
    private: uint8_t reg = 0x00;
    private: bool i_scale = false;
    private: bool internal_rSense = false;
    private: bool spreadCycle = false;
    private: bool shaft = true;
    private: bool index_otpw = false;
    private: bool index_step = false;
    private: bool pdn_disable = false; // set to true for using uart interface
    private: bool step_reg_select = false;// set to true to set steps over uart
    private: bool multistep_filter = false;// leave zero don't need to filter step pulses
    private: bool test_mode = false;// ensure this is set to zero
    public: unsigned char read[4];
    public: unsigned char write[8];
    public: gconf_maintainer() {
        constructReadChar(reg, read);
    }
    public: void set_i_scale(bool set) {
        i_scale = set;
        bitdata[0] = set;
        uint32_t data = bitdata.to_ulong();
        setWrite(data, write, reg);
    }
    public: bool get_i_scale() {
        return i_scale;
    }
    public: void set_internal_rSense(bool set) {
        internal_rSense = set;
        bitdata[1] = set;
        uint32_t data = bitdata.to_ulong();
        setWrite(data, write, reg);
    }
    public: bool get_internal_rSense() {
        return internal_rSense;
    }
    public: void set_spreadCycle(bool set) {
        spreadCycle = set;
        bitdata[2] = set;
        uint32_t data = bitdata.to_ulong();
        setWrite(data, write, reg);
    }
    public: bool get_spreadCycle() {
        return spreadCycle;
    }
    public: void set_shaft(bool set) {
        shaft = set;
        bitdata[3] = set;
        uint32_t data = bitdata.to_ulong();
        setWrite(data, write, reg);
    }
    public: bool get_shaft() {
        return shaft;
    }
    public: void set_index_otpw(bool set) {
        index_otpw = set;
        bitdata[4] = set;
        uint32_t data = bitdata.to_ulong();
        setWrite(data, write, reg);
    }
    public: bool get_index_otpw() {
        return index_otpw;
    }
    public: void set_index_step(bool set) {
        index_step = set;
        bitdata[5] = set;
        uint32_t data = bitdata.to_ulong();
        setWrite(data, write, reg);
    }
    public: bool get_index_step() {
        return index_step;
    }
    public: void set_pdn_disable(bool set) {
        pdn_disable = set;
        bitdata[6] = set;
        uint32_t data = bitdata.to_ulong();
        setWrite(data, write, reg);
    }
    public: bool get_pdn_disable() {
        return pdn_disable;
    }
    public: void set_step_reg_select(bool set) {
        step_reg_select = set;
        bitdata[7] = set;
        uint32_t data = bitdata.to_ulong();
        setWrite(data, write, reg);
    }
    public: bool get_step_reg_select() {
        return step_reg_select;
    }
    public: void set_multistep_filter(bool set) {
        multistep_filter = set;
        bitdata[8] = set;
        uint32_t data = bitdata.to_ulong();
        setWrite(data, write, reg);
    }
    public: bool get_multistep_filter() {
        return multistep_filter;
    }
    public: void set_test_mode(bool set) {
        test_mode = set;
        bitdata[9] = set;
        uint32_t data = bitdata.to_ulong();
        setWrite(data, write, reg);
    }
    public: bool get_test_mode() {
        return test_mode;
    }
};