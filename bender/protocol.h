#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

namespace Protocol {

enum Direction {
    TO,
    FROM
};

enum Commands {
    SEND_START,
    SEND_NEW_VAL_Y,
    SEND_NEW_VAL_X,
    SEND_Y_PLUS,
    SEND_Y_MINUS,
    SEND_X_PLUS,
    SEND_X_MINUS,
};

enum Replies {
    START,
    VAL_Y,
    VAL_X,
    CURRENT_Y,
    CURRENT_X,
    LIMIT_Y_PLUS,
    LIMIT_Y_MINUS,
    LIMIT_X_PLUS,
    LIMIT_X_MINUS,
    STOP
};
// send
// 0 - TO
// 1 - COMMAND
// 2 - BYTE_HIGH_HIGH
// 3 - BYTE_HIGH
// 4 - BYTE_LOW
// 5 - BYTE_LOW_LOW
// 6 - CS_HIGH
// 7 - CS_LOW

// receive
// 0 - FROM
// 1 - COMMAND
// 2 - BYTE_HIGH_HIGH
// 3 - BYTE_HIGH
// 4 - BYTE_LOW
// 5 - BYTE_LOW_LOW
// 6 - CS_HIGH
// 7 - CS_LOW

uint16_t Crc16(uint8_t* data, uint8_t length);

} // namespace Protocol

#endif // PROTOCOL_H
