#include "protocol.h"

Protocol::Command Protocol::parseFrame(uint8_t* arr,
                                       [[maybe_unused]] uint8_t len) {
    Command temp{(Commands)arr[1], (uint32_t)((arr[2] << 24) + (arr[3] << 16) +
                                              (arr[3] << 8) + arr[4])};
    if (arr[0] != TO) {
        temp.currentCommand = ERROR;
        return temp;
    }
    // check crc if uart
    // uint16_t CS = crc16(arr,len-2);
    //
    // if (temp.currentCommand == SEND_NEW_VAL_X ||
    //    temp.currentCommand == SEND_NEW_VAL_Y) {
    //    temp.val = (arr[2] << 24) + (arr[3] << 16) + (arr[3] << 8) + arr[4];
    //}
    return temp;
}

uint16_t Protocol::crc16(uint8_t* data, uint8_t len) {
    unsigned short reg_crc =
        0xFFFF; // Load a 16–bit register with FFFF hex (all 1’s).
    while (len--) {
        unsigned char data1 = *data;
        data++;
        reg_crc ^= data1;
        // Exclusive OR the first 8–bit byte of the message with the low–order
        // byte of the 16–bit CRC register, putting the result in the CRC
        // register.
        for (int j = 0; j < 8; j++) {
            // Shift the CRC register one bit to the right (toward the LSB),
            // zero–filling the MSB. //"EXTRACT!!!- то есть сравнивать байт до
            // XOR-а => в if-е" and examine the LSB.

            //(If the LSB was 1): Exclusive OR the CRC register with the
            // polynomial value 0xA001 (1010 0000 0000 0001).
            if (reg_crc & 0x0001) {
                reg_crc = (reg_crc >> 1) ^ 0xA001;
            } // LSB(b0)=1
            // (If the LSB was 0): Repeat Step 3 (another shift).
            else
                reg_crc = reg_crc >> 1;
        }
    }
    // uint8_t CRChi=reg_crc>>8;
    // uint8_t CRClo=reg_crc&0x00FF;
    // uint16_t mod=(CRClo<<8|CRChi);
    // reg_crc=mod;
    return reg_crc;
}
