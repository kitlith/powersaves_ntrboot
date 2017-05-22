#include <cstring>
#include <algorithm>
using std::memcpy;

#include <powerslaves.h>

#include "../device.h"

class R4i_Gold_3DS : Flashcart {
    protected:
        static const uint8_t cmdReadFlash[8];
        static const uint8_t cmdEraseFlash[8];
        static const uint8_t cmdWriteByteFlash[8];
        static const uint8_t cmdGetHWRevision[8];
        static const uint8_t cmdWaitFlashBusy[8];

    public:
        virtual bool setup() {
            uint32_t hw_revision;
            // TODO: check chipid?
            powerslaves_sendreceive(NTR, cmdGetHWRevision, 4, (uint8_t*)&hw_revision); // Get HW Revision
            if (hw_revision != 0xA6A6A6A6) return false;

            // Doesn't use any locking or unlocking functions?
            return true;
        }

        virtual void waitFlashBusy() {
            uint8_t state[4];
            while (true) {
                powerslaves_sendreceive(NTR, cmdWaitFlashBusy, 4, state);
                if((state[3] & 1) == 0) break;
            }
        }

        virtual void formatReadCommand(uint8_t *cmdbuf, uint32_t address) {
            memcpy(cmdbuf, cmdReadFlash, 8);
            cmdbuf[1] = (address >> 16) & 0xFF;
            cmdbuf[2] = (address >>  8) & 0xFF;
            cmdbuf[3] = (address >>  0) & 0xFF;
        }

        virtual void formatEraseCommand(uint8_t *cmdbuf, uint32_t address) {
            memcpy(cmdbuf, cmdEraseFlash, 8);
            cmdbuf[1] = (address >> 16) & 0xFF;
            cmdbuf[2] = (address >>  8) & 0xFF;
            cmdbuf[3] = (address >>  0) & 0xFF;
        }

        virtual void formatWriteCommand(uint8_t *cmdbuf, uint32_t address, uint8_t value) {
            memcpy(cmdbuf, cmdWriteByteFlash, 8);
            cmdbuf[1] = (address >> 16) & 0xFF;
            cmdbuf[2] = (address >>  8) & 0xFF;
            cmdbuf[3] = (address >>  0) & 0xFF;
            cmdbuf[4] = value;
        }

        virtual void cleanup() {
            // Nothing needed?
        };
};

const uint8_t R4i_Gold_3DS::cmdReadFlash[8] = {0xA5, 0x00, 0x00, 0x00, 0x00, 0x5A, 0x00, 0x00};
const uint8_t R4i_Gold_3DS::cmdEraseFlash[8] = {0xDA, 0x00, 0x00, 0x00, 0x00, 0xA5, 0x00, 0x00};
const uint8_t R4i_Gold_3DS::cmdWriteByteFlash[8] = {0xDA, 0x00, 0x00, 0x00, 0x00, 0x5A, 0x00, 0x00};
const uint8_t R4i_Gold_3DS::cmdWaitFlashBusy[8] = {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t R4i_Gold_3DS::cmdGetHWRevision[8] = {0xD1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

R4i_Gold_3DS r4igold3ds;
