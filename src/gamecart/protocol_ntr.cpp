#include <cstdio>
#include <cstdint>
#include <powerslaves.h>
#include "gamecart/protocol_ntr.h"

using std::puts;

int Cart_NTRInit() {
    return powerslaves_mode(ROM_MODE);
}

void NTR_SendCommand(const uint8_t command[8], uint32_t pageSize, uint32_t latency, void* buffer) {
    powerslaves_sendreceive(NTR, command, pageSize, static_cast<uint8_t*>(buffer));
}
