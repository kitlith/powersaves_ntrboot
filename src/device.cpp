#include <cstdio>
#include <cstdlib>
#include <powerslaves.h>

#include "device.h"

const uint8_t Flashcart::NTR_cmdDummy[8] = {0x9F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t Flashcart::NTR_cmdChipid[8] = {0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

Flashcart::Flashcart() {
    flashcart_list.push_back(this);
}

void Flashcart::reset() {
    uint8_t *garbage = new uint8_t[0x2000];
    if (!garbage) {
        puts("Memory allocation failure!");
        exit(EXIT_FAILURE);
    }

    powerslaves_mode(ROM_MODE);
    powerslaves_sendreceive(NTR, NTR_cmdDummy, 0x2000, garbage);
    delete[] garbage;  // TODO: prevent thrashing?
}

void Flashcart::readFlash(uint32_t address, uint32_t length, uint8_t *buf) {
    uint8_t cmdbuf[8];

    // Read less if less is requested, otherwise read the maximum that we can.
    length = std::min<uint32_t>(0x200000, length);
    uint32_t endaddr = address + length; // TODO: make sure that length is aligned?
    uint8_t *curpos = buf;

    for (uint32_t addr=address; addr < endaddr; addr += 0x200, curpos += 0x200) {
        formatReadCommand(cmdbuf, addr);
        powerslaves_sendreceive(NTR, cmdbuf, 0x200, curpos);
    }
}

void Flashcart::eraseFlash(uint32_t address, uint32_t length) {
    // TODO: make sure length is aligned?
    uint8_t cmdbuf[8];

    length = std::min<uint32_t>(0x200000, length);
    uint32_t endaddr = address + length;

    //printf("Erasing Flash.\n");
    for (uint32_t addr=address; addr < endaddr; addr+=64*1024) {
        formatEraseCommand(cmdbuf, addr);
        powerslaves_send(NTR, cmdbuf, 0);
        waitFlashBusy();
    }
}

void Flashcart::writeFlash(uint32_t address, uint32_t length, const uint8_t *buf) {
    uint8_t cmdbuf[8];

    length = std::min<uint32_t>(0x200000, length);
    uint32_t endaddr = address + length;
    const uint8_t *curpos = buf;

    //printf("Flashing. This may take a while.\n");
    for (uint32_t addr=0; addr < endaddr; ++addr, ++curpos) {
        formatWriteCommand(cmdbuf, addr, *curpos);
        powerslaves_send(NTR, cmdbuf, 0);
        waitFlashBusy();
    }
}
