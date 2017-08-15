#include <powerslaves.h>
#include <device.h>

void Flashcart::platformInit() {
    // TODO error check
    powerslaves_mode(ROM_MODE);
    return;
}

void Flashcart::sendCommand(const uint8_t *cmdbuf, uint16_t response_len, uint8_t *resp) {
    powerslaves_sendreceive(NTR, cmdbuf, response_len, resp);
}

void Flashcart::showProgress(uint32_t current, uint32_t total) {
    // TODO
}
