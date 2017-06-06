#include <cstring>
#include <algorithm>
#include <powerslaves.h>

#include "../device.h"

class Genuine_ak2i_44 : protected Flashcart {
    protected:
        static const uint8_t ak2i_cmdSetMapTableAddress[8];
        static const uint8_t ak2i_cmdActiveFatMap[8];
        static const uint8_t ak2i_cmdUnlockFlash[8];
        static const uint8_t ak2i_cmdLockFlash[8];
        static const uint8_t ak2i_cmdUnlockASIC[8];
        static const uint8_t ak2i_cmdGetHWRevision[8];
        static const uint8_t ak2i_cmdWaitFlashBusy[8];

        // 0xA5..0x5A on R4i Gold 3DS
        static const uint8_t ak2i_cmdReadFlash[8];
        // 0xDA..0xA5 on R4i Gold 3DS
        static const uint8_t ak2i_cmdEraseFlash[8];

        // 0xDA..0x5A on R4i Gold 3DS
        static const uint8_t ak2i_cmdWriteByteFlash[8];

        virtual void waitFlashBusy() {
            uint8_t state[4];
            while (true) {
                powerslaves_sendreceive(NTR, ak2i_cmdWaitFlashBusy, 4, state);
                if((state[3] & 1) == 0) break;
            }
        }

        virtual unsigned getMaxLength() { return 0x200000; }

        virtual void formatReadCommand(uint8_t *cmdbuf, uint32_t address) {
            memcpy(cmdbuf, ak2i_cmdReadFlash, 8);
            cmdbuf[1] = (address >> 24) & 0xFF;
            cmdbuf[2] = (address >> 16) & 0xFF;
            cmdbuf[3] = (address >>  8) & 0xFF;
            cmdbuf[4] = (address >>  0) & 0xFF;
        }

        virtual void formatEraseCommand(uint8_t *cmdbuf, uint32_t address) {
            memcpy(cmdbuf, ak2i_cmdEraseFlash, 8);
            cmdbuf[1] = (address >> 16) & 0x1F;
            cmdbuf[2] = (address >>  8) & 0xFF;
            cmdbuf[3] = (address >>  0) & 0xFF;
        }

        virtual void formatWriteCommand(uint8_t *cmdbuf, uint32_t address, uint8_t value) {
            memcpy(cmdbuf, ak2i_cmdWriteByteFlash, 8);
            cmdbuf[1] = (address >> 16) & 0x1F;
            cmdbuf[2] = (address >>  8) & 0xFF;
            cmdbuf[3] = (address >>  0) & 0xFF;
            cmdbuf[4] = value;
        }

    public:
        virtual bool setup() {
            uint32_t chipid;
            uint32_t hw_revision;
            uint32_t garbage;

            reset(); // Known good state.
            powerslaves_sendreceive(NTR, NTR_cmdChipid, 4, (uint8_t*)&chipid);
            if (chipid != 0x00000FC2) return false;

            powerslaves_sendreceive(NTR, ak2i_cmdGetHWRevision, 4, (uint8_t*)&hw_revision); // Get HW Revision
            if (hw_revision != 0x44444444) return false;

            powerslaves_send(NTR, ak2i_cmdSetMapTableAddress, 0); // TODO: Not all of this is needed when reading, is it?
            powerslaves_sendreceive(NTR, ak2i_cmdActiveFatMap, 4, (uint8_t*)&garbage);
            powerslaves_send(NTR, ak2i_cmdUnlockFlash, 0);
            powerslaves_send(NTR, ak2i_cmdUnlockASIC, 0);
            return true;
        }

        virtual void cleanup() {
            uint32_t garbage;
            powerslaves_send(NTR, ak2i_cmdLockFlash, 0);
            powerslaves_sendreceive(NTR, ak2i_cmdActiveFatMap, 4, (uint8_t *)&garbage);
        }
};

const uint8_t Genuine_ak2i_44::ak2i_cmdSetMapTableAddress[8] = {0xD0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t Genuine_ak2i_44::ak2i_cmdActiveFatMap[8] = {0xC2, 0x55, 0xAA, 0x55, 0xAA, 0x00, 0x00, 0x00};
const uint8_t Genuine_ak2i_44::ak2i_cmdUnlockFlash[8] = {0xC2, 0xAA, 0x55, 0xAA, 0x55, 0x00, 0x00, 0x00};
const uint8_t Genuine_ak2i_44::ak2i_cmdLockFlash[8] = {0xC2, 0xAA, 0xAA, 0x55, 0x55, 0x00, 0x00, 0x00};
const uint8_t Genuine_ak2i_44::ak2i_cmdUnlockASIC[8] = {0xC2, 0xAA, 0x55, 0x55, 0xAA, 0x00, 0x00, 0x00};
const uint8_t Genuine_ak2i_44::ak2i_cmdWaitFlashBusy[8] = {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t Genuine_ak2i_44::ak2i_cmdGetHWRevision[8] = {0xD1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// 0xA5..0x5A on R4i Gold 3DS
const uint8_t Genuine_ak2i_44::ak2i_cmdReadFlash[8] = {0xB7, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00};
// 0xDA..0xA5 on R4i Gold 3DS
const uint8_t Genuine_ak2i_44::ak2i_cmdEraseFlash[8] = {0xD4, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};

// 0xDA..0x5A on R4i Gold 3DS
const uint8_t Genuine_ak2i_44::ak2i_cmdWriteByteFlash[8] = {0xD4, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00};

class Genuine_ak2i_81 : protected Genuine_ak2i_44 {
    protected:
        static const uint8_t ak2i_cmdSetFlash1681_81[8];
        static const uint8_t ak2i_cmdEraseFlash81[8];
        static const uint8_t ak2i_cmdWriteByteFlash81[8];

        virtual unsigned getMaxLength() { return 0x1000000; }

        virtual void formatEraseCommand(uint8_t *cmdbuf, uint32_t address) {
            memcpy(cmdbuf, ak2i_cmdEraseFlash81, 8);
            cmdbuf[1] = (address >> 16) & 0x1F;
            cmdbuf[2] = (address >>  8) & 0xFF;
            cmdbuf[3] = (address >>  0) & 0xFF;
        }

        virtual void formatWriteCommand(uint8_t *cmdbuf, uint32_t address, uint8_t value) {
            memcpy(cmdbuf, ak2i_cmdWriteByteFlash81, 8);
            cmdbuf[1] = (address >> 16) & 0x1F;
            cmdbuf[2] = (address >>  8) & 0xFF;
            cmdbuf[3] = (address >>  0) & 0xFF;
            cmdbuf[4] = value;
        }

    public:
        virtual bool setup() {
            uint32_t hw_revision;
            uint32_t garbage;
            uint32_t chipid;

            reset(); // Known good state.
            powerslaves_sendreceive(NTR, NTR_cmdChipid, 4, (uint8_t*)&chipid);
            if (chipid != 0x00000FC2) return false;

            powerslaves_sendreceive(NTR, ak2i_cmdGetHWRevision, 4, (uint8_t*)&hw_revision); // Get HW Revision
            if (hw_revision != 0x81818181) return false;

            powerslaves_send(NTR, ak2i_cmdSetMapTableAddress, 0);
            powerslaves_send(NTR, ak2i_cmdSetFlash1681_81, 0);
            powerslaves_sendreceive(NTR, ak2i_cmdActiveFatMap, 4, (uint8_t*)&garbage);
            powerslaves_send(NTR, ak2i_cmdUnlockFlash, 0);
            powerslaves_send(NTR, ak2i_cmdUnlockASIC, 0);
            return true;
        }
};

const uint8_t Genuine_ak2i_81::ak2i_cmdSetFlash1681_81[8] = {0xD8, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC6, 0x06};
const uint8_t Genuine_ak2i_81::ak2i_cmdEraseFlash81[8] = {0xD4, 0x00, 0x00, 0x00, 0x30, 0x80, 0x00, 0x35};
const uint8_t Genuine_ak2i_81::ak2i_cmdWriteByteFlash81[8] = {0xD4, 0x00, 0x00, 0x00, 0x30, 0xa0, 0x00, 0x63};

Genuine_ak2i_44 ak2i_44; // These are automatically added to the list of types to check.
Genuine_ak2i_81 ak2i_88;
