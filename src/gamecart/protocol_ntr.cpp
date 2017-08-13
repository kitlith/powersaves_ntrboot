#include <cstdint>
#include <powerslaves.h>
#include "gamecart/protocol_ntr.h"

void Cart_NTRInit() {
    // ResetCartSlot();
    // delay()
    // SwitchToNTRCARD();
    // delay();
    // ...
}

void NTR_SendCommand(const uint8_t command[8], uint32_t pageSize, uint32_t latency, void* buffer) {
    powerslaves_sendreceive(NTR, command, pageSize, static_cast<uint8_t*>(buffer));
}
