#include "../main.h"
using namespace std;

class tmc_register_base {

    private: uint8_t sync = 0b1010000;

    protected: UartTCM2209Interface interface = UartTCM2209Interface();

    protected: void consumeDataMessage(uint8_t *dataMessage, bitset<32> existing_bitset, int bitLength) {
        
        uint32_t dataFullUint =  dataMessage[6] | (dataMessage[5] << 8) | (dataMessage[4] << 16) | (dataMessage[3] << 24);

        bitset<32> newBitData(dataFullUint);

        for(int i = 0;i < bitLength;i++) {
            existing_bitset[i] = newBitData[i];
        }

    }

    protected: void constructReadChar(uint8_t reg, uint8_t *datagram) {

        datagram[0] =sync;
        datagram[1] =0;
        datagram[2] =reg;

        swuart_calcCRC(datagram, 4);

    }
    protected: void constructWriteChar(uint8_t reg, uint8_t *datagram, uint8_t *data) {

        datagram[0] = sync;
        datagram[1] = 0;
        datagram[2] =reg >> 1;

        byteswap(data);

        for(int i = 0; i < 4; i++) {
            datagram[i + 3] = data[i];
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

        interface.write_to_register(write);
        
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
        update();
    }
    public: void update() {

        uint8_t full_message[8];

        interface.read_register(read, full_message);
        
        consumeDataMessage(full_message, bitdata, 9);

        set_i_scale(bitdata[0]);
        set_internal_rSense(bitdata[1]);
        set_spreadCycle(bitdata[2]);
        set_shaft(bitdata[3]);
        set_index_otpw(bitdata[4]);
        set_index_step(bitdata[5]);
        set_pdn_disable(bitdata[6]);
        set_step_reg_select(bitdata[7]);
        set_multistep_filter(bitdata[8]);
        set_test_mode(bitdata[9]);
        
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

class gstat: protected tmc_register_base {
    public: bitset<32> bitdata;
    private: uint enable_pin;
    private: uint8_t reg = 0x01;
    private: bool reset = false;
    private: bool drv_err = false;
    private: bool uv_cp = false;
    public: uint8_t read[4];
    public: gstat(uint enable_pin) {
        this->enable_pin = enable_pin;
        constructReadChar(reg, read);
        update();
    }
    public: void update() {

        gpio_put(enable_pin, 0);

        uint8_t full_message[8];

        interface.read_register(read, full_message);

        gpio_put(enable_pin, 1);
        
        consumeDataMessage(full_message, bitdata, 3);

        reset = bitdata[0];
        drv_err = bitdata[1];
        uv_cp = bitdata[2];
        
    }
    public: bool get_reset() {
        return reset;
    }
    public: bool get_drv_err() {
        return drv_err;
    }
    public: bool get_uv_cp() {
        return uv_cp;
    }
};

class ifcnt: protected tmc_register_base {
    public: bitset<32> bitdata;
    private: uint8_t reg = 0x02;
    private: uint8_t count = 0;
    public: unsigned char read[4];
    public: ifcnt() {
        constructReadChar(reg, read);
        update();
    }
    public: void update() {

        
        uint8_t full_message[8];

        interface.read_register(read, full_message);
        
        consumeDataMessage(full_message, bitdata, 3);

        for(int i = 0; i < 8; i++) {
            count | (bitdata[i] << i);
        }

    }
    public: uint8_t get_count() {
        return count;
    }
};

class otp_read: protected tmc_register_base {
    public: bitset<32> bitdata;
    private: uint8_t reg = 0x05;
    private: uint8_t otp_fclktrim = 0;
    private: bool otp_ottrim = false;
    private: bool otp_internalRsense = false;
    private: bool otp_tbl = false;
    private: uint8_t otp_pwm_grad = 0;
    private: bool otp_pwm_autograd = false;
    private: uint8_t otp_tpwmthrs = 0;
    public: unsigned char read[4];
    public: otp_read() {
        constructReadChar(reg, read);
        update();
    }
    public: void update() {
        uint8_t full_message[8];

        interface.read_register(read, full_message);
        
        consumeDataMessage(full_message, bitdata, 24);

        otp_fclktrim =  bitdata[0] << bitdata[1] << bitdata[2] << bitdata[3] << bitdata[4]; 
        otp_pwm_grad = bitdata[8] << bitdata[9] << bitdata[10] << bitdata[11];
        otp_tpwmthrs = bitdata[13] << bitdata[14] << bitdata[15];
        otp_ottrim = bitdata[5];    
        otp_internalRsense = bitdata[6];    
        otp_tbl = bitdata[7];
        otp_pwm_autograd = bitdata[12];

    }
};

class inputPinStates: protected tmc_register_base {
    public: bitset<32> bitdata;
    private: uint8_t reg = 0x06;
    private: bool ENN = false;
    private: bool O1 = false;
    private: bool MS1 = false;
    private: bool MS2 = false;
    private: bool DIAG = false;
    private: bool O2 = false;
    private: bool PDN_UART = false;
    private: bool STEP = false;
    private: bool SPREAD_EN = false;
    private: bool DIR = false;
    public: unsigned char read[4];
    public: inputPinStates() {
        constructReadChar(reg, read);
        update();
    }
    public: void update() {

        uint8_t full_message[8];

        interface.read_register(read, full_message);
        
        consumeDataMessage(full_message, bitdata, 9);

        ENN = bitdata[0];
        O1 = bitdata[1];
        MS1 = bitdata[2];
        MS2 = bitdata[3];
        DIAG = bitdata[4];
        O2 = bitdata[5];
        PDN_UART = bitdata[6];
        STEP = bitdata[7];
        SPREAD_EN = bitdata[8];
        DIR = bitdata[9];
        
    }
    public: bool get_ENN() {
        return ENN;
    }
    public: bool get_O1() {
        return O1;
    }
    public: bool get_MS1() {
        return MS1;
    }
    public: bool get_MS2() {
        return MS2;
    }
    public: bool get_DIAG() {
        return DIAG;
    }
    public: bool get_O2() {
        return O2;
    }
    public: bool get_PDN_UART() {
        return PDN_UART;
    }
    public: bool get_STEP() {
        return STEP;
    }
    public: bool get_SPREAD_EN() {
        return SPREAD_EN;
    }
    public: bool get_DIR() {
        return DIR;
    }
};

class factory_conf: protected tmc_register_base {
    public: bitset<32> bitdata;
    public: uint8_t reg = 0x07;
    public: unsigned char read[4];
    public: unsigned char write[8];
    private: uint8_t fclktrim = 0;//bit 4..0
    private: uint8_t ottrim = 0;//bit 9..8
    public: factory_conf() {
        constructReadChar(reg, read);
        update();
    }
    public: void update() {

        uint8_t full_message[8];

        interface.read_register(read, full_message);
        
        consumeDataMessage(full_message, bitdata, 10);

        fclktrim = bitdata[0] | (bitdata[1] << 1) | (bitdata[2] << 2) | (bitdata[3] << 3);

        ottrim = bitdata[8] | (bitdata[9] << 1);

    }
    public: uint8_t get_fclktrim() {
        return fclktrim;
    }
    public: uint8_t get_ottrim() {
        return ottrim;
    }
    public: void set_fclktrim(uint8_t val) {
        if(val > 15) return;
        fclktrim = val;
        bitset<4> newBits(val);
        for(int i = 0; i < 4; i++) {
            bitdata[i] = newBits[i];
        };
        uint32_t data = bitdata.to_ulong();
        setWrite(data, write, reg);
    }
    public: void set_ottrim(uint8_t val) {
        if(val > 3) return;
        ottrim = val;
        bitset<2> newBits(val);
        for(int i = 0; i < 2; i++) {
            bitdata[i + 8] = newBits[i]; 
        }
        uint32_t data = bitdata.to_ulong();
        setWrite(data, write, reg);
    }
};
